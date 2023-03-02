#ifndef __CGUILDMGR_H__
#define __CGUILDMGR_H__

#include "guild/guilddef.h"
#include "guildlistener.h"
#include "guildsorter.h"
#include "table/GuildSkill.h"
#include "table/SkillList.h"

namespace KKSG
{
	class GuildAllDBData;
	class GStoreBrief;
}

typedef bool (*GuildFun)(Guild *, void *arg);

struct GuildDoor
{
	UINT32 nNextTime;
	UINT32 nCount;
};

class CRole;
class CGuildMgr : public IGuildListener
{
	CGuildMgr();
	~CGuildMgr();
	DECLARE_SINGLETON(CGuildMgr)

public:
	void DoRoleLogin(CRole* pRole);   // 玩家上线处理
	UINT32 CanCreateGuild(CRole* pRole, std::string name, UINT32 icon);

public:
	bool Init();
	void Uninit();
	bool OnTimer(); // 定时接口
	void DoOnlineCheck(UINT64 roleId);  // 上线检查
	void InitGuildTimeBonus();//所有公会的定时红包

	const std::unordered_map<UINT64, Guild*>& GetGuilds() { return m_guilds; }
	UINT64 GetGuildIdByRoleId(UINT64 roleid);
	Guild* GetGuild(UINT64 uid);
	UINT64 MakeGuildID();
	void   AddGuild(Guild *pGuild);
	Guild* GetGuildByRoleId(UINT64 roleId);
	UINT64 GetRoleGuild(UINT64 roleId);
	const std::string& GetName(UINT64 guildid);
	bool IsSameGuild(UINT64 roleID11, UINT64 roleID22);
	void AddDismissGuild(Guild *pGuild);
	void LoadDBGuild(const KKSG::GuildAllDBData &guilddata);
	void SaveToDB();
	void AddRoleContribute(UINT64 qwRoleID, int contribute);
	int  CreateGuild(CRole* pRole, const std::string& guildName, int icon);
	int  JoinGuild(UINT64 roleId, UINT32 attr, UINT64 guildId);
	

	void Update();
	void RemoveGuildName(const std::string &name);
	void SetLoaded() { m_bIsLoaded = true; }
	GuildSorter &GetSorter() { return m_sorter; }
	virtual void OnAddMember(Guild *pGuild, GuildMember *pMember);
	virtual void OnAddMemberFromDB(Guild *pGuild, GuildMember *pMember);
	virtual void OnRemoveMember(Guild *pGuild, UINT64 qwRoleID);
	virtual void OnMemberPosChanged(Guild *pGuild, GuildMember *pMember);
	virtual void OnMembergGuildBossDps(Guild *pGuild, GuildMember *pMember){}
	virtual void OnGuildLevelUp(Guild *pGuild);
	virtual void OnGuildAddExp(Guild* pGuild);
	virtual void OnGuildAddPrestige(Guild* pGuild);

	void HintNotify(CRole* role);
	inline bool IsLoaded() { return m_bIsLoaded; }
	void DoGuildDareReward(const std::set<UINT64>& roleSet, std::string sceneName, std::map<UINT64, UINT32> guildexp);
	SkillList::RowData* GetSkillConfig(UINT32 skillHash, int skillLevel);
	void BillingActivity(); // 公会活跃度结算
	Guild* GetGuildByName(const std::string &name);
	UINT32 GetRankNum(UINT32 rankType, UINT64 guildId);
	void RemoveGuildFromSort(UINT64 guildId);
	void StartUpGuildmgrEvent();
	void DoRankLog(Guild* pGuild, UINT32 rank);

	void StressTest(UINT32 nMaxPlayer);
	bool IsNameNotUsed(const std::string &name)
	{ 
		return m_mapGuildNames.find(name) == m_mapGuildNames.end();
	}
	bool ModifyName(Guild *pGuild,const std::string &oldName,const std::string &newName);
	bool IsLockName(const std::string &name) 
	{
		auto iter=m_LockName.find(name);
		if(iter!=m_LockName.end())
		{
			return true;
		}
		return false;
	}
	void AddLockName(const std::string &name)
	{
		m_LockName.insert(name);
	}

	void UnlockName(const std::string &name)
	{
		auto iter = m_LockName.find(name);
		if(iter!=m_LockName.end())
		{
			m_LockName.erase(iter);
		}
	}

	void AddJoinInfo(UINT64 nRoleID);
	void ClearJoinCount();
	bool CheckJoin(UINT64 nRoleID);
private:
	Guild *FindQuickJoinGuild(double ppt);

	void OnGuildCreated(Guild* pGuild);
	UINT32 GetSaveMode() { return (++m_saveMode) % 10; }

private:
	bool m_bIsLoaded;
	UINT32 m_saveMode; // 存储取模id
	time_t m_LastSaveGuildTime;
	std::unordered_map<UINT64, UINT64> m_roleid2guildid;
	std::unordered_map<UINT64, Guild*> m_guilds;
	std::unordered_map<std::string,Guild*> m_mapGuildNames;
	std::list<Guild *> m_DismissedGuild;
	GuildSorter m_sorter;
	SkillList m_SkillTable;
	std::unordered_map<UINT32, std::vector<SkillList::RowData*> > m_SkillIndex;
	std::map<UINT32, UINT32> m_allGuildTimeBonus;
	std::set<std::string> m_LockName;
	std::unordered_map<UINT64, GuildDoor> m_mapGuildDoor;
};

#endif // __GUILDMGR_H__