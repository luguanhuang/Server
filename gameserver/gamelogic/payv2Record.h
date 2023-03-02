#ifndef __PAYV2RECORD_H__
#define __PAYV2RECORD_H__

#include "unit/roleserilizeinterface.h"
#include "pb/project.pb.h"
#define MAX_PAY_BILL_TIME 48*3600 //订单的最大有效期;

struct CmpTs 
{  
	bool operator()(const std::pair<std::string, UINT32>& ts1, const std::pair<std::string, UINT32>& ts2) 
	{  
		return ts1.second < ts2.second;  
	}  
};  
struct stPaytssInfo
{
	stPaytssInfo()
	{
		m_iBeginTime = 0;
		m_iEndTime = 0;
		m_iLastGetAwardTime = 0;
	}
	void ToKKSG(KKSG::PaytssInfo* pTssinfo) const;
	void ToThis(const KKSG::PaytssInfo& pTssinfo);
	INT32 m_iBeginTime;
	INT32 m_iEndTime;
	INT32 m_iLastGetAwardTime;
};
class Role;

class CPayV2Record : public RoleSerializeInterface
{
public:
	CPayV2Record(Role* pRole);
	~CPayV2Record();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	int  GetVipLevel() { return m_VipLevel; }
	UINT32 GetSaveAmt() { return m_VipPoint/10 + m_GmTotalAmt; }
	void Update();
	INT32 GetChatCount(){ return m_iChatcount; }
	INT32 GetFatigueLimit(){ return m_iFatigueLimit; }
	INT32 GetReviveCount() {return m_iReviveCount; }
	INT32 GetBossRushCount() {return m_iBossRushCount; }
	INT32 GetBuyGreenAgateLimit(){ return m_iBuyGreenAgateLimit; }
	INT32 GetSuperRiskCount(){ return m_iSuperRiskCount; }
	INT32 GetAbyssCount(){ return m_iAbyssCount; }
	INT32 GetNpcFeelingCount() { return m_iNpcFeelingCount; }
	bool GetIsEverPay() { return m_isEverPay; }
	void SetIsEverPay();
	bool IsCheckinDoubleDays(INT32 iDays);//检查今天签到是否翻倍;
	void CheckMemberPrivilege();//上线，购买、过期的时候检查
	void Init();
	void NotifyMemberPrivilege();
	void ChangeAndNotify();
	void Change(){ m_isModified.Set();}
	void SavePayMemberMask();
	INT32 GetExpireTimeByID(INT32 ID);
	bool IsBuyGrowFund();
	void SetGMTotalAmt(UINT32 amt) { m_GmTotalAmt = amt; }


	void UpateReviveCount();//使用特权复活次数
	bool HasReviveCount();//是否特权复活次数，true为有

	//深渊
	INT32 GetRemainAbyssCount();//剩余特权深渊次数
	INT32 GetUsedAbyssCount();//用了多少次
	void  UpdateAbyssCount();//增加次数
	void  ClearAbyssCount();//清零次数

	//bossrush
	INT32 GetRemainBossRushCount();
	INT32 GetUsedBossRushCount();
	void  UpdateBossRushCount();//
	void  ClearBossRushCount();//清零次数

	//superrisk
	INT32 GetRemainSuperRiskCount();//剩余次数
	bool ReduceSuperRiskCount(INT32 iCount);//扣除次数

	//chat
	INT32 GetRemainChatCount();
	void UpdateChatCount();
	void ClearUpdateChatCount();

	//商城玛瑙
	INT32 GetRemainShopCount(INT32 iGoodsID);
	void UpdateShopCount(INT32 iGoodsID, INT32 iCount);
	void RefreshShopCount();

	bool AddConsumeBill(std::string billno);
	void AddScore(INT32 amt);
	void NotifyPreClient();
	void GetPayScorePre(KKSG::OutLookConsumePre* pScore);
	void GetPayScorePre(KKSG::PayConsume* pScore);
	bool IsActivatePre(UINT32 id);
	void ActivatePre(UINT32 id);
	bool SetPreShow(const KKSG::SetPreShowArg& roArg);
	void ClearSetPre();
	void GiveBuff(KKSG::SceneType type);

	void AddRebate(INT32 amt);
	void CheckRebate();
	UINT32 GetTodayConsume() { return m_RebateConsumeNum;}

	// 礼包
	//inline UINT32 GetRewardCoolTime() { return m_rewardTime; }
	UINT32 RewardCoolTime();
	void SetRewardTime();

	//玩家用户的次数
private:
	INT32 m_iChatcount;
	INT32 m_iFatigueLimit;
	INT32 m_iAbyssCount;
	INT32 m_iReviveCount;
	INT32 m_iBossRushCount;
	INT32 m_iBuyGreenAgateLimit;
	INT32 m_iSuperRiskCount;
	INT32 m_iNpcFeelingCount;
	std::vector<INT32> m_vCheckinDays;

	//玩家已经使用的次数
private:
	INT32 m_iUsedChatCount;
	INT32 m_iUsedAbyssCount;
	INT32 m_iUseReviveCount;
	INT32 m_iUsedBossRushCount;
	INT32 m_iUsedBuyGreenAgateCount;
	INT32 m_iUsedSuperRiskCount;
	INT32 m_iUsedNpcFeelingCount;
	std::vector<KKSG::PayPrivilegeShop> m_vPriShop;

private:
	bool m_isEverPay;//是否曾经充值过充值，true是，false否

public:
	UINT32 m_uLastUpdateDay;
	Role* m_pRole;
	Switch m_isModified;
	UINT32 m_TotalPay;//非直充的金额;
	UINT32 m_VipPoint;//直充的金额;
	UINT32 m_GmTotalAmt;
	UINT32 m_VipLevel;//vip等级;
	UINT32 payCardButtonStatus;//周月卡按钮的状态;
	UINT32 payAileenButtonStatus;//艾琳礼包按钮状态;
	UINT32 m_uLastFirstPayAwardTime;//领取首充奖励时间;
	UINT32 m_uPayFirstAwardButtonStatus;//首充奖励按钮状态;
	UINT32 m_uGrowthFundButtonStatus;//成长基金按钮状态;

	stPaytssInfo payWeekCard;
	stPaytssInfo payMonthCard;
	stPaytssInfo payGrowthFund;

	std::vector<KKSG::PayBaseInfo> payInfo;//只存充值过的记录;
	std::vector<KKSG::PayAwardRecord> fundLevelInfo;//已经领取的成长基金等级领取信息;
	std::vector<KKSG::PayAwardRecord> fundLoginInfo;//已经领取的成长基金登录领取信息;
	std::vector<KKSG::PayAwardRecord> vipLevelGiftInfo;//已经领取的vip等级礼包领取信息;
	std::vector<KKSG::PayMemberRecord> memberInfo;//会员特权相关信息;
	std::map<std::string, KKSG::PayAileenRecord> aileenInfo;//只存购买过的记录;

	std::multimap<UINT32, std::string> billListInfo;
	UINT32 m_rewardTime; // 礼包领取时间
	UINT32 m_growthfundnotifytime;//最后发送基金将关闭邮件的时间;

private:
	std::vector<UINT32> m_vSetId;//已经设置的id;
	std::vector<UINT32> m_vActId;//已经激活的id;

private:
	UINT32 m_RebateConsumeNum;
	UINT32 m_RebateConsumeTime;
	UINT32 m_RebateRate;
};

#endif