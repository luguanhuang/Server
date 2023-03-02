#include "pch.h"
#include "systeminfo.h"
#include "time.h"
#include "pb/project.pb.h"
#include "table/OpenSystemMgr.h"
#include "table/ChannelCloseSys.h"
#include "define/systemiddef.h"
#include "config.h"
#include "unit/role.h"


CRoleSystem::CRoleSystem(Role *pRole)
{
	m_pRole = pRole;
}

CRoleSystem::~CRoleSystem()
{
	m_pRole = NULL;
}

void CRoleSystem::FirstInit(UINT32 roleNum)
{
	/*
	const std::vector<int> &initSystems = OpenSystemMgr::Instance()->OpendSystemWhenCreateRole();
	for (UINT32 j = 0; j < initSystems.size(); ++j)
	{
		m_openSystem.Set(initSystems[j], true);
	}
	m_ischanged.Set();
	*/
}

bool CRoleSystem::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	const KKSG::RoleSystem& system = poRoleAllInfo->system();
	m_openSystem.FromString(system.system());
	m_firsttime.FromString(system.firsttime());
	for (int i = 0; i < system.opentime_size(); ++i)
	{
		m_openTime[system.opentime(i).id()] = system.opentime(i).time();
	}
	return true;
}

void CRoleSystem::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (!m_ischanged.TestAndReset())
	{
		return;
	}
	KKSG::RoleSystem* system = poRoleAllInfo->mutable_system();
	system->Clear();

	system->set_system(SystemBits());
	system->set_firsttime(FirstBits());
	for (auto i = m_openTime.begin(); i != m_openTime.end(); ++i)
	{
		KKSG::SysOpenTime* openTime = system->add_opentime();
		openTime->set_id(i->first);
		openTime->set_time(i->second);
	}

	roChanged.insert(system);
}

void CRoleSystem::SetSystemOpened(int sysID, bool isopen /*= true*/)
{
	m_ischanged.Set();

	m_openSystem.Set(sysID, isopen);

	if (isopen && m_openTime.find(sysID) == m_openTime.end())
	{
		m_openTime[sysID] = (int)time(0);
	}
}

bool CRoleSystem::IsSystemOpened(int sysID)
{
	if (sysID == SYS_QQDIANJING && GSConfig::Instance()->GetAppType() != KKSG::GAME_APP_QQ)
	{
		return false;
	}
	//const KKSG::RoleAllInfo&  roleAllInfo =  m_pRole->GetRoleAllInfo();
	//if(roleAllInfo.has_extrainfo()&&roleAllInfo.extrainfo().has_fristchannel())
	//{
	//	if (ChannelCloseSysMgr::Instance()->IsSysOpen(m_pRole,sysID, m_pRole->GetClientConf().loginchannel())==false)
	//		return false;
	//}
	if (ChannelCloseSysMgr::Instance()->IsSysOpen(m_pRole,sysID, m_pRole->GetClientConf().loginchannel())==false)
		return false;
	return IsSystemOpenedAtRole(sysID) && !OpenSystemMgr::Instance()->IsClosedSystem(sysID, m_pRole->GetID()); 
}

bool CRoleSystem::IsSystemOpenedAtRole(int sysID)
{
	return m_openSystem.check(sysID);
}

int CRoleSystem::GetSystemOpenTime(int sysID)
{
	auto find = m_openTime.find(sysID);
	if (find != m_openTime.end())
	{
		return find->second;
	}

	return 0;
}

void CRoleSystem::SetAllSystemOpened()
{
	for (int i = 0; i <= MAX_SYSTEM_ID; ++i)
	{
		SetSystemOpened(i, true);
	}
}

