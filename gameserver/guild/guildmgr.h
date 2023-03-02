#ifndef __GUILDMGR_H__
#define __GUILDMGR_H__

#include "guild.h"

class GuildSimpleMgr
{
public:
	GuildSimpleMgr();
	~GuildSimpleMgr();
	DECLARE_SINGLETON(GuildSimpleMgr)

	bool Init();
	void Uninit();


public:
	void DeleteGuildSimple(UINT64 guildId);
	CGuildSimple& AddGuildSimple(UINT64 guildId) { return m_guildSimpleMap[guildId]; }
	CGuildSimple* GetGuildSimple(UINT64 guildId);
	CGuildSimple* GetGuildByRoleId(UINT64 roleId) { return GetGuildSimple(GetGuildId(roleId)); }


	void DoAddGuild(UINT64 roleId, UINT64 guildId);
	void DoLeaveGuild(UINT64 roleId);
	UINT64 GetGuildId(UINT64 roleId) { return m_roleGuildMap.find(roleId) == m_roleGuildMap.end() ? 0 : m_roleGuildMap[roleId]; }
	std::string GetGuildName(UINT64 guildId);
	std::unordered_map<UINT64, CGuildSimple>& GetAllGuild() { return m_guildSimpleMap; }

	void SetGuildAttr(UINT64 guildid, UINT32 count);
	UINT32 GetGuildAttr(UINT64 guildid);
	void ClearGuildAttr() { m_guild2attrcount.clear(); }
	
private:
	std::unordered_map<UINT64, UINT64> m_roleGuildMap;
	std::unordered_map<UINT64, CGuildSimple> m_guildSimpleMap;

	std::map<UINT64, UINT32> m_guild2attrcount;
};

//#include "guildlistener.h"
//#include "guildsorter.h"
//
//namespace KKSG
//{
//	class GuildAllDBData;
//}
//
//class Role;
//
//typedef bool (*GuildFun)(Guild *, void *arg);
//
//class GuildMgr : public IGuildListener
//{
//	GuildMgr();
//	~GuildMgr();
//	DECLARE_SINGLETON(GuildMgr)
//
//public:
//
//	bool Init();
//	void Uninit();
//
//	UINT64 MakeGuildID();
//	void   AddGuild(Guild *pGuild);
//	Guild *FindGuild(UINT64 gid);
//	Guild *FindGuildByRoleId(UINT64 roleid);
//
//	void QueryDb();
//	void AddDismissGuild(Guild *pGuild);
//	void LoadDBGuild(const KKSG::GuildAllDBData &guilddata);
//	void AddRoleContribute(UINT64 qwRoleID, int contribute);
//	int  CreateGuild(Role* pRole, const std::string& guildName, int icon);
//	int  JoinGuild(Role* pRole, UINT64 gid);
//	
//	const std::unordered_map<UINT64, Guild*>& GetGuilds() { return m_guilds; }
//
//	void Update();
//	void SetLoaded() { m_bIsLoaded = true; }
//	
//	GuildSorter &GetSorter() { return m_sorter; }
//	UINT64 GetRoleGuild(UINT64 qwRoleID);
//
//	virtual void OnAddMember(Guild *pGuild, GuildMember *pMember);
//	virtual void OnAddMemberFromDB(Guild *pGuild, GuildMember *pMember);
//	virtual void OnRemoveMember(Guild *pGuild, UINT64 qwRoleID);
//	virtual void OnMemberPosChanged(Guild *pGuild, GuildMember *pMember);
//	virtual void OnMembergGuildBossDps(Guild *pGuild, GuildMember *pMember){}
//	virtual void OnGuildLevelUp(Guild *pGuild);
//	virtual void OnGuildAddExp();
//
//	void HintNotify(Role* role);
//	void DailyClear();  // 零点清零
//	inline bool IsLoaded() { return m_bIsLoaded; }
//	void DoGuildDareReward(const std::set<UINT64>& roleSet, std::string sceneName);
//	void BillingActivity(); // 公会活跃度结算
//
//private:
//	Guild *FindQuickJoinGuild(double ppt);
//	bool IsNameNotUsed(const std::string &name) { return m_usedGuildNames.find(name) == m_usedGuildNames.end();}
//
//private:
//	bool m_bIsLoaded;
//	time_t m_LastSaveGuildTime;
//	std::unordered_map<UINT64, UINT64> m_roleid2guildid;
//	std::unordered_map<UINT64, Guild*> m_guilds;
//	std::set<std::string> m_usedGuildNames;
//	std::list<Guild *> m_DismissedGuild;
//	GuildSorter m_sorter;
//};

#endif // __GUILDMGR_H__