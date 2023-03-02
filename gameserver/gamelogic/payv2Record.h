#ifndef __PAYV2RECORD_H__
#define __PAYV2RECORD_H__

#include "unit/roleserilizeinterface.h"
#include "pb/project.pb.h"
#define MAX_PAY_BILL_TIME 48*3600 //�����������Ч��;

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
	bool IsCheckinDoubleDays(INT32 iDays);//������ǩ���Ƿ񷭱�;
	void CheckMemberPrivilege();//���ߣ����򡢹��ڵ�ʱ����
	void Init();
	void NotifyMemberPrivilege();
	void ChangeAndNotify();
	void Change(){ m_isModified.Set();}
	void SavePayMemberMask();
	INT32 GetExpireTimeByID(INT32 ID);
	bool IsBuyGrowFund();
	void SetGMTotalAmt(UINT32 amt) { m_GmTotalAmt = amt; }


	void UpateReviveCount();//ʹ����Ȩ�������
	bool HasReviveCount();//�Ƿ���Ȩ���������trueΪ��

	//��Ԩ
	INT32 GetRemainAbyssCount();//ʣ����Ȩ��Ԩ����
	INT32 GetUsedAbyssCount();//���˶��ٴ�
	void  UpdateAbyssCount();//���Ӵ���
	void  ClearAbyssCount();//�������

	//bossrush
	INT32 GetRemainBossRushCount();
	INT32 GetUsedBossRushCount();
	void  UpdateBossRushCount();//
	void  ClearBossRushCount();//�������

	//superrisk
	INT32 GetRemainSuperRiskCount();//ʣ�����
	bool ReduceSuperRiskCount(INT32 iCount);//�۳�����

	//chat
	INT32 GetRemainChatCount();
	void UpdateChatCount();
	void ClearUpdateChatCount();

	//�̳����
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

	// ���
	//inline UINT32 GetRewardCoolTime() { return m_rewardTime; }
	UINT32 RewardCoolTime();
	void SetRewardTime();

	//����û��Ĵ���
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

	//����Ѿ�ʹ�õĴ���
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
	bool m_isEverPay;//�Ƿ�������ֵ����ֵ��true�ǣ�false��

public:
	UINT32 m_uLastUpdateDay;
	Role* m_pRole;
	Switch m_isModified;
	UINT32 m_TotalPay;//��ֱ��Ľ��;
	UINT32 m_VipPoint;//ֱ��Ľ��;
	UINT32 m_GmTotalAmt;
	UINT32 m_VipLevel;//vip�ȼ�;
	UINT32 payCardButtonStatus;//���¿���ť��״̬;
	UINT32 payAileenButtonStatus;//���������ť״̬;
	UINT32 m_uLastFirstPayAwardTime;//��ȡ�׳佱��ʱ��;
	UINT32 m_uPayFirstAwardButtonStatus;//�׳佱����ť״̬;
	UINT32 m_uGrowthFundButtonStatus;//�ɳ�����ť״̬;

	stPaytssInfo payWeekCard;
	stPaytssInfo payMonthCard;
	stPaytssInfo payGrowthFund;

	std::vector<KKSG::PayBaseInfo> payInfo;//ֻ���ֵ���ļ�¼;
	std::vector<KKSG::PayAwardRecord> fundLevelInfo;//�Ѿ���ȡ�ĳɳ�����ȼ���ȡ��Ϣ;
	std::vector<KKSG::PayAwardRecord> fundLoginInfo;//�Ѿ���ȡ�ĳɳ������¼��ȡ��Ϣ;
	std::vector<KKSG::PayAwardRecord> vipLevelGiftInfo;//�Ѿ���ȡ��vip�ȼ������ȡ��Ϣ;
	std::vector<KKSG::PayMemberRecord> memberInfo;//��Ա��Ȩ�����Ϣ;
	std::map<std::string, KKSG::PayAileenRecord> aileenInfo;//ֻ�湺����ļ�¼;

	std::multimap<UINT32, std::string> billListInfo;
	UINT32 m_rewardTime; // �����ȡʱ��
	UINT32 m_growthfundnotifytime;//����ͻ��𽫹ر��ʼ���ʱ��;

private:
	std::vector<UINT32> m_vSetId;//�Ѿ����õ�id;
	std::vector<UINT32> m_vActId;//�Ѿ������id;

private:
	UINT32 m_RebateConsumeNum;
	UINT32 m_RebateConsumeTime;
	UINT32 m_RebateRate;
};

#endif