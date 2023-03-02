#ifndef __DRAGON_GUILDMGR_H__
#define __DRAGON_GUILDMGR_H__

#include "dragonguild.h"

class DragonGuildSimpleMgr
{
public:
	DragonGuildSimpleMgr();
	~DragonGuildSimpleMgr();
	DECLARE_SINGLETON(DragonGuildSimpleMgr)

	bool Init();
	void Uninit();


public:
	void DeleteDragonGuildSimple(UINT64 guildId);
	CDragonGuildSimple& AddDragonGuildSimple(UINT64 guildId) { return m_dragonGuildSimpleMap[guildId]; }
	CDragonGuildSimple* GetDragonGuildSimple(UINT64 guildId);
	CDragonGuildSimple* GetDragonGuildByRoleId(UINT64 roleId) { return GetDragonGuildSimple(GetDragonGuildId(roleId)); }


	void DoAddDragonGuild(UINT64 roleId, UINT64 guildId);
	void DoLeaveDragonGuild(UINT64 roleId);
	UINT64 GetDragonGuildId(UINT64 roleId) { return m_roleDragonGuildMap.find(roleId) == m_roleDragonGuildMap.end() ? 0 : m_roleDragonGuildMap[roleId]; }
	std::string GetDragonGuildName(UINT64 guildId);
	std::unordered_map<UINT64, CDragonGuildSimple>& GetAllGuild() { return m_dragonGuildSimpleMap; }

	void SetDragonGuildAttr(UINT64 guildid, UINT32 count);
	UINT32 GetDragonGuildAttr(UINT64 guildid);
	void ClearDragonGuildAttr() { m_dragonGuild2attrcount.clear(); }
	bool IsInSameDragonGuild(UINT64 roleId1, UINT64 roleId2);

private:
	std::unordered_map<UINT64, UINT64> m_roleDragonGuildMap;
	std::unordered_map<UINT64, CDragonGuildSimple> m_dragonGuildSimpleMap;

	std::map<UINT64, UINT32> m_dragonGuild2attrcount;
};

#endif