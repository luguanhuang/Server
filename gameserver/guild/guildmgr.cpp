#include "pch.h"
#include "guildmgr.h"
#include "foreach.h"

INSTANCE_SINGLETON(GuildSimpleMgr)
GuildSimpleMgr::GuildSimpleMgr()
{

}

GuildSimpleMgr::~GuildSimpleMgr()
{
	
}

bool GuildSimpleMgr::Init()
{
	return true;
}

void GuildSimpleMgr::Uninit()
{
	return ;
}

CGuildSimple* GuildSimpleMgr::GetGuildSimple(UINT64 guildId)
{
	auto iter = m_guildSimpleMap.find(guildId);
	return iter == m_guildSimpleMap.end() ? NULL : &(iter->second);
}

void GuildSimpleMgr::DoAddGuild(UINT64 roleId, UINT64 guildId)
{
	m_roleGuildMap[roleId] = guildId;
}

void GuildSimpleMgr::DoLeaveGuild(UINT64 roleId)
{
	m_roleGuildMap.erase(roleId);
}

void GuildSimpleMgr::DeleteGuildSimple(UINT64 guildId)
{
	auto iter = m_guildSimpleMap.find(guildId);
	if (iter == m_guildSimpleMap.end())
	{
		return ;
	}

	const std::map<UINT64, UINT32>& allMember = iter->second.getAllMember();
	foreach (i in allMember)
	{
		DoLeaveGuild(i->first);
	}

	m_guildSimpleMap.erase(iter);
}

std::string GuildSimpleMgr::GetGuildName(UINT64 guildId)
{
	auto pGuild = GetGuildSimple(guildId);
	if(pGuild)
	{
		return pGuild->GetGuildName();
	}
	return "";
}

void GuildSimpleMgr::SetGuildAttr(UINT64 guildid, UINT32 count)
{
	m_guild2attrcount[guildid] = count;
}

UINT32 GuildSimpleMgr::GetGuildAttr(UINT64 guildid)
{
	auto i = m_guild2attrcount.find(guildid);
	if (i == m_guild2attrcount.end())
	{
		return 0;
	}
	else
	{
		return i->second;
	}
}
