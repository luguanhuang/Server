#ifndef __WORLDBOSSMGR_H__
#define __WORLDBOSSMGR_H__

#include "table/MultActivityMgr.h"
#include "table/BossTable.h"
#include "table/WorldBossRewardTable.h"
#include "table/WorldBossGuildRewardTable.h"
#include "role/rolesummary.h"

enum EWorldBossSceneState
{
	eBeginPre	= 0,
	eBegin		= 1,
	eGoing		= 2,
	eWaitEnd	= 3,
	eEnd		= 4,

	eDummy,
};

struct BossInfo
{
	UINT32 dwId;
	UINT32 dwEnemyId;
	float fAttackPercent;
	float fLifePercent;
	float currenthp;
	float maxhp;

	float percent;

	BossInfo()
	{
		dwId = 0;
		dwEnemyId = 0;
		fAttackPercent = 1.0f;
		fLifePercent = 1.0f;
		currenthp = 0.0f;
		maxhp = 0.0f;
		percent = 1.0f;
	}
};

struct GuildAttr
{
	UINT32 count;
	std::set<UINT32> m_delayrpcs;

	bool isempty() { return (0 == count) && m_delayrpcs.empty(); }
	GuildAttr()
	{
		count = 0;
	}
};

class CRole;
struct ItemDesc;
class WorldBossMgr : public ITimer 
{
	WorldBossMgr();
	~WorldBossMgr();
	DECLARE_SINGLETON(WorldBossMgr)
public:
	bool Init();
	void Uninit();

	void ClearFile();
	bool LoadFile();
	bool CheckFile();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

public:
	UINT32 CanEnter(UINT64 roleid);

	void DesRole(UINT64 roleid);

	bool IsSceneOpen() { return eGoing == m_eState; }
	bool IsSceneEnd() { return eDummy == m_eState; }
	bool IsBossDead() { return m_isdead; }

	///> 刷新世界boss
	void InitBoss(UINT32 id, UINT32 enemyid);
	void SubHp(float hp);
	void OnDied() { m_isdead = true; }
	BossInfo* GetBossInfo() { return &m_stBossInfo; }	

	UINT32 GetGuildReward(UINT32 rank);

	time_t GetBeginTime(time_t dwNowTime);
	time_t GetEndTime(time_t dwNowTime);
	UINT32 GetBeginTimeLeft();
	UINT32 GetEndTimeLeft();
	UINT32 GetRoleEndTimeLeft(UINT64 roleid);

	void HintNotify(CRole* role);
	void ErrorNotify(UINT64 roleid, KKSG::ErrorCode errorcode);

	void ResetAll(UINT32 beginhour, UINT32 beginmin, UINT32 endhour, UINT32 endmin);

	void OnRoleLeaveGuild(UINT64 guildid, UINT64 roleid);

	GuildAttr& GetGuildAttr(UINT64 guildid);

private:
	///> boss刷新时更新DB的boss信息
	void SendBossInfoToDb();
	UINT32 RandomBoss(std::vector<Sequence<UINT32, 2>>& bosses);

	///> 场景开启和结束
	void OnSceneTimeBegin();
	void OnSceneTimeWaitEnd();
	void OnSceneTimeEnd();

	void NoticeState(bool startorend);

	void SyncGS(UINT32 state);

	void NoticeRank(UINT32 rank, UINT64 roleid);
	void NoticeGuild();
	void NoticePercent(float lastper, float per);
	bool GetReward(UINT32 level, UINT32 rank, std::vector<ItemDesc>& items);
	void GiveRankReward();	
	void GiveAuctReward();	
	void DoTxLog(UINT32 rank, UINT64 roleid, float damge, CRoleSummary* data);

private:
	bool m_isdead;
	BossInfo m_stBossInfo;
	time_t m_dwWaitEndTime;

	UINT32 m_noticetime;
	BossTable m_oBossTable;

	HTIMER m_TimeHandler;
	EWorldBossSceneState m_eState;

	WorldBossGuildRewardTable m_guildreward;

	WorldBossRewardTable m_rewardtable;
	std::map<UINT32, std::vector<WorldBossRewardTable::RowData*>> m_leve2reward;

	std::map<UINT64, UINT32> m_role2time;
	std::map<UINT64, UINT32> m_role2count;

	std::map<UINT64, GuildAttr> m_guild2attrcount;
};

#endif // __WORLDBOSSMGR_H__