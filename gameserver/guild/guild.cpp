#include "pch.h"
#include "guild.h"
#include "pb/project.pb.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guildmgr.h"
#include "unit/roleoutlook.h"
#include "foreach.h"



CGuildSimple::CGuildSimple() : m_guildId(0), m_guildLvl(0), m_guildIcon(0)
{
}

CGuildSimple::~CGuildSimple()
{
}


void CGuildSimple::BroadCast(const CProtocol& ptcMsg)
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

void CGuildSimple::AddGuildMember(UINT64 roleId, UINT32 position) 
{ 
	m_allMemberMap[roleId] = position; 
	GuildSimpleMgr::Instance()->DoAddGuild(roleId, m_guildId);

	Role* pRole = RoleManager::Instance()->FindByRoleID(roleId);
	if (pRole != NULL)
	{

		RoleOutLook  outLook(pRole);
		outLook.Fill(SUMMARY_GUILD, pRole);
		outLook.BroadCast();
	}
}

void CGuildSimple::DeleteMember(UINT64 roleId) 
{ 
	GuildSimpleMgr::Instance()->DoLeaveGuild(roleId);
	m_allMemberMap.erase(roleId); 

	Role* pRole = RoleManager::Instance()->FindByRoleID(roleId);
	if (pRole != NULL)
	{

		RoleOutLook  outLook(pRole);
		outLook.Fill(SUMMARY_GUILD, pRole);
		outLook.BroadCast();
	}
}

UINT64 CGuildSimple::GetGuildLeaderId()
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

UINT64 CGuildSimple::GetGuildLeader2Id()
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

UINT32 CGuildSimple::GetPosition(UINT64 roleId)
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

void CGuildSimple::FillGuildBrief(KKSG::MyGuild* ginfo)
{
	if(NULL == ginfo)
	{
		return ;
	}
	ginfo->set_gid(m_guildId);
	ginfo->set_name(m_guildName);
	ginfo->set_level(m_guildLvl);
	ginfo->set_icon(m_guildIcon);
}