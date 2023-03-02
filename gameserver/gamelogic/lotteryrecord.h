#ifndef __LOTTERYRECORD_H__
#define __LOTTERYRECORD_H__

#include "time.h"
#include "unit/roleserilizeinterface.h"
#include "pandoraconfig.h"
#include "util/XRandom.h"

enum DropIDType
{
	DROP_ITEMID,
	DROP_DROPID,
};

struct DropResultInfo
{
	DropIDType type;
	UINT32 id; //itemid
	UINT32 count;
};

struct ItemResultInfo
{
	ItemResultInfo()
	{
		id = 0;
		count = 0;
	}
	UINT32 id;
	UINT32 count;
};

struct PandoraDropTimes
{
	PandoraDropTimes();
	PandoraDropTimes(const KKSG::PandoraDrop& pandoraDrop, const PandoraDropConf& conf);
	
	void Init(const PandoraDropConf& conf);
	
	void CheckInit(const PandoraDropConf& conf);

	UINT32 betterDropTimes;
	std::vector<UINT32> bestDropTimes;
	UINT32 nextBetterDropTimes;
	std::vector<UINT32> nextBestDropTimes;
};

class Role;
struct ItemDesc;

class LotteryRecord : public RoleSerializeInterface
{
public:
	LotteryRecord(Role *pRole);
	~LotteryRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);

	void UpdatePerSecond();

	void OnSystemOpened(UINT32 type);

	void UpdateNextFreeBuyTime();

	bool   DragonCoinBuyCheckFree();
	UINT32 GetDragonCoinFreeBuyCoolDown();

	const ItemDesc* GetDragonCoinOneItemID();
	const ItemDesc* GetDragonCoinTenItemID();

	bool   GoldBuyCheckFree();
	UINT32 GetGoldFreeBuyCoolDown();
	UINT32 GetGoldFreeBuyCount();

	void ClearCoolDown();

	void HintNotify();

	///////////////////////////////////////////////////////////////
	const ItemDesc* GetGoldOneDrawItemID();
	const ItemDesc* GetGoldTenDrawItemID();

	UINT32 GetGoldMinimumRewardCount() { return m_Gold_10_MinimumRewardCount; }
	UINT32 GetDragonCoinMinimumRewardCount() { return m_DragonCoin_10_MinimumRewardCount; }

	void ClearLastLotteryItem();
	void AddLotteryItem(UINT32 itemid);
	bool IsHaveAndDelete(UINT32 itemid);

	///> 摇钱树功能
	void UpdateGoldClick(time_t nowtime);
	void UpdateGoldClickDay();
	void HintNotifyGoldClick();

	bool CountLimit(bool isfree);

	bool CanFree();
	void UpdateGoldClickNextFreeTime();
	void SetNextFreeTime();
	void AddCount(bool isfree);

	inline UINT32 GetFreeCount() { return m_GoldClickFreeCount; }
	inline UINT32 GetCount() { return m_GoldClickCostCount; }
	UINT32 GetAllFreeCount();
	UINT32 GetAllCount();
	UINT32 GetFreeTimeLeft();

	void ResetCount() { m_GoldClickCostCount = 0; m_GoldClickFreeCount = 0; m_isModify.Set(); }
	void ResetCD() { m_GoldClickFreeTime = (UINT32)time(NULL) + 5; m_isModify.Set(); }

	///> 潘多拉掉落
	const ItemDesc* GetPandoraDropItem(UINT32 pandoraDropID);
	void GetPandoraDropItem(UINT32 pandoraDropID, UINT32 openTimes, std::vector<const ItemDesc*>& vecItems);

	bool IsHaveShareGiftCount();
	void AddShareGiftCount();
	void UpdateShareGiftCount();

	void GMClearShareGiftCount();

private:
	void _CheckPandoraDropTime(const PandoraDropConf* conf, UINT32 dropQuality, UINT32 itemQuality, PandoraDropTimes& dropTimes);

private:

	// 变量命名方式 m_[抽奖消耗]_[抽奖次数]_[后缀]
	// 比如  m_DragonCoin_1_FreeDrawTime
	//           龙币   1连抽    后缀
	Role* m_pRole;

	Switch m_isModify;

	UINT32 m_DragonCoin_1_DrawCount;            // 龙币蛋抽奖总次数
	UINT32 m_DragonCoin_10_MinimumRewardCount;  // 龙币蛋十连抽的低保
	
	UINT32 m_Gold_1_DrawCount;					// 金币蛋抽奖总次数
	UINT32 m_Gold_10_MinimumRewardCount;		// 金币蛋十连抽的低保

	////////////////////////////////////////////////////////////////
	
	UINT32 m_DragonCoin_1_FreeLastTime;		// 上次龙币免费购买的时间
	UINT32 m_nextFreeBuyTime;				// 下次龙币免费购买时间

	UINT32 m_Gold_1_FreeLastTime;			// 上次金币免费购买的时间
	UINT32 m_nextFreeGoldBuyTime;			// 下次金币购买免费时间
	UINT32 m_Gold_1_FreeCount;				// 今天已经用了几次免费金币购买
	UINT32 m_Gold_1_FreeDay;				// 免费金币购买的日期	

	std::multiset<UINT32> m_lastLotteryGoodItem;

	///> 摇钱树功能
	///> unique day
	UINT32 m_GoldClickDay;
	///> 到期可免费抽奖
	UINT32 m_GoldClickFreeTime;
	///> 每日免费抽次数
	UINT32 m_GoldClickFreeCount;
	///> 每日付费抽次数
	UINT32 m_GoldClickCostCount;

	std::map<UINT32, PandoraDropTimes> m_mapPandora;

	UINT32 m_lastGiftUpdateTime;	// 内置宝箱上次重置次数时间
	UINT32 m_shareGiftCount;		// 内置宝箱今日次数
	UINT32 m_spriteMinGuarantee;	// 精灵蛋抽奖保底次数统计
	UINT32 m_spriteNextMinGuarantee;// 精灵蛋抽奖下一次保底次数
};

#endif // __LOTTERYRECORD_H__