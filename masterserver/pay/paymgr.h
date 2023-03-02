//���ļ������������ֵ������ع��ܵ�;
#ifndef __PAYMGR_H__
#define __PAYMGR_H__
#include "util/gametime.h"
#include "role/role.h"
#include "pb/project.pb.h"
#include "httptask/paysend.h"


enum EnumConsumeBillStatus
{
	ConsumeBillStatus_Midas = 1,//midas�۷�״̬
	ConsumeBillStatus_AddItem = 2, //�۷��귢��״̬
};

enum EnumPayTokenStatus
{
	PayTokenStatus_Pay = 1,//�۷�
	PayTokenStatus_AddItem = 2,//�ѷ���״̬;
};

enum EnumPayType
{
	PayTypeNormal = 1, //��ͨ�ĳ�ֵ�����ѣ�����ʯ��
	PayTypeBuyGoods = 2, //ֱ��
	PayTypeXinYueR = 3,//����
};
#define MIN_CHECK_CONSUME_TIME 6
#define MIN_CHECK_SEND_TIME 5
#define GetBalanceTime 5
#define NormalGetBalanceTime 600
#define MAX_PAY_CHECK_TIME 3600
#define CHECK_PAY_TOKEN_TIME 60
#define MAX_CHECK3000111_COUNT 2
#define MAX_PAYTOKEN_CHECK_TIME 600
#define MAX_PAYTOKEN_TIME 48 * 3600
#define MAX_PAY_RETRYCOUNT 128
#define MAX_RETRY_EXPIRE_TIME 24*3600
#define CHECK_RETRY_INTERVAL_TIME 3600
#define GetBalanceUrlPath "/mpay/get_balance_m"
#define SendUrlPath "/mpay/present_m"
#define ConsumeUrlPath "/mpay/pay_m"
#define BuyGoodsUrlPath "/mpay/buy_goods_m"
#define BuyGoodsCallBack "/buynotify"

struct stPayAccessInfo
{
	stPayAccessInfo()
	{
		m_iNoSaveAmt = 0;
		m_uLastCheckPayTime = (INT32)GameTime::GetTime();
		m_oOtherList.clear();
		m_uLastCheckConsumeTime = (INT32)GameTime::GetTime();
		m_oConsumeList.clear();
		m_uLastCheckSendTime = (INT32)GameTime::GetTime();
		m_oSendList.clear();
		m_uRoleID = 0;
		m_iSaveamt = 0;
		m_iRetryCount = 0;
		m_iSendRetryCount = 0;
		m_iConsumeRetryCount = 0;
	}
	void ToKKSG(KKSG::PayAccessInfo* pAccessinfo) const;
	void ToThis(const KKSG::PayAccessInfo& accessinfo);
	INT32 m_iNoSaveAmt;//��ֵδ���˵Ľ��;
	INT32 m_uLastCheckPayTime;//���һ�μ���ֵ��ʱ��;
	INT32 m_iRetryCount;
	INT32 m_iSendRetryCount;
	INT32 m_iConsumeRetryCount;
	std::vector<KKSG::Payotherlist> m_oOtherList;

	INT32 m_uLastCheckConsumeTime;//���һ�μ�����ѵ�ʱ��;
	std::map<std::string, KKSG::Payconsumelist> m_oConsumeList;

	INT32 m_uLastCheckSendTime;//���һ�μ�����͵�ʱ��;
	std::map<std::string, KKSG::Paysendlist> m_oSendList;

	UINT64 m_uRoleID;
	INT32 m_iSaveamt;
};
struct EqualPayParamID
{
	std::string ID;
	EqualPayParamID(std::string strID) { ID = strID;}
	bool operator()(const KKSG::Payotherlist& list)
	{
		return list.paramid() == ID;
	}
};

class PayConsumeTeamTask;

class CPayMgr : public ITimer
{
	CPayMgr();
	~CPayMgr();
	DECLARE_SINGLETON(CPayMgr);

public:
	bool Init();
	void Uninit();
	void BuildHttpParam(CRole* pRole, std::map<std::string, std::string> &params, INT32 paytype=PayTypeNormal);
	void BuildSign(CRole* pRole, std::map<std::string, std::string> &params, std::string url, INT32 paytype=PayTypeNormal);
	void PrintLog(std::map<std::string, std::string> &params);
	void PayTokenInvalidNtf(CRole* role);
	std::string BuildCookie(KKSG::PayParameterInfo oData, std::string org_loc);
	bool AddDiamond(UINT64 roleid, KKSG::Paysendlist list);
	bool AddDiamond(UINT64 roleid, INT32 iCount, INT32 reason, INT32 subreason);//�����Ϸ��;
	bool isFinishConsume(stPayAccessInfo* pData, INT32 type, INT32 index);
	INT32 GetPayCheckIntervalTime(INT32 iCount);

	void SaveData(UINT64 roleid, INT32 op = KKSG::PayAccess_ALL);
	void SaveData2DB(UINT64 roleid, stPayAccessInfo oInfo, INT32 op = KKSG::PayAccess_ALL);
	void SaveConsumeDataOnReply(UINT64 roleid, KKSG::Payconsumelist list, bool isSuccess);
	stPayAccessInfo* GetPayAccessInfo(UINT64 roleid);
	void PayBegin(KKSG::PayParamType paytype, std::string paramid, INT32 amount, UINT64 roleid);
	void PayFinish(bool isOther, std::string paramid, UINT64 roleid, INT32 saveamt);
	void ConsumeFinish(UINT64 roleid, INT32 iBalance, std::string billno);
	void OnTimer(UINT32 dwID, UINT32 dwCount);
	void UpdatePay(stPayAccessInfo* pData, CRole* pRole);
	void UpdateConsume(stPayAccessInfo* pData, CRole* pRole);
	void UpdateSend(stPayAccessInfo* pData, CRole* pRole);
	void PayNotify(CRole* role, KKSG::PayParamType paytype, std::string paramid);
	PayConsumeTeamTask* TeamCost(CRole* role, INT32 rpcid, INT32 iCount);
	void TeamCost(CRole* role, INT32 rpcid, INT32 iCount, const KKSG::TeamOPArg& roArg);
	void TeamCost(CRole* role, INT32 rpcid, INT32 iCount, const KKSG::TeamTransData& roArg);
	void PayConsume(stPayAccessInfo* pData, CRole* pRole);
	void PaySend(stPayAccessInfo* pData, CRole* pRole);
	void OnLogin(CRole* role);
	void UpdateMidasErrCount();
	void ClearMidasErrCount(){ m_ErrCount = 0;}
	std::string GetPayUrl();
	UINT32 GetRequestCount(bool isClear=true);
	void UpdateRequestCount();
	time_t GetGrowthFundCloseTime();
	void ClearTimers();
	void LoadTimer();
	bool GrowthFundNotice();
	std::string GetMidasPayKey(INT32 plattype, INT32 paytype=PayTypeNormal);
	std::string GetMidasPayAppid(INT32 plattype, INT32 paytype=PayTypeNormal);
	void PayOtherFinsih(std::string paramid, UINT64 roleid);
	void CheckPayRetryOver();//�����쳣����(��ֹǰ���󱨵���һֱ����);
	bool CheckIsXinYuePay(PaySendTask* poTask, const KKSG::Paysendlist& data);//�Ƿ�Ϊ���ô���;
	void PayPresentM(CRole* pRole, const KKSG::Paysendlist& list);
	bool IsGMPayCheckRole() {return m_IsGMPayCheckRole;}
	void SetGMPayCheckRole(bool isErr) { m_IsGMPayCheckRole = isErr;}
	UINT32 GetSaveAmt(UINT64 roleid);
	
	// by huangds, 2018/5/16
	void CheckRecharge();
private:
	bool ReadAllPayFromDB();
	typedef std::unordered_map<UINT64, stPayAccessInfo> CPayAccessInfoMap;//ֻ��
	CPayAccessInfoMap m_oPayAccessInfo; 
	HTIMER m_TimeHandler;

	UINT32 m_ErrCount;//��ǰ�������Ӵ���Ĵ��������������л�;
	bool m_bIsMaster;//����;
	UINT32 m_RequestCount;
	time_t m_lastUpdateTime;
	std::vector<UINT32> m_timers;
	UINT32 m_LastCheckRetryOverTime;
	bool m_IsGMPayCheckRole;//���ú󣬳�ֵ��ֱ֤�ӷ��ش������ڲ���;
};
#endif
