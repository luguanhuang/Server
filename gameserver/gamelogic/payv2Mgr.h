#ifndef __PAYV2MGR_H__
#define __PAYV2MGR_H__
#include "unit/role.h"
#include "table/VIPTable.h"
#include "table/PayMemberTable.h"
#include "payv2Record.h"
#include "midas.h"

#define WEEK_CARD_DAYS 7
#define MONTH_CARD_DAYS 30
#define EXPIRE_SOON_TIME 

enum pay_param_table
{
	pay_param_none = 0,//Ĭ��ֵ
	pay_param_list = 1,//��ֵ
	pay_param_aileen = 2,//ÿ�հ������
	pay_param_card = 3,//���¿�
};

enum growthfund_award_type
{
	growthfund_award_level = 1,//�ȼ�����
	growthfund_award_login = 2, //��¼����
};

enum pay_member
{
	pay_member_1 = 1, //ð�ռҹ���
	pay_member_2 = 2, //�̻��Ա
	pay_member_3 = 3, //��͢����
};

class PayV2Mgr
{
	PayV2Mgr();
	~PayV2Mgr();
	DECLARE_SINGLETON(PayV2Mgr)

public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	void GetPayInfo(Role* pRole, KKSG::PayAllInfo* roPayInfo);
	void GetPayAileenInfo(Role* pRole);//��ȡ���������Ϣ;
	INT32 Pay(Role* pRole, std::string ParamId, KKSG::PaytssInfo oData);
	INT32 BuyPay(Role* pRole, INT32 iBalance, INT32 iSaveAmt);//��ֵ����;
	INT32 BuyCard(Role* pRole, std::string ParamId, KKSG::PaytssInfo oData);//�������¿�����;
	INT32 BuyAileen(Role* pRole, std::string ParamId, KKSG::PaytssInfo oData);//������ÿ�����;
	INT32 BuyGrowthFund(Role* pRole, std::string ParamId, KKSG::PaytssInfo oData);//����ɳ�����;
	INT32 BuyMember(Role* pRole, std::string ParamId, KKSG::PaytssInfo oData);//�����Ա
	void SetButtonStatus(Role* pRole, INT32 nSystemID);
	void CalcVipLevel(Role* pRole, UINT32 value, INT32 type);
	void CheckPaySystemID(Role* pRole, INT32 nSystemID);
	void CheckMemberDragonFlowerAward(Role* pRole);//���ߵ�ʱ�������ʱ���鷢�����ʻ�����;
	bool SendMemberDailyAward(Role* pRole, PayMemberTable::RowData* pRow, INT32 iTime);
	void CheckMemberExpire(Role* pRole);//����Ա�Ƿ����
	void SendMail(Role* pRole, INT32 iID, INT32 iTime);
	void SendDailyAwardMail(Role* pRole, INT32 iID, INT32 iTime, std::vector<ItemDesc>& items);
	void ChangePayMember(Role* pRole, INT32 iID);
	bool IsNeedOpenPaySystemID(Role* pRole, INT32 nSystemID);
	INT32 GetRemainedDays(INT32 iTime);
	void NotifyDiamond(Role* pRole, INT32 iCount);
	void DoTxLog(Role* pRole, INT32 iAmount, std::string ParamId);
	void SetPayAileenStatus(Role* pRole, std::string paramid);
	void CloseGrowthFund(Role* pRole, bool isopen=false);

	VIPTable::RowData *GetVipConf(int vipLevel);
	UINT32 GetVipLevel(UINT32 rmb);
	UINT32 GetGoldClickCount(int vip);

	IConsumeListener* GetConsumeLister(INT32 type);

	//gm�������
	void SetPayCard(Role* pRole, INT32 nType, INT32 uTime);//gm�����������¿�����ʱ��;
	bool SetPayMemberExpireTime(Role* pRole, INT32 iID, INT32 iTime);
	bool SetPayMemberDragonFlowerTime(Role* pRole, INT32 iID, INT32 iTime);
	
	//֪ͨ
	void PayCardHintNotify(Role* pRole);//���¿����;
	void VipGiftHintNotify(Role* pRole);//vip���;
	void GrowthFundHintNotify(Role* pRole);//�ɳ�������;
	void AileenHintNotify(Role* pRole);//����������;
	void PayFirstAwardHintNotify(Role* pRole);//�׳佱�����;
	void NotifyPayAllInfoToClient(Role* pRole, INT32 nType);//typeΪ���ĸ�����ֵ��
	void HintNotify(Role* pRole);//��¼��Ϸ����ֵС���֪ͨ;
	void PayCheck(Role* pRole);//���ڳ�ֵ��صģ����߼�����

	//����ʱ�����
	UINT32 GetGrowthFundLeftTime();
	time_t GetGrowthFundCloseTime();

private:
	INT32 m_ExpireSoonTime;//��ǰ֪ͨ��ʱ��;
	VIPTable m_oVipTable;
	std::map<INT32, IConsumeListener*> m_oConsumeLister;
};

#endif
