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
	pay_param_none = 0,//默认值
	pay_param_list = 1,//充值
	pay_param_aileen = 2,//每日艾琳礼包
	pay_param_card = 3,//周月卡
};

enum growthfund_award_type
{
	growthfund_award_level = 1,//等级奖励
	growthfund_award_login = 2, //登录奖励
};

enum pay_member
{
	pay_member_1 = 1, //冒险家公会
	pay_member_2 = 2, //商会成员
	pay_member_3 = 3, //宫廷贵族
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
	void GetPayAileenInfo(Role* pRole);//获取艾琳礼包信息;
	INT32 Pay(Role* pRole, std::string ParamId, KKSG::PaytssInfo oData);
	INT32 BuyPay(Role* pRole, INT32 iBalance, INT32 iSaveAmt);//充值处理;
	INT32 BuyCard(Role* pRole, std::string ParamId, KKSG::PaytssInfo oData);//购买周月卡处理;
	INT32 BuyAileen(Role* pRole, std::string ParamId, KKSG::PaytssInfo oData);//购买艾琳每日礼包;
	INT32 BuyGrowthFund(Role* pRole, std::string ParamId, KKSG::PaytssInfo oData);//购买成长基金;
	INT32 BuyMember(Role* pRole, std::string ParamId, KKSG::PaytssInfo oData);//购买会员
	void SetButtonStatus(Role* pRole, INT32 nSystemID);
	void CalcVipLevel(Role* pRole, UINT32 value, INT32 type);
	void CheckPaySystemID(Role* pRole, INT32 nSystemID);
	void CheckMemberDragonFlowerAward(Role* pRole);//上线的时候或跨天的时候检查发龙币鲜花奖励;
	bool SendMemberDailyAward(Role* pRole, PayMemberTable::RowData* pRow, INT32 iTime);
	void CheckMemberExpire(Role* pRole);//检查会员是否过期
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

	//gm命令相关
	void SetPayCard(Role* pRole, INT32 nType, INT32 uTime);//gm命令设置周月卡购买时间;
	bool SetPayMemberExpireTime(Role* pRole, INT32 iID, INT32 iTime);
	bool SetPayMemberDragonFlowerTime(Role* pRole, INT32 iID, INT32 iTime);
	
	//通知
	void PayCardHintNotify(Role* pRole);//周月卡红点;
	void VipGiftHintNotify(Role* pRole);//vip红点;
	void GrowthFundHintNotify(Role* pRole);//成长基金红点;
	void AileenHintNotify(Role* pRole);//艾琳礼包红点;
	void PayFirstAwardHintNotify(Role* pRole);//首充奖励红点;
	void NotifyPayAllInfoToClient(Role* pRole, INT32 nType);//type为从哪个类别充值的
	void HintNotify(Role* pRole);//登录游戏，充值小红点通知;
	void PayCheck(Role* pRole);//关于充值相关的，上线检查汇总

	//基金时间控制
	UINT32 GetGrowthFundLeftTime();
	time_t GetGrowthFundCloseTime();

private:
	INT32 m_ExpireSoonTime;//提前通知的时间;
	VIPTable m_oVipTable;
	std::map<INT32, IConsumeListener*> m_oConsumeLister;
};

#endif
