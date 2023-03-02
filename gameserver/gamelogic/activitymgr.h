#ifndef __ACTIVITY_H__
#define __ACTIVITY_H__

#include "table/ActivityTable.h"
#include "table/ActivityChestTable.h"
#include "unit/role.h"
#include "scene/scene.h"

enum EActivityId
{
	eActivityStage = 110,
	eActivitySpecialStage = 111,
	eActivitySkillUp = 13,

	eActivityBuyGold = 566,
	eActivityBuyFatigue = 63,
	eActivityShopBuy = 32,
	eActivityAuction = 22,

	eActivitySlotAttr = 123,
	eActivityJade = 122,
	eActivityEnhance = 121,
	eActivityEnhanceFashion = 160,
	eActivitySmelting = 124,

	eActivityArena = 51,
	eActivityPk = 54,
	
	eActivityCompose = 45,
	eActivityDecompose = 180,

	eActivityBossRush = 48,
	eActivityWorldBoss = 524,
	eActivityGuildBoss = 85,

	eActivityExpedtion = 525,
	eActivityDragon = 526,
	eActivityTower = 527,
	eActivityGoddess = 529,
	eActivityDragonExp = 50,
	eActivityEndLessAbyss = 540,
	
	eActivityNest = 520,
	eActivityGuildStage = 84,

	eActivityFlower = 72,
	eActivityCard = 820,
	eActivityGuildCheckin = 810,

	eActivityQAMulti = 551,
	eActivityQAGuild = 821,

	eActivityBuyAileen = 562,	
	
	eActivityMallBuy = 651,
	
	eActivityDailyTask = 886,

	eActivityGuildInherit = 890,

	eActivityPayMember = 568,

	eActivityHeroBattle = 956,
};

class Role;
class CActivityRecord;
struct ItemDesc;
class CActivityMgr
{
	CActivityMgr();
	~CActivityMgr();
	DECLARE_SINGLETON(CActivityMgr);
public:
	bool Init();
	void Uninit();

	void Reset();
	bool LoadFile();
	bool Reload();

	bool OnTimer();

	//void SendActivityInfo(Role* pRole, bool bIsPackAll = false);
	void UpdateRecord(CActivityRecord* pRecord, Role* pRole); //没有修改调用这个

	void UpdateActiviyId(CActivityRecord* pRecord, Role* pRole); //可能会修改就调用这个

	void OnFinishActivityId(CActivityRecord* pRecord, Role* pRole, UINT32 dwActivityId,bool israndom = false);
	//bool IsFinishActivity(CActivityReocrd* pRecord, UINT32 dwActivityId);

	//UINT32 GetChestIndex(CActivityReocrd* pRecord, UINT32 dwIndex);
	bool GetChestItem(UINT32 dwAllValue, UINT32 dwLevel, UINT32 dwIndex, std::vector<ItemDesc>& vecItemDesc);

	UINT32 GetConfigFinishCount(UINT32 dwActivityId);

	UINT32 GetIndexID(UINT32 systemid);

	void HintNotify(Role* pRole);

	void OnRoleFirstEnterScene(Role* pRole);

	void CheckFinishPayMember(Role* pRole);

	std::vector<ItemDesc> GetActivityCoin(UINT32 activityid);	

	bool CheckPlayerGuileRelation(Role* role,Scene* scene);
	UINT32 ChangeActivityID(UINT32 old_activity_id,UINT32 random = 0);

	bool CheckRandomActivityID(UINT32 activity_id);

	UINT32 TestAddVaule(UINT32 value);
public:
	static UINT32 activityChestNum;

private:
	std::vector<UINT32> GetChestDropId(UINT32 dwIndex, UINT32 dwLevel, UINT32 dwAllValue);	

	void Test();

private:
	//UINT32 m_dwActivityValueScale;
	UINT32 m_handler;

	ActivityTable m_oTable;
	ActivityChestTable m_oChestTable;
	std::map<UINT32, UINT32> m_systemid2index;
};

#endif