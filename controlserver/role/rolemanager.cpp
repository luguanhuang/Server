#include "pch.h"
#include "util.h"
#include "rolemanager.h"
#include "pb/project.pb.h"
#include "pb/enum.pb.h"
#include "util/gametime.h"
#include "shm/shmmgr.h"

INSTANCE_SINGLETON(CRoleManager)


CRoleManager::CRoleManager()
{
}

CRoleManager::~CRoleManager()
{
}

bool CRoleManager::Init()
{
	RecoverFromShm();
	return true;
}

void CRoleManager::Uninit()
{
}

void CRoleManager::RecoverFromShm()
{
	std::vector<CShmRole*> shmRoles;
	ShmMgr::Instance()->GetRoleShm().GetAll(shmRoles);
	if (shmRoles.empty())
	{
		return;
	}
	LogWarn("Recover %u role from shm", shmRoles.size());

	for (size_t i = 0; i < shmRoles.size(); ++i)
	{
		CShmRole* shmData = shmRoles[i];
		CRole* poRole = new CRole();
		poRole->Init(shmData);
		LogInfo("Recover role[%llu]", poRole->GetID());
		AddRole(poRole);
	}
}

void CRoleManager::AddRole(CRole* role)
{
	if(GetByRoleID(role->GetID()) != NULL)
	{
		LogError("Role id [%llu], session [%llu] already exist", role->GetID(), role->GetSessionID());
	}

	m_oRoleDataMap.insert(role);	
	m_oName2IdMap[role->GetName()] = role->GetID();
	LogDebug("Add role, id [%llu], session [%llu]", role->GetID(), role->GetSessionID());
}

void CRoleManager::RemoveRole(CRole* role)
{
	m_oRoleDataMap.erase<SessionIndex>(role->GetSessionID());

	auto i = m_oName2IdMap.find(role->GetName());
	if (i != m_oName2IdMap.end())
	{
		m_oName2IdMap.erase(i);
	}
}

CRole * CRoleManager::GetBySession(SessionType sessionID)
{
	return m_oRoleDataMap.find<SessionIndex>(sessionID);
}

CRole * CRoleManager::GetByRoleID(UINT64 roleID)
{
	return m_oRoleDataMap.find<RoleIDIndex>(roleID);
}

CRole * CRoleManager::GetByName(const std::string& name)
{
	auto i = m_oName2IdMap.find(name);
	if (i == m_oName2IdMap.end())
	{
		return NULL;
	}
	return GetByRoleID(i->second);
}

void CRoleManager::RemoveBySession(SessionType sessionID)
{
	CRole *pRole = GetBySession(sessionID);
	if (pRole != NULL)
	{
		LogDebug("Remove role, id [%llu], session [%llu]", pRole->GetID(), pRole->GetSessionID());

		RemoveRole(pRole);
		pRole->Uninit();
		delete pRole;
	}
	else
	{
		LogWarn("Remove role by session [%llu], but not found", sessionID);
	}
}

static CRole *GetRole(const std::string &RoleIDString)
{
	UINT64 RoleID = convert<UINT64>(RoleIDString);
	return CRoleManager::Instance()->GetByRoleID(RoleID);
}

CRole* CRoleManager::ConvertSession(SessionType oldSession, SessionType newSession)
{
	CRole* pRole = GetBySession(oldSession);
	if (pRole == NULL)
	{
		return NULL;
	}
	RemoveRole(pRole);
	pRole->SetSessionID(newSession);
	AddRole(pRole);
	return pRole;
}

CRole* CRoleManager::LoginRole(const KKSG::RoleAllInfo *pInfo, const KKSG::ClientInfo& roCliConf, UINT64 sessionID)
{
	CRole *pRole = new CRole;
	pRole->Init(*pInfo, sessionID);
	AddRole(pRole);

	LogInfo("Role {account=%s, serverid=%u roleid=%llu, name=%s} Login",
		pRole->GetAccount().c_str(), pRole->GetServerID(), pRole->GetID(), pRole->GetName().c_str());

	pRole->OnLogin();
	return pRole;
}

void CRoleManager::LogoutRole(UINT64 sessionID)
{
	CRole *pRole = GetBySession(sessionID);
	if (pRole != NULL)
	{
		LogInfo("Role {account=%s, serverid=%u roleid=%llu, name=%s} Logout",
			pRole->GetAccount().c_str(), pRole->GetServerID(), pRole->GetID(), pRole->GetName().c_str());

		pRole->OnLogout();
		RemoveRole(pRole);
		pRole->Uninit();
		delete pRole;
	}
}

bool CRoleManager::IsOnline(UINT64 roleid)
{
	CRole* role = GetByRoleID(roleid);
	return NULL != role;
}

void CRoleManager::BroadCast(const CProtocol& ptc, UINT32 line)
{
	for (auto i = m_oRoleDataMap.begin(); i != m_oRoleDataMap.end(); ++i)
	{
		CRole* role = *i;
		if (0 == line || role->GetGsLine() == line)
		{
			role->Send(ptc);
		}
	}
}

UINT32 CRoleManager::GetRoleCount(UINT32 line, bool isCross)
{
	UINT32 count = 0;
	for (auto i = m_oRoleDataMap.begin(); i != m_oRoleDataMap.end(); ++i)
	{
		CRole* role = *i;
		if (role->GetGsLine() == line && role->IsInCrossGs() == isCross)
		{
			count++;
		}
	}

	return count;
}