#include "pch.h"
#include "dragonguildmgr.h"
#include "foreach.h"


INSTANCE_SINGLETON(DragonGuildSimpleMgr)
DragonGuildSimpleMgr::DragonGuildSimpleMgr()
{

}

DragonGuildSimpleMgr::~DragonGuildSimpleMgr()
{

}

bool DragonGuildSimpleMgr::Init()
{

	return true;
}

void DragonGuildSimpleMgr::Uninit()
{
	return ;
}

CDragonGuildSimple* DragonGuildSimpleMgr::GetDragonGuildSimple(UINT64 guildId)
{
	auto iter = m_dragonGuildSimpleMap.find(guildId);
	return iter == m_dragonGuildSimpleMap.end() ? NULL : &(iter->second);
}

void DragonGuildSimpleMgr::DoAddDragonGuild(UINT64 roleId, UINT64 guildId)
{
	m_roleDragonGuildMap[roleId] = guildId;
}

void DragonGuildSimpleMgr::DoLeaveDragonGuild(UINT64 roleId)
{
	m_roleDragonGuildMap.erase(roleId);
}

void DragonGuildSimpleMgr::DeleteDragonGuildSimple(UINT64 guildId)
{
	auto iter = m_dragonGuildSimpleMap.find(guildId);
	if (iter == m_dragonGuildSimpleMap.end())
	{
		return ;
	}

	const std::map<UINT64, UINT32>& allMember = iter->second.getAllMember();
	foreach (i in allMember)
	{
		DoLeaveDragonGuild(i->first);
	}

	m_dragonGuildSimpleMap.erase(iter);
}

std::string DragonGuildSimpleMgr::GetDragonGuildName(UINT64 guildId)
{
	auto pGuild = GetDragonGuildSimple(guildId);
	if(pGuild)
	{
		return pGuild->GetDragonGuildName();
	}
	return "";
}

void DragonGuildSimpleMgr::SetDragonGuildAttr(UINT64 guildid, UINT32 count)
{
	m_dragonGuild2attrcount[guildid] = count;
}

UINT32 DragonGuildSimpleMgr::GetDragonGuildAttr(UINT64 guildid)
{
	auto i = m_dragonGuild2attrcount.find(guildid);
	if (i == m_dragonGuild2attrcount.end())
	{
		return 0;
	}
	else
	{
		return i->second;
	}
}

bool DragonGuildSimpleMgr::IsInSameDragonGuild(UINT64 roleId1, UINT64 roleId2)
{
	if (m_roleDragonGuildMap.find(roleId1) == m_roleDragonGuildMap.end() || m_roleDragonGuildMap.find(roleId2) == m_roleDragonGuildMap.end())
	{
		return false;
	}
	return m_roleDragonGuildMap[roleId1] == m_roleDragonGuildMap[roleId2];
}
