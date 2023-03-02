#include "pch.h"
#include <stdexcept>
#include "util.h"
#include "rolemanager.h"
#include "pb/project.pb.h"
#include "pb/enum.pb.h"
#include "rolesummary.h"
#include "rolesummarymgr.h"
#include "util/timespecificmgr.h"
#include "util/gametime.h"
#include "logutil.h"

INSTANCE_SINGLETON(CRoleManager)


CRoleManager::CRoleManager()
{
}

CRoleManager::~CRoleManager()
{
}

bool CRoleManager::Init()
{
	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	m_timer = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&CRoleManager::OnTimer, this));
	return true;
}


bool CRoleManager::OnTimer()
{
	for (auto iter = m_oRoleDataMap.begin(); iter != m_oRoleDataMap.end(); ++iter)
	{
		CRole* pRole = *iter;
		pRole->DailyClean();
	}
	return true;
}

void CRoleManager::Uninit()
{
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

CRole* CRoleManager::LoginRole(const KKSG::RoleInfoOnMs *pInfo, UINT64 sessionID)
{
	CRole *pRole = new CRole;
	pRole->Init(*pInfo, sessionID);
	AddRole(pRole);

	LogInfo("Role {account=%s, roleid=%llu, name=%s} Login", pRole->GetAccount().c_str(), pRole->GetID(), pRole->GetName().c_str());

	pRole->OnLogin(pInfo->level());
	return pRole;
}

void CRoleManager::ConvertSession(CRole* pRole, SessionType newSessionID)
{
	if (pRole == NULL)
	{
		return;
	}
	RemoveRole(pRole);
	pRole->SetSessionID(newSessionID);
	AddRole(pRole);
}

void CRoleManager::LogoutRole(UINT64 sessionID, UINT32 level)
{
	CRole *pRole = GetBySession(sessionID);
	if (pRole != NULL)
	{
		LogInfo("Role {account=%s, roleid=%llu, name=%s} Logout", pRole->GetAccount().c_str(), pRole->GetID(), pRole->GetName().c_str());

		pRole->OnLogout(level);
		RemoveRole(pRole);
		pRole->Uninit();
		delete pRole;
	}
}

CRole* CRoleManager::RecoverRole(const KKSG::RoleInfoOnMs *pInfo, UINT64 sessionID)
{
	CRole *pRole = new CRole;
	pRole->Init(*pInfo, sessionID);
	AddRole(pRole);

	LogInfo("Role {account=%s, roleid=%llu, name=%s} Recover from gs", pInfo->accountid().c_str(), pInfo->roleid(), pInfo->name().c_str());

	return pRole;
}

bool CRoleManager::IsOnline(UINT64 roleid)
{
	CRole* role = GetByRoleID(roleid);
	return NULL != role;
}

void CRoleManager::GetRoleState(UINT64 roleid, UINT32& type, UINT32& lastlogout)
{
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
	if (NULL != summary)
	{
		lastlogout = summary->GetLastLogout();
	}
	if (0 == lastlogout)
	{
		type = KKSG::InHall;
	}
	else
	{
		type = KKSG::Logoff;
	}
}

void CRoleManager::GetFilterRole(CRoleSummary* me, FilterRole filter, std::vector<UINT64>& roles
	, std::vector<UINT64>& exroles)
{
	for (auto i = m_oRoleDataMap.begin(); i != m_oRoleDataMap.end(); ++i)
	{
		CRole* role = *i;
		UINT64 roleid = role->GetID();
		if (me->GetID() == roleid)
		{
			continue;
		}
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
		if (NULL == summary)
		{
			continue;
		}
		UINT32 ret = filter(me, summary);
		if (0 == ret)
		{
			roles.push_back(role->GetID());
		}
		else if (1 == ret)
		{
			exroles.push_back(role->GetID());
		}
	}
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

void CRoleManager::LogoutRoleOnGsLine(UINT32 dwLine, bool isCross)
{
	std::vector<UINT64> oSessionList;
	role_iterator it = Begin();
	role_iterator itEnd = End();
	for(; it != itEnd; ++it)
	{
		CRole* poRole = *it;
		if(poRole->GetGsLine() == dwLine && poRole->IsInCrossGs() == isCross)
		{
			oSessionList.push_back(poRole->GetSessionID());
		}
	}

	if(!oSessionList.empty())
	{
		LogWarn("Gameserver line [%u] crash, Kickout [%u] roles in %s_%u", dwLine, oSessionList.size(), GetGsPrefix(isCross), dwLine);
	}

	for(std::vector<UINT64>::size_type i = 0; i < oSessionList.size(); ++i)
	{
		LogoutRole(oSessionList[i], 0);
	}
}