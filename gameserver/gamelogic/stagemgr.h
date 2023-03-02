#ifndef __STAGEMGR_H__
#define __STAGEMGR_H__

#include "unit/roleserilizeinterface.h"
#include "cvsreader.h"
#include "dragonexpbase.h"
#include "tablemgr/backflowtablemgr.h"

#define INVALID_STAGERANK (-1)

struct BRRoleState
{
	BRRoleState()
	{
		confid = 0;
		brbid1 = 0;
		brbid2 = 0;
		rank = 0;
	}
	void DayZero()
	{
		confid = 0;
		brbid1 = 0;
		brbid2 = 0;
		rank = 0;
		reward.clear();
	}
	int confid;
	int brbid1;
	int brbid2;
	int rank;
	std::vector<Sequence<UINT32,2>> reward;//只做暂存用
};


struct StageCountInfo
{
	UINT32 count;
	UINT32 buycount;
};


struct TrophyTypeDetail
{
	UINT32 trophy_order;
	UINT32 trophy_time;
	TrophyTypeDetail()
	{
		trophy_order = 0;
		trophy_time = 0;
	}
};
struct StageTrophyDataDetail
{
	UINT32 scene_id;
	UINT32 pass_count;
	UINT32 quickly_pass_time;
	UINT64 hight_damage;
	UINT64 hight_treat;
	UINT32 help_count;
	UINT32 no_deathpass_count;
	std::map<UINT32,std::vector<TrophyTypeDetail>> trophy_detail;
	StageTrophyDataDetail()
	{
		scene_id = 0;
		pass_count = 0;
		quickly_pass_time = 0;
		hight_damage = 0;
		hight_treat = 0;
		help_count = 0;
		no_deathpass_count = 0;
		trophy_detail.clear();
	}
};
struct StageTrophyInfo
{
	UINT64 total_score;
	UINT32 honour_rank;
	std::vector<StageTrophyDataDetail> trophy_data;
	StageTrophyInfo()
	{
		total_score = 0;
		honour_rank = 1;
		trophy_data.clear();
	}
};

typedef void (*SceneCountCallback)(void *ptr, UINT32 groupid, UINT32 count, UINT32 buycount);
typedef void (*SceneCoolDownCallback)(void *ptr, UINT32 groupid, UINT32 cooldown);

struct SceneInfo;
class Role;
class StageMgr : public RoleSerializeInterface
{
public:
	StageMgr(Role* role);
	~StageMgr();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);
	void FillStageInfo(KKSG::StageInfo* pInfo);
	void UpdateStageInfo2Client();

	void Update();

	void OnEnterScene(Scene* scene);
	bool CanPlayCount(SceneInfo *pSceneInfo);
	bool CanPlayCount(UINT32 nScenID);
	UINT32 CanPlayCoolDown(SceneInfo *pSceneInfo);
	bool ResetPlayCount(UINT32 groupid);
	UINT32 GetBuyCount(UINT32 groupid);
	void AddScenePlayCount(SceneInfo *pSceneInfo);
	void AddScenePlayCount(UINT32 sceneID);
	void AddSceneBuyCount(SceneInfo *pSceneInfo, UINT32 count = 1);
	UINT32 GetCanPlayCount(SceneInfo *pSceneInfo);
	UINT32 GetCanPlayCount(UINT32 nSceneID);
	UINT32 GetPlayedCount(SceneInfo *pSceneInfo);
	UINT32 GetPlayedCount(UINT32 sceneID);
	UINT32 GetSceneBuyCount(SceneInfo *pSceneInfo);
	void AddSceneCoolDown(SceneInfo *pSceneInfo);
	UINT32 GetSceneCoolDown(SceneInfo *pSceneInfo);
	void ClearSceneCoolDown(SceneInfo *pSceneInfo);
	void ClearSceneCoolDown();
	void SetBossHpProgress(UINT32 sceneID, UINT32 bossid, int bosshppercent);
	int GetBossHpProgress(UINT32 sceneID, UINT32 bossid);
	int GetBossAVGHpProgress(UINT32 sceneID);
	void ClearProgress(UINT32 sceneID = 0);
	void SetRank(UINT32 sceneID, int rank);
	void SetAbyss(UINT32 nType,UINT32 nDiff);
	void SetAbyssMailTime(UINT32 nTime);
	UINT32 GetAbyssMailTime();
	void SendAbyssInfo();
	void OnFinishAbyss(UINT32 AbyssID);
	int  GetRank(UINT32 sceneID);
	bool IsPassed(UINT32 sceneID);
	UINT32 GetStageCount(UINT32 nGroupId);
	void ForeachSceneCount(SceneCountCallback callback, void *ptr, std::set<UINT32>& groupids);
	void ForeachSceneCoolDown(SceneCoolDownCallback callback, void *ptr, std::set<UINT32>& groupids);
	bool IsFetchedChapterChest(int chapter, int chestID);
	void SetFetchedChapterChest(int chapter, int chestID);
	bool IsSceneChestOpened(UINT32 sceneID);
	void SetSceneChestOpened(UINT32 sceneID);
	const std::vector<UINT32>  &GetChestOpenedScene() { return m_ChestOpenedScene; }

	//BossRush
	void AddBRJoinCount();
	int GetBRJoinCount();
	void AddBRRefreshCount();
	int GetBRRefreshCount(bool self = false);
	void ResetBRRefreshCount();
	bool AddStageAssist(UINT32 stageid, int point);
	int GetStageAssistPoint();
	std::string DumpStageAssistPoint(); 
	void ClearBRRoleState();
	BRRoleState* GetBRRoleState();
	void SetModified(){ m_IsModified.Set(); }
	inline UINT32 GetBossRushMax() { return m_bossrushmax; }
	inline void SetBossRushMax(UINT32 max) { m_bossrushmax = max; m_IsModified.Set(); }
	void AddHelperWinCount();
	void HandleHelperWeekWin(int helperWinCount);
	void TestSetStageDay(){m_StageDay = 0;}
	void TestClear();
	void DebugResetStageRank(UINT32 SceneID);

	int HolidayCheck(UINT32 sceneId);
	void DoAfterHolidayEnd(UINT32 sceneId);
	UINT32 GetHolidayTimes(UINT32 holidayId);

	UINT32 GetAbyssDiff(UINT32 nType);

	//时间范围6-05号，如果有活动要用可以复用。复用需要先reset一下。
	void SetKidHelperCount(const UINT32 nCount)
	{
		m_nKidHelperCount = nCount; 
		m_IsModified.Set();
	}
	const UINT32 GetKidHelperCount()
	{
		return m_nKidHelperCount;
	}

	void IncKidHelperCount()
	{
		m_nKidHelperCount++;
		m_IsModified.Set();
	}
	void ResetKidHelperCount() 
	{
		m_nKidHelperCount = 0;
		m_IsModified.Set();
	}
	//塔姐福利
	void AddTarjaTime();
	void SyncTarjaMs();
	UINT32 GetTarjaTime()
	{
		return m_nTarjaTime;
	}


	void SetTarjaAward(UINT32 nCount)
	{
		m_nTarjaAward = nCount;
		m_IsModified.Set();
	}

	UINT32 GetTarjaAward()
	{
		return m_nTarjaAward;
	}

	UINT32 GetChapterStarNum(UINT32 chapterId);

	void SetTrophy(Scene *scene);
	void GetTrophyData(KKSG::PersonalCareerRes &roRes);
	void OnAddDne(UINT32 nDneID);
    UINT32 GetDneCount(UINT32 nDneID);

	bool GetDeSpecialFlag() { return m_deSpecialFlag; }
	void SetDeSpecialFlag(bool flag) { m_deSpecialFlag = flag ; m_IsModified.Set(); }

	void InfoPrint();

	void FinishBackFlowStage(BackFlowServerConfig::RowData* pData);

public:
	void GetTrophyOrderBySceneID(const UINT32 scene_id,UINT32& trophy_id,UINT32& trophy_order);
	void DebugTest(UINT32 trophy_id,UINT32 trophy_order,UINT32 score);
private:
	void GiveTrophyReward(const UINT32 scene_id);

	void DayZero();
	void WeekZero();
	void CheckSpecial();

private:
	Switch m_IsModified;
	Role*  m_pRole;
	int    m_StageDay;
	time_t m_lastWeekUpTime;	
	bool m_deSpecialFlag;


	int m_brupday;
	UINT32 m_bossrushmax;
	UINT32 m_BRJoinCounttoday;   // 今日参与次数
	UINT32 m_BRRefreshCountToday;// 今天使用刷新次数
	UINT32 m_HolidayId;          // 节日活动id
	UINT32 m_HolidayTimes;       // 节日活动次数
	BRRoleState m_BRState;

	UINT32 m_nAbyssID;

	int m_helperWinCount;//助战胜利次数
	int m_helperWeekWinCount;//助战周胜利次数

	UINT32 m_nKidHelperCount;

	std::map<UINT32,UINT32> m_mapAbyss;
	UINT32  m_nAbyssMailTime;

	UINT32 m_nTarjaAward;
	UINT32 m_nTarjaTime;
	std::vector<UINT32> m_ChapterChest;
	std::vector<UINT32> m_ChestOpenedScene;
	//需统一 *****
	std::unordered_map<UINT32, int> m_stageassist;//次核心辅助系统
	std::unordered_map<UINT32, int> m_StagesRank;//通过星级 (有些只有没通过和3星两种状态)
	std::unordered_map<UINT32, StageCountInfo> m_StagesCount;//参加次数 按场景的Group计算
	std::unordered_map<UINT32, time_t> m_StagesCoolDown;//冷却 按成就group计算
	std::unordered_map<UINT32, CDEStageProgress> m_DEStageProgress;// 龙之远征进度记录
	std::unordered_map<UINT32,UINT32> m_mapDne;

	StageTrophyInfo trophy_;
};

#endif // __STAGEMGR_H__