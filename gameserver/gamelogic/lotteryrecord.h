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

	///> ҡǮ������
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

	///> �˶�������
	const ItemDesc* GetPandoraDropItem(UINT32 pandoraDropID);
	void GetPandoraDropItem(UINT32 pandoraDropID, UINT32 openTimes, std::vector<const ItemDesc*>& vecItems);

	bool IsHaveShareGiftCount();
	void AddShareGiftCount();
	void UpdateShareGiftCount();

	void GMClearShareGiftCount();

private:
	void _CheckPandoraDropTime(const PandoraDropConf* conf, UINT32 dropQuality, UINT32 itemQuality, PandoraDropTimes& dropTimes);

private:

	// ����������ʽ m_[�齱����]_[�齱����]_[��׺]
	// ����  m_DragonCoin_1_FreeDrawTime
	//           ����   1����    ��׺
	Role* m_pRole;

	Switch m_isModify;

	UINT32 m_DragonCoin_1_DrawCount;            // ���ҵ��齱�ܴ���
	UINT32 m_DragonCoin_10_MinimumRewardCount;  // ���ҵ�ʮ����ĵͱ�
	
	UINT32 m_Gold_1_DrawCount;					// ��ҵ��齱�ܴ���
	UINT32 m_Gold_10_MinimumRewardCount;		// ��ҵ�ʮ����ĵͱ�

	////////////////////////////////////////////////////////////////
	
	UINT32 m_DragonCoin_1_FreeLastTime;		// �ϴ�������ѹ����ʱ��
	UINT32 m_nextFreeBuyTime;				// �´�������ѹ���ʱ��

	UINT32 m_Gold_1_FreeLastTime;			// �ϴν����ѹ����ʱ��
	UINT32 m_nextFreeGoldBuyTime;			// �´ν�ҹ������ʱ��
	UINT32 m_Gold_1_FreeCount;				// �����Ѿ����˼�����ѽ�ҹ���
	UINT32 m_Gold_1_FreeDay;				// ��ѽ�ҹ��������	

	std::multiset<UINT32> m_lastLotteryGoodItem;

	///> ҡǮ������
	///> unique day
	UINT32 m_GoldClickDay;
	///> ���ڿ���ѳ齱
	UINT32 m_GoldClickFreeTime;
	///> ÿ����ѳ����
	UINT32 m_GoldClickFreeCount;
	///> ÿ�ո��ѳ����
	UINT32 m_GoldClickCostCount;

	std::map<UINT32, PandoraDropTimes> m_mapPandora;

	UINT32 m_lastGiftUpdateTime;	// ���ñ����ϴ����ô���ʱ��
	UINT32 m_shareGiftCount;		// ���ñ�����մ���
	UINT32 m_spriteMinGuarantee;	// ���鵰�齱���״���ͳ��
	UINT32 m_spriteNextMinGuarantee;// ���鵰�齱��һ�α��״���
};

#endif // __LOTTERYRECORD_H__