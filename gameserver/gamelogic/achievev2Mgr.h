#ifndef __ACHIEVE_V2_MGR_H__
#define __ACHIEVE_V2_MGR_H__

#include "table/AchievementV2Table.h"
#include "table/EquipSuitTable.h"
#include "table/AchievementPointRewardTable.h"
#include "table/FashionSuitTable.h"
#include "item.h"

struct ItemDesc;
class Role;
struct StageRankInfo;
class AchieveV2Mgr
{
	AchieveV2Mgr();
	~AchieveV2Mgr();
	DECLARE_SINGLETON(AchieveV2Mgr);
public:
	bool Init();
	void Uninit();
	bool LoadFile();
	void ClearFile();
	bool CheckFile();

	///>获取table
	AchievementV2Table* GetTable();

	///>获取奖励table
	AchievementPointRewardTable* GetRewardTable();

	///>系统红点提示
	void HintNotify(Role* role);

	///>获得新的成就推送
	void SendReachAchieveNotify(Role* pRole, UINT32 achieveID);

	///>刷新获得成就
	void UpdateAchieve(Role* pRole, int achieveId);

	///>GM指令获得称号
	void OnAddGetAchievement(Role* pRole, UINT32 aid);

	///>根据套装编号获取套装数量：该编号的套装有几套
	UINT32 GetSuitCountBySuitID(Role* pRole, UINT32 suitId);

	///>指定时装ID, 获取该时装套数
	UINT32 GetFashionCountBySuitID(Role* pRole, UINT32 suitId);

	///>获取套装数量
	//UINT32 GetSuitCount(Role* pRole);

	///>获取首饰套装数量
	UINT32 GetJewelrySuitCount(Role* pRole);

	///>获取装备套装数量
	UINT32 GetEquipSuitCount(Role* pRole);

	///>获取某个装备套装中不同装备的数量：相同装备ID的装备数量不重复计算数量
	UINT32 GetDiffEquipCountInSuit( Role* pRole, UINT32 equipSuitID );

	/////>获取物品所在套装的所有装备
	//bool GetCurEquipAllSuitEquip(UINT32 itemId, std::vector<int>& vecEquip);

	///>获取当前装备所在的套装的强化等级
	UINT32 GetEquipSuitLevel(Role* pRole, UINT64 equipUid, UINT32 needLevel,UINT32 needCount);

	///>获取时装数量：整套时装数量
	UINT32 GetFashionCount(Role* pRole);

	///>获取某件时装所在套装的品质：如果没有完整的套装，则返回false
	bool GetFashionQuality(Role* pRole, UINT32 fashionId, UINT32& quality);


	///>外部调用函数：完成条件判断
	void OnJoinGuild(Role* pRole);
	void OnHaveSuitCount(Role* pRole, UINT32 itemID );
	void OnJewelrySuitCount( Role* pRole, UINT32 itemID );
	void OnEquipLevel(Role* pRole, UINT32 level);
	void OnSuitLevel(Role* pRole, UINT64 equipUid, UINT32 level);
	void OnJewlerySuitLevel(Role* pRole, UINT64 equipUid, UINT32 level);
	void OnChangeJob(Role* pRole);
	void OnLevelUp(Role* pRole);
	void OnDelGetAchievement(Role* pRole, UINT32 aid);
	void OnFinishChapter(Role* pRole, UINT32 sceneId);
	void OnLimitTimeFinishRaid(Role* pRole, UINT32 sceneId, UINT32 timespan);
	void OnStarFinishRaid(Role* pRole, UINT32 sceneId, UINT32 star);
	void OnNoHurtFinishRaid(Role* pRole, UINT32 sceneId, UINT32 hpPercent);
	void OnOneStarFinishRaid(Role* pRole, UINT32 sceneId);

	void OnGiveFlower(Role* pRole, int num);

	///>完整时装套数
	void OnFashionCount(Role* pRole);
	void OnFashionQuality(Role* pRole, UINT32 fashionId);
	void OnNoDieFinishRaid(Role* pRole, UINT32 sceneId);
	void OnAloneFinishRaid(Role* pRole, UINT32 sceneId);
	void On3SFinishRaid(Role* pRole, UINT32 sceneId, UINT32 star);
	void OnFourNoDieFinishRaid(Role* pRole, UINT32 sceneId);
	void OnPKWinTimes(Role* pRole,UINT32 nWin);
	void OnPKScore(Role* pRole,UINT32 nPoint);
	void OnArenaRank(Role* pRole, UINT32 rank);
	void OnTowerFloor(Role* pRole,UINT32 nFloor);
	void OnTotalLogin(Role* pRole);
	void OnRechargeAmount(Role* pRole, UINT32 totalPay);
	void OnFirstRecharge(Role* pRole);

	void OnAssistFight(Role *pRole,UINT32 nCount);

	void OnSceneFinished(Role* pRole, UINT32 nSceneTemplateID, const StageRankInfo& stageinfo);

	void OnPassDragonScene(Role* pRole, UINT32 expID, UINT32 roleSize, UINT32 costTime);

	void OnMentor(Role* pRole, UINT32 nStudent);
	void OnMarry(Role* pRole);
	// garden achieve 
	void OnGarden(Role* pRole, UINT32 index);
	void DoEvent(Role *pRole,const KKSG::DesignationEvent *pEvent);
private:
	AchievementV2Table			m_achieveTable;
	AchievementPointRewardTable	m_rewardTable;

//优化代码
private:
	std::map<UINT32, std::vector<AchievementV2Table::RowData *> > m_mapConfig;
public:
	void LoadData();
	std::vector<AchievementV2Table::RowData *>* GetConfigType(UINT32 nType);
};

#endif