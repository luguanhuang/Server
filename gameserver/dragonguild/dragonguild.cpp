#include "pch.h"
#include "dragonguild.h"
#include "pb/project.pb.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "dragonguildmgr.h"
#include "unit/roleoutlook.h"
#include "dragonguild/dragonguildrecord.h"
#include "foreach.h"



CDragonGuildSimple::CDragonGuildSimple() : m_dragonGuildId(0), m_dragonGuildLvl(0)
{
}

CDragonGuildSimple::~CDragonGuildSimple()
{
}


void CDragonGuildSimple::BroadCast(const CProtocol& ptcMsg)
{
	for (auto i = m_allMemberMap.begin(); i != m_allMemberMap.end(); ++i)
	{
		Role* role = RoleManager::Instance()->FindByRoleID(i->first);
		if (NULL != role)
		{
			role->Send(ptcMsg);
		}
	}
}

void CDragonGuildSimple::AddDragonGuildMember(UINT64 roleId, UINT32 position) 
{ 
	m_allMemberMap[roleId] = position; 
	DragonGuildSimpleMgr::Instance()->DoAddDragonGuild(roleId, m_dragonGuildId);

	//加入工会成功, 如果玩家在线，检查一下任务和活跃红点
	Role* pRole = RoleManager::Instance()->FindByRoleID(roleId);
	if(pRole == NULL)
		return;
	DragonGuildRecord * record = pRole->Get<DragonGuildRecord>();
	if(record == NULL)
		return;
	record->CheckRedPoint();
}

void CDragonGuildSimple::DeleteMember(UINT64 roleId) 
{ 
	DragonGuildSimpleMgr::Instance()->DoLeaveDragonGuild(roleId);
	m_allMemberMap.erase(roleId); 

	//Role* pRole = RoleManager::Instance()->FindByRoleID(roleId);
}

UINT64 CDragonGuildSimple::GetDragonGuildLeaderId()
{
	foreach (i in m_allMemberMap)
	{
		if (i->second == GPOS_LEADER)
		{
			return i->first;
		}
	}

	return 0;
}

UINT64 CDragonGuildSimple::GetDragonGuildLeader2Id()
{
	foreach (i in m_allMemberMap)
	{
		if (i->second == GPOS_VICELEADER)
		{
			return i->first;
		}
	}

	return 0;
}

UINT32 CDragonGuildSimple::GetPosition(UINT64 roleId)
{
	foreach (i in m_allMemberMap)
	{
		if (i->first == roleId)
		{
			return i->second;
		}
	}

	return GPOS_MAX;
}

void CDragonGuildSimple::FillDragonGuildBrief(KKSG::MyGuild* ginfo)
{
	if(NULL == ginfo)
	{
		return ;
	}
	ginfo->set_gid(m_dragonGuildId);
	ginfo->set_name(m_dragonGuildName);
	ginfo->set_level(m_dragonGuildLvl);
}