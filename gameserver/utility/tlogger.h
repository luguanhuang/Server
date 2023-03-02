#ifndef _H_TLog_H__
#define _H_TLog_H__
#include "util/jsonutil.h"
#include "unit/pet.h"
#include "tlogmgr.h"
#include "tlogwriter.h"
class Role;
class LogHelper;

INT32 GetTLogMoneyType(UINT32 dwItemID);
INT32 GetMoneyTypeFromTxType(INT32 iType);
INT32 GetChatType(INT32 iType);
class TServerStateLog
{
public:
	TServerStateLog() {}
	~TServerStateLog() {}
	void Do();
};

//腾讯tlog日志的接口定义后，不能修改，之前第一批的用这个老的，后面的统一规范用TRoleLog;
class TRoleLogOld
{
public:
	explicit TRoleLogOld(Role* poRole)
	{
		m_poRole = poRole;
		m_strTransTag = TTag::Instance()->GetTagString();
	}
	virtual ~TRoleLogOld() {}

	void Do();
	void SetTransTag(std::string tag) { m_strTransTag = tag;}
	virtual void Build(LogHelper& roLog) {}
	virtual const char* GetTableName() const { return ""; }
protected:
	Role*	m_poRole;
	std::string m_strTransTag;
};

class TRoleLog
{
public:
	explicit TRoleLog(Role* poRole)
	{
		m_poRole = poRole;
		m_strTransTag = TTag::Instance()->GetTagString();
	}
	virtual ~TRoleLog() {}

	void Do();
	void SetTransTag(std::string tag) { m_strTransTag = tag;}
	const std::string GetTransTag() { return m_strTransTag; }
	virtual void Build(LogHelper& roLog) {}
	virtual const char* GetTableName() const { return ""; }
protected:
	Role*	m_poRole;
	std::string m_strTransTag;
};

class TCommonLog
{
public:
	explicit TCommonLog(Role* poRole) :m_poRole(poRole) {}
	virtual ~TCommonLog() {}

	void Do();
	virtual void Build(LogHelper& roLog) {}
	virtual const char* GetTableName() const { return "";}

protected:
	Role*	m_poRole;
};
class TLoginLog : public TRoleLogOld
{
public:
	explicit TLoginLog(Role* poRole) :TRoleLogOld(poRole) {}
	virtual ~TLoginLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "PlayerLogin"; }
};


class TLogoutLog : public TRoleLogOld
{
public:
	explicit TLogoutLog(Role* poRole)
		:TRoleLogOld(poRole)
		,m_dwOnlieTime(0) {}
	virtual ~TLogoutLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "PlayerLogout"; }
public:
	UINT32	m_dwOnlieTime;		//本次登录在线时间(秒)
};


class TMoneyFlowLog : public TRoleLogOld
{
public:
	explicit TMoneyFlowLog(Role* poRole)
		:TRoleLogOld(poRole)
		,m_nMoneyChange(0)
		,m_nReason(0)
		,m_nSubReason(0)
		,m_nAddOrReduce(0)
		,m_nMoneyType(0) {}
	virtual ~TMoneyFlowLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "MoneyFlow"; }
public:
	INT32	m_nMoneyChange;		//动作涉及的金钱数
	INT32	m_nReason;			//货币流动一级原因
	INT32	m_nSubReason;		//货币流动二级原因
	INT32	m_nAddOrReduce;		//
	INT32	m_nMoneyType;		//
};

class TItemFlowLog : public TRoleLogOld
{
public:
	explicit TItemFlowLog(Role* poRole)
		:TRoleLogOld(poRole)
		,m_nItemID(0)
		,m_nType(0)
		,m_nCount(0)
		,m_nAfterCount(0)
		,m_nReason(0)
		,m_nSubReason(0)
		,m_nMoneyCost(0)
		,m_nMoneyType(0)
		,m_nAddOrReduce(0)
		,m_nQuality(0){}
	virtual ~TItemFlowLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "ItemFlow"; }
public:
	INT32	m_nItemID;
	INT32	m_nType;			//道具类型
	INT32	m_nCount;			//改变数量
	INT32	m_nAfterCount;		//动作后的物品存量

	INT32	m_nReason;			//道具流动一级原因
	INT32	m_nSubReason;		//道具流动二级原因
	INT32	m_nMoneyCost;		//花费代币或金币购买道具情况下输出消耗的钱数量，否则填0
	INT32	m_nMoneyType;		//钱的类型MONEYTYPE
	INT32	m_nAddOrReduce;		//增加/减少
	INT32	m_nQuality;			//品质
};


class TPlayerExpFlowLog : public TRoleLogOld
{
public:
	explicit TPlayerExpFlowLog(Role* poRole)
		:TRoleLogOld(poRole)
		,m_nExpChange(0)
		,m_nBeforeLevel(0)
		,m_nAfterLevel(0)
		,m_nTime(0)
		,m_nReason(0)
		,m_nSubReason(0)
		,m_WorldLevelBuff(0.0)
		,m_LevelSealBuff(0.0)
		,m_CurExp(0)
		,m_PrePowerPoint(0){}
	virtual ~TPlayerExpFlowLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "PlayerExpFlow"; }
public:
	INT32	m_nExpChange;		//经验变化
	INT32   m_nBeforeLevel;		//动作前等级
	INT32	m_nAfterLevel;		//动作后等级
	INT32	m_nTime;			//升级所用时间(秒)
	INT32	m_nReason;			//经验流动一级原因
	INT32	m_nSubReason;		//道具流动二级原因
	double  m_WorldLevelBuff;   //世界等级buff;
	float   m_LevelSealBuff;	//封印等级buff;
	INT32   m_CurExp;			//当前经验值
	INT32   m_PrePowerPoint;	//升级前的战力
};


class TSnsFlowLog : public TRoleLogOld
{
public:
	explicit TSnsFlowLog(Role* poRole)
		:TRoleLogOld(poRole)
		,m_nCount(0)
		,m_nSNSType(0)
		,m_nSNSSubType(0) {}
	virtual ~TSnsFlowLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "SnsFlow"; }
public:
	INT32	m_nCount;			//发送的数量
	INT32	m_nSNSType;			//交互一级类型
	INT32	m_nSNSSubType;		//交互二级类型
};


class TRoundFlowLog : public TRoleLogOld
{
public:
	explicit TRoundFlowLog(Role* poRole)
		:TRoleLogOld(poRole)
		,m_nBattleID(0)
		,m_nBattleType(0)
		,m_nRoundScore(0)
		,m_nRoundTime(0)
		,m_nResult(0)
		,m_nRank(0)
		,m_nGold(0)
		,m_nStar(0)
		,m_nTeam(0)
		,m_nTopFloor(0)
		,m_nPowerPoint(0)
		,m_iStageTime(0)
		,m_iHpPercent(0)
		,m_iReviveCount(0)
		,m_iHelper(0)
		,m_iCount(0)
		,m_iAutoFight(0)
		,m_iContinueWin(0)
		,m_iContinueWinHis(0)
		,m_TeamCount(0)
		,m_TeamSurviveCount(0)
		,m_Damge(0)
		,m_BeHitDamage(0)
		,m_Uid(0)
		,m_TeamID(0)
		,m_DeathCount(0)
		,m_BrID(0)
		,m_Brbid1(0)
		,m_Brbid2(0)
		,m_BrrefreshCount(0){}
	virtual ~TRoundFlowLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "RoundFlow"; }
public:
	INT32	m_nBattleID;		//关卡id或者副本id[副本ID必须有序]，对局时填0
	INT32	m_nBattleType;		//战斗类型
	INT32	m_nRoundScore;		//本局分数,无得分的填0
	INT32	m_nRoundTime;		//对局时长(秒)
	INT32	m_nResult;			//单局结果
	INT32	m_nRank;			//排名
	INT32	m_nGold;			//金钱
	INT32   m_nStar;			//星级
	INT32   m_nTeam;			//0:非组队， 1：组队
	INT32   m_nTopFloor;			//层数
	INT32   m_nPowerPoint;		//战力
	INT32   m_iStageTime;		//通关时长
	INT32   m_iHpPercent;		//剩余血量百分比
	INT32   m_iReviveCount;		//复活次数
	INT32   m_iHelper;			//是否助战0否 1是;
	INT32	m_iCount;			//扫荡次数
	INT32	m_iAutoFight;		//自动战斗0：否 1：是
	std::string m_strPkWinRate; //天梯赛总胜率 用于wx数据上报
	std::string m_strPvpWinRate;//保护队长胜率 用于wx数据上报
	INT32	m_iContinueWin;		// 本赛季最大连胜
	INT32	m_iContinueWinHis;	// 历史最大连胜		
	INT32	m_TeamCount;		//队伍人数
	INT32	m_TeamSurviveCount;	//队伍存活人数
	UINT64  m_Damge;//伤害
	UINT64  m_BeHitDamage;//受到伤害;
	UINT32  m_Uid;//战斗id
	INT32 m_TeamID;
	INT32 m_DeathCount;//死亡次数;
	INT32 m_BrID;//bossrush唯一id
	INT32 m_Brbid1;//bossrush buff1 id
	INT32 m_Brbid2;//bossrush buff2 id
	INT32 m_BrrefreshCount;//刷新次数;
};

class TGuideFlowLog : public TRoleLogOld
{
public:
	explicit TGuideFlowLog(Role* poRole)
		:TRoleLogOld(poRole)
		,m_nGuideID(0) {}
	virtual ~TGuideFlowLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "GuideFlow"; }
public:
	INT32	m_nGuideID;			//节点ID(节点ID按实际步骤递增编号)
};

class TVipLevelFlowLog : public TRoleLogOld
{
public:
	explicit TVipLevelFlowLog(Role* poRole)
		:TRoleLogOld(poRole)
		,m_nBeforeVipLevel(0)
		,m_nAfterVipLevel(0) {}
	virtual ~TVipLevelFlowLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "VipLevelFlow"; }
public:
	INT32	m_nBeforeVipLevel;	//动作前等级
	INT32	m_nAfterVipLevel;	//动作后等级
};

class TFatigueLog : public TRoleLog
{
public:
	explicit TFatigueLog(Role* poRole)
		:TRoleLog(poRole)
		,m_nFatigueChange(0)
		,m_nReason(0)
		,m_nSubReason(0)
		,m_nAddOrReduce(0)
		,m_nSceneType(0){}
	virtual ~TFatigueLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "Fatigue"; }
public:
	INT32	m_nFatigueChange;		//动作涉及的金钱数
	INT32	m_nReason;			//货币流动一级原因
	INT32	m_nSubReason;		//货币流动二级原因
	INT32	m_nAddOrReduce;		//
	INT32	m_nSceneType;
};
//副本扫荡
class TSweepLog : public TRoleLog
{
public:
	explicit TSweepLog(Role* poRole)
		:TRoleLog(poRole)
		,m_nBattleID(0)
		,m_nBattleType(0)
		,m_nResult(0)
		,m_nPowerPoint(0)
		,m_nCount(0){}
	virtual ~TSweepLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "Sweep"; }
public:
	INT32	m_nBattleID;		//关卡id或者副本id[副本ID必须有序]，对局时填0
	INT32	m_nBattleType;		//战斗类型
	INT32	m_nResult;			//单局结果
	INT32   m_nPowerPoint;		//战力
	INT32   m_nCount;			//扫荡次数
};

class TActivityFlowLog : public TRoleLogOld
{
public:
	explicit TActivityFlowLog(Role* poRole)
		:TRoleLogOld(poRole)
		,m_nVipLevel(0)
		,m_nActivityType(0)
		,m_nActivityID(0) {}
	virtual ~TActivityFlowLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "ActivityFlow"; }
public:
	INT32	m_nVipLevel;		//VIP等级
	INT32	m_nActivityType;	//活动类型
	INT32	m_nActivityID;		//活动ID
};


class TTaskFlowLog : public TRoleLogOld
{
public:
	explicit TTaskFlowLog(Role* poRole)
		:TRoleLogOld(poRole)
		,m_nVipLevel(0)
		,m_nTaskType(0)
		,m_nTaskID(0)
		,m_nState(0)
		,m_Score(0){}
	virtual ~TTaskFlowLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TaskFlow"; }
public:
	INT32	m_nVipLevel;		//VIP等级
	INT32	m_nTaskType;		//任务类型
	INT32	m_nTaskID;			//任务ID
	INT32	m_nState;			//任务操作状态
	INT32   m_Score;
};


class TLotteryFlowLog : public TRoleLogOld
{
public:
	explicit TLotteryFlowLog(Role* poRole)
		:TRoleLogOld(poRole)
		,m_nVipLevel(0)
		,m_nLotteryType(0)
		,m_nMoney(0){}
	virtual ~TLotteryFlowLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "LotteryFlow"; }
public:
	INT32	m_nVipLevel;		//VIP等级
	INT32	m_nLotteryType;		//抽取类型
	INT32   m_nMoney;		//钱类型
};

//新加的tlog用ToleLog
class TUIGuideFlowLog : public TRoleLog
{
public:
	explicit TUIGuideFlowLog(Role* poRole)
		:TRoleLog(poRole)
		,m_nPosition(0)
		,arg(""){}
	virtual ~TUIGuideFlowLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "UIGuideFlow"; }
public:
	INT32	m_nPosition;//打点位置
	std::string arg;	
};
class TItemDesc
{
public:
	TItemDesc(){}
	void SetLogHelper(LogHelper& roLog, INT32 ItemNum);
public:
	std::vector<ItemDesc> m_vList;
};
class TItemEnHanceLog : public TRoleLog
{
public:
	explicit TItemEnHanceLog(Role* poRole)
		:TRoleLog(poRole)
		,m_nOperType(0)
		,m_nChgLevel(0)
		,m_nAfterLevel(0)
		,m_iItemID(0)
		,m_lUID(0)
		,m_iItemID2(0)
		,m_iLevel2(0)
		,m_lUID2(0)
		,m_Quality(0)
		,m_Quality2(0){}
	virtual ~TItemEnHanceLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "ItemEnHance"; }
public:
	INT32 m_nOperType;		//变化类型
	INT32 m_nChgLevel;		//装备等级变化值
	INT32 m_nAfterLevel;	//变化后等级
	INT32 m_iItemID;		//装备ID
	INT64 m_lUID;			//装备唯一标识ID
	INT32 m_iItemID2;		//从这个item转移的;
	INT32 m_iLevel2;		//装备等级
	INT64 m_lUID2;
	INT32 m_Quality;
	INT32 m_Quality2;
	TItemDesc m_ItemList;
};

class TPkLog : public TRoleLog
{
public:
	explicit TPkLog(Role* poRole)
		:TRoleLog(poRole)
		,m_nVipLevel(0)
		,m_nEmenyRoleID(0)
		,m_nResultID(0)
		,m_nBeforePoint(0)
		,m_nAfterPoint(0)
		,m_nEmenyPro(0)
		,m_nEmenyPowerPoint(0){}
	virtual ~TPkLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "PkFlow"; }
public:
	INT32 m_nVipLevel;		//VIP等级
	INT64 m_nEmenyRoleID;	//对手roleid
	INT32 m_nResultID;		//挑战结果
	INT32 m_nBeforePoint;	//挑战前评分
	INT32 m_nAfterPoint;	//挑战后评分
	INT32 m_nEmenyPro;		//对手职业
	INT32 m_nEmenyPowerPoint; //对手战力
	std::string strEmenyName; //对手名字
};

class TOpenSystemLog : public TRoleLog
{
public:
	explicit TOpenSystemLog(Role* poRole)
		:TRoleLog(poRole)
	,m_nSystemID(0){}
	virtual ~TOpenSystemLog(){}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "OpenSystemLog";}
public:
	INT32 m_nSystemID;
};

class TOdViewFlow : public TRoleLog
{
public:
	explicit TOdViewFlow(Role* poRole)
		:TRoleLog(poRole)
	,m_nOdType(0){}

	virtual ~TOdViewFlow(){}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "OdViewFlow";}
public:
	INT32 m_nOdType;
};

class TGuildFightFlow : public TRoleLog
{
public:
	explicit TGuildFightFlow(Role* poRole)
		:TRoleLog(poRole)
	,m_nGuildID(0)
	,m_nPowerPoint(0)
	,m_nChallengeType(0){}
	virtual ~TGuildFightFlow(){}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "GuildFightFlow";}
public:
	UINT64 m_nGuildID;
	INT32 m_nPowerPoint;
	INT32 m_nChallengeType;
};

class TGuildArenaFlow : public TRoleLog
{
public:
	explicit TGuildArenaFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_uGuildID = 0;
		m_uGuildID2 = 0;
		m_uRoleId2 = 0;
		m_iProfession2 = 0;
		m_iLevel2 = 0;
		m_iVipLevel2 = 0;
		m_iPowerPoint2 = 0;
		m_iPoint = 0;
		m_iOp = 0;
	}

	virtual ~TGuildArenaFlow(){}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "GuildArenaFlow";}

public:
	UINT64 m_uGuildID;
	UINT64 m_uGuildID2;
	UINT64 m_uRoleId2;
	INT32 m_iProfession2;
	INT32 m_iLevel2;
	INT32 m_iVipLevel2;
	INT32 m_iPowerPoint2;
	INT32 m_iPoint;
	INT32 m_iOp;
	std::string m_strOpenId2;
};

class TBossRushRefreshFlow : public TRoleLog
{
public:
	explicit TBossRushRefreshFlow(Role* poRole)
		:TRoleLog(poRole)
	,m_nCurRefreshCount(0)
	,m_iCurBossCount(0){}

	virtual ~TBossRushRefreshFlow(){}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "BossRushRefreshFlow";}

public:
	INT32 m_nCurRefreshCount;
	INT32 m_iCurBossCount;
};

class TAuctionFlow : public TRoleLog
{
public:
	explicit TAuctionFlow(Role* poRole)
		:TRoleLog(poRole)
		,m_nItemID(0)
		,m_nItemCount(0)
		,m_nPrice(0)
		,m_nOperType(0)
		,szItemName(""){}

	virtual ~TAuctionFlow(){}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "AuctionFlow";}

public:
	INT32 m_nItemID;
	INT32 m_nItemCount;
	INT32 m_nPrice;	//价格
	INT32 m_nOperType; //出售、购买
	std::string szItemName;
};

class TPayFlow : public TRoleLog
{
public:
	explicit TPayFlow(Role* poRole)
		:TRoleLog(poRole)
		,m_iAmount(0){}

	virtual ~TPayFlow(){}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "PayFlow";}

public:
	INT32 m_iAmount;
	std::string m_szParamID;
	std::string m_szOrderID;
	std::string m_szUserIP;
};
class TShopFlow : public TRoleLog
{
public:
	explicit TShopFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_iShopType = 0;
		m_iMoneyType = -1;
		m_iMoneyValue = 0;
		m_iMoneyValueExt = 0;
		m_iItemID = 0;
		m_iItemCount = 0;
		m_iGoodsType = 0;
		m_iLimitFlag = 0;
	}

	virtual ~TShopFlow(){}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "ShopFlow";}

public:
	INT32 m_iShopType;
	INT32 m_iMoneyType;
	INT32 m_iMoneyValue;
	INT32 m_iMoneyValueExt;
	INT32 m_iItemID;
	INT32 m_iItemCount;
	INT32 m_iGoodsType;
	INT32 m_iLimitFlag;
};
class TSendFlowerFlow : public TRoleLog
{
public:
	explicit TSendFlowerFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_iCostID = 0;
		m_iCostNum = 0;
		m_iSendItemID = 0;
		m_iSendCount = 0;
		m_ToRoleId = 0;
	}

	virtual ~TSendFlowerFlow(){}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "SendFlowerFlow";}

public:
	INT32 m_iCostID;
	INT32 m_iCostNum;
	INT32 m_iSendItemID;
	INT32 m_iSendCount;
	UINT64 m_ToRoleId;
};

class TAnswerFlow : public TRoleLog
{
public:
	explicit TAnswerFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_iAnswerType = 0;
		m_iResultID = 0;
		m_uQuestionID = 0;
		m_iIndex = -1;
	}

	virtual ~TAnswerFlow(){}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "AnswerFlow";}

public:
	INT32 m_iAnswerType;
	INT32 m_iResultID;
	UINT32 m_uQuestionID;
	INT32 m_iIndex;
};

class TGuildCardFlow : public TRoleLog
{
public:
	explicit TGuildCardFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_iOperType = 0;
		m_lGuildID = 0;
		m_iGuildLevel = 0;
	}

	virtual ~TGuildCardFlow(){}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "GuildCardFlow";}

public:
	INT32 m_iOperType;
	INT64 m_lGuildID;
	INT32 m_iGuildLevel;
};

class TSpriteFlow : public TRoleLog
{
public:
	explicit TSpriteFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_uUid = 0;
		m_uSpriteID = 0;
		m_uSkillID = 0;
		m_uLevel = 0;
		m_uEvolutionLevel = 0;
		m_uExp = 0;
		m_uPowerPoint = 0;
		m_iOperType = -1;
		m_iChangeValue = 0;	
		m_iReason = -1;
		m_iSubReason = -1;
		m_Quality = 0;
	}
	virtual ~TSpriteFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "SpriteFlow";}

public:
	UINT64 m_uUid;
	UINT32 m_uSpriteID;
	UINT32 m_uSkillID;
	UINT32 m_uLevel;			
	UINT32 m_uEvolutionLevel;	//升星等级
	UINT32 m_uExp;				//精灵经验
	UINT32 m_uPowerPoint;		//精灵战力
	INT32 m_iOperType;			//行为类别
	INT32 m_iChangeValue;		//变化值	
	INT32	m_iReason;			//货币流动一级原因
	INT32	m_iSubReason;		//货币流动二级原因
	std::string m_strPassiveSkillID;//精灵技能 ID1,ID2,ID3
	INT32 m_Quality; //品质
};

class TPetFlow : public TRoleLog
{
public:
	explicit TPetFlow(Role* role)
		:TRoleLog(role)
	{
		m_uUid = 0;
		m_uPetID = 0;
		m_uLevel = 0;
		m_uExp = 0;
		m_uSex = 0;
		m_uPower = 0;
		m_uMood = 0;
		m_uHunary = 0;
		m_iOperType = -1;
		m_iChangeValue = 0;
	}
	void SetData(Pet* pet);
	virtual ~TPetFlow(){};
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "PetFlow";}
public:
	UINT64 m_uUid;
	UINT32 m_uPetID;
	UINT32 m_uLevel;
	UINT32 m_uExp;
	UINT32 m_uSex;
	UINT32 m_uPower;
	UINT32 m_uMood;
	UINT32 m_uHunary;
	INT32 m_iChangeValue;		//变化值
	INT32 m_iOperType;			//行为类别
	std::string m_strFixedSkills;
	std::string m_strRandSkills;
};

class TSuperRiskFlow : public TRoleLog
{
public:
	explicit TSuperRiskFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_iMapID = 0;
		m_iCurx = 0;
		m_iCury = 0;
		m_iDirection = 0;
	}

	virtual ~TSuperRiskFlow(){};
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "SuperRiskFlow";}
public:
	INT32 m_iMapID;
	INT32 m_iCurx;
	INT32 m_iCury;
	INT32 m_iDirection;
};

class TDesignationFlow : public TRoleLog
{
public:
	explicit TDesignationFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_uDesignationID = 0;
		m_iOperType = -1;
	}
	virtual ~TDesignationFlow(){};
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "DesignationFlow";}

public:
	UINT32 m_uDesignationID;
	INT32 m_iOperType;
};

class TSkyCityFlow : public TRoleLog
{
public:
	explicit TSkyCityFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_iResult = 0;
		m_TeamID = 0;
	}
	virtual ~TSkyCityFlow(){};
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "SkyCityFlow";}
public:
	INT32 m_iResult;
	UINT32 m_TeamID;
};


class TResWarFlow : public TRoleLog
{
public:
	explicit TResWarFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_iResult = 0;
	}
	virtual ~TResWarFlow(){};
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "SkyCityFlow";}
public:
	INT32 m_iResult;
};

class TSceneFlow : public TRoleLog
{
public:
	explicit TSceneFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_iSceneType = 0;
		m_iTotalTime = 0;
		m_iRoleNum = 0;
		m_iStatus = 0;
		m_iWatch = -1;
	}
	virtual ~TSceneFlow(){};
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "SceneFlow";}

public:
	INT32 m_iSceneType;
	INT32 m_iTotalTime;
	INT32 m_iRoleNum;
	INT32 m_iStatus;
	INT32 m_iWatch;//0:非观战 1:观战 2:观战点赞;
};
class TGuildInheritFlow : public TRoleLog
{
public:
	explicit TGuildInheritFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		pInheritRole = NULL;
	}

	virtual ~TGuildInheritFlow(){};
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "GuildInheritFlow";}

public:
	Role* pInheritRole;
};

class TDanceFlow : public TRoleLog
{
public:
	explicit TDanceFlow(Role* poRole)
		:TRoleLog(poRole)
	,m_iOp(0){}

	virtual ~TDanceFlow(){};
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "DanceFlow";}

public:
	INT32 m_iOp;
};

class TRoundUnlockLog : public TRoleLog
{
public:
	explicit TRoundUnlockLog(Role* poRole)
		:TRoleLog(poRole)
	{
		m_BattleID = 0;
		m_BattleType = 0;
	}
	virtual ~TRoundUnlockLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "RoundUnlockFlow"; }
public:
	INT32 m_BattleID;
	INT32 m_BattleType;
};

class TProfessionFlow : public TRoleLog
{
public:
	explicit TProfessionFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_PreProfession = 0;
		m_Op = 0;
	}
	virtual ~TProfessionFlow() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "ProfessionFlow"; }
public:
	INT32 m_PreProfession;
	INT32 m_Op;
};

class TTitleFlow : public TRoleLog
{
public:
	explicit TTitleFlow(Role* poRole)
		:TRoleLog(poRole){}
	virtual ~TTitleFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TitleFlow"; }
};

struct TEmbleAttr
{
	TEmbleAttr()
	{
		m_AttrID = 0;
		m_AttrValue = 0;
		m_AttrFirst = 0;
		m_AttrSecond = 0;
	}
	UINT32 m_AttrID;
	UINT32 m_AttrValue;
	UINT32 m_AttrFirst;
	UINT32 m_AttrSecond;
};
typedef struct TEmbleAttr TBaseAttr;

//纹章的洗炼等
class TEmblemFlow : public TRoleLog
{
public:
	explicit TEmblemFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_EmblemID = 0;
		m_EmblemLv = 0;
		m_QualityID = 0;
		m_Op = 0;
		m_ThirdSlot = 0;
		m_EmblemUid = 0;
		m_SmeltAttrID = 0;
		m_AttrNum = 0;
	}
	virtual ~TEmblemFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "EmblemFlow"; }

public:
	UINT32 m_EmblemID;//纹章ID ----需要填写
	UINT32 m_EmblemLv;//等级  ----需要填写
	UINT32 m_QualityID;//品质
	INT32 m_Op;//1：鉴定 2:洗炼 3：装备 TEmblemType   ----需要填写
	UINT32 m_ThirdSlot;//0代表没有，1代表有孔未鉴定，2代表有孔且鉴定	  ----需要填写
	UINT64 m_EmblemUid;//uid  ----需要填写
	INT32 m_SmeltAttrID;//洗练的属性ID;
	UINT32 m_AttrNum;//属性个数
	std::vector<TEmbleAttr> m_vAttrList;
	TItemDesc m_ItemList;
};

class TItemAttr
{
public:
	TItemAttr()
	{
		m_EnchantAttrID = 0;
		m_EnchantAttrValue = 0;
		m_ForgeAttrNum = 0;
		m_RandomAttrNum = 0;
	}
	void SetItem(XItem* pItem);
	void SetBaseLogHelper(LogHelper& roLog);
	void SetLogHelper(LogHelper& roLog);
	void SetLogEnchanNotAttr(LogHelper& roLog);
public:
	UINT32 m_EnchantAttrID;//附魔属性id
	UINT32 m_EnchantAttrValue;//附魔属性值
	UINT32 m_ForgeAttrNum;
	UINT32 m_RandomAttrNum;
	std::vector<TBaseAttr> m_vForgeList;//锻造信息;
	std::vector<TBaseAttr> m_vRandomList;//随机属性;
	std::vector<TBaseAttr> m_vEnchantNotList;//附魔未使用的属性
};
//装备的洗练等
class TItemSysFlow : public TRoleLog
{
public:
	explicit TItemSysFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_ItemID = 0;
		m_ItemLv = 0;
		m_Op = 0;
		m_Quality = 0;
		m_ItemUid = 0;
		m_EnchantID = 0;
		m_JadeID = 0;
		m_PreJadeID = 0;
		m_FuseLevel = 0;
		m_FuseExp = 0;
	}
	virtual ~TItemSysFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "ItemSysFlow"; }

public:
	UINT32	m_ItemID;  //----需要填写
	UINT32   m_ItemLv;  //----需要填写
	INT32   m_Op;//1洗练 2锻造 3附魔TItemSysType  //----需要填写
	UINT32	m_Quality;			//品质
	UINT64	m_ItemUid;  //----需要填写
	UINT32  m_EnchantID; //附魔时消耗的id
	UINT32 m_JadeID;//操作的龙玉ID
	UINT32 m_PreJadeID;//操作前的龙玉ID,仅适用于龙玉升级;
	UINT32 m_FuseLevel;
	UINT32 m_FuseExp;
	//UINT32 m_SmeltAttrID;//洗练的属性id;
	TItemAttr m_Attr;
	TItemDesc m_ItemList;
};

//技能装备
class TSkillFlow : public TRoleLog
{
public:
	explicit TSkillFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_Page = 0;
	}
	virtual ~TSkillFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "SkillFlow"; }

public:
	UINT32 m_Page;//当前携带的页数
	std::string skill1;//第一页携带的技能
	std::string skill2;//第2页携带的技能
};

//英雄战场购买英雄的记录;
class THeroBattleBuyFlow : public TRoleLog
{
public:
	explicit THeroBattleBuyFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_CostID = 0;
		m_CostNum = 0;
		m_HeroID = 0;
	}
	virtual ~THeroBattleBuyFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "HeroBattleBuyFlow"; }

public:
	UINT32 m_CostID; //消耗的道具ID  ----需填写
	UINT32 m_CostNum;//消耗的道具数  ----需填写
	UINT32 m_HeroID;//购买的英雄ID ---需填写
};

//英雄战场玩法参与;
class THeroBattleRoundFlow : public TRoleLog
{
public:
	explicit THeroBattleRoundFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_Elo = 0;
		m_Result = 0;
		m_KillNum = 0;
		m_DeadNum = 0;
		m_IsMvp = 0;
	}
	virtual ~THeroBattleRoundFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "HeroBattleRoundFlow"; }

public:
	UINT32 m_Elo; //  ----需填写
	UINT32 m_NewElo;
	UINT32 m_Result;//对局结果  TBattleResult ----需填写
	UINT32 m_KillNum;//击杀次数 ---需填写
	UINT32 m_DeadNum;//死亡次数 ---需填写 
	UINT32 m_IsMvp;//0否 1是  ---需填写 
	std::string m_strHaveHero;//已拥有英雄id1#id2#id3  ---需填写 
	std::string m_strFreeWeekHero;//限免英雄 id1#id2#id3  ---需填写 
};

//英雄参战的记录;
class THeroBattleResultFlow : public TRoleLog
{
public:
	explicit THeroBattleResultFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_UseHeroID = 0;
		m_KillNum = 0;
		m_LifeTime = 0;
		m_Damage = 0;
	}
	virtual ~THeroBattleResultFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "THeroBattleResultFlow"; }

public:
	UINT32 m_UseHeroID;//使用的英雄ID; ---需填写
	UINT32 m_KillNum;//击杀数; ---需填写
	UINT32 m_LifeTime;//英雄存活时长; ---需填写
	UINT32 m_Damage;//英雄收获的伤害; ---需填写
	UINT32 m_Order;//上场顺序
	UINT32 m_IsChange;//是否改变（0未改变，1改变）
	UINT32 m_IsDead;//是否死亡（0未死亡，1死亡）
};

class TActiveDegreeFlow : public TRoleLog
{
public:
	TActiveDegreeFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_ActivityId = 0;
		m_Value = 0;
		m_TotalValue = 0;
	}
	virtual ~TActiveDegreeFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TActiveDegreeFlow"; }

public:
	UINT32 m_ActivityId;
	UINT32 m_Value;
	UINT32 m_TotalValue;
};

class TSpactivityFlow : public TRoleLog
{
public:
	TSpactivityFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_ActId = 0;
		m_TaskId = 0;
		m_State = 0;
		m_Progress = 0;
		m_FinishTaskCount = 0;
		m_TotalTaskCount = 0;
	}
	virtual ~TSpactivityFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TSpactivityFlow"; }

public:
	UINT32 m_ActId;
	UINT32 m_TaskId;
	UINT32 m_State;
	UINT32 m_Progress;
	UINT32 m_FinishTaskCount;
	UINT32 m_TotalTaskCount;
};

class TInvFightFlow : public TRoleLog
{
public:
	TInvFightFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_pRoleFight = NULL;
		 m_Result = 0;
	}
	virtual ~TInvFightFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TInvFightResultFlow"; }
	void SetFriendRole(Role* pRoleFight) {m_pRoleFight = pRoleFight;}

public:
	Role* m_pRoleFight;
	INT32 m_Result;
};

class TChangeProFlow : public TRoleLog
{
public:
	TChangeProFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_PrePro = 0;
		m_CurPro = 0;
	}
	virtual ~TChangeProFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TChangeProFlow"; }

public:
	INT32 m_PrePro;
	INT32 m_CurPro;
};

class TAtlasFlow : public TRoleLog
{
public:
	TAtlasFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_AtlasPowerPoint = 0;
		m_GroupId = 0;
		m_TeamId = 0;
		m_Num = 0;
		m_Round = 0;
		m_op = 0;
	}
	virtual ~TAtlasFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TAtlasFlow"; }

public:
	UINT32 m_AtlasPowerPoint;
	UINT32 m_GroupId;
	UINT32 m_TeamId;
	UINT32 m_Num;
	UINT32 m_Round;
	UINT32 m_op;//0：激活 1：下线登出记录;
};

class TLeagueBattleFlow
{
public:
	TLeagueBattleFlow(){}
	virtual ~TLeagueBattleFlow(){}

	void Do();
	void Build(LogHelper& roLog);
	const char* GetTableName() const { return "TLeagueBattleFlow"; }

public:
	UINT64 m_winLeagueId;
	UINT64 m_loseLeagueId;
	std::string m_strWinInfo;
	std::string m_strLoseInfo;
};

class TRoleChestFlow : public TRoleLog
{
public:
	TRoleChestFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_SuitId = 0;
		m_RealCount = 0;
		m_TotalCount = 0;
	}
	virtual ~TRoleChestFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TRoleChestFlow"; }

public:
	UINT32 m_SuitId;
	UINT32 m_RealCount;
	UINT32 m_TotalCount;
};

class TItemReturnFlow : public TRoleLog
{
public:
	TItemReturnFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_ItemId = 0;
		m_BackItemId = 0;
		m_BackItemNum = 0;
		m_ItemUid = 0;
	}
	virtual ~TItemReturnFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TItemReturnFlow"; }

public:
	UINT32 m_ItemId;
	UINT32 m_BackItemId;
	UINT32 m_BackItemNum;
	UINT64 m_ItemUid;
};

class TPkTwoFlow : public TRoleLog
{
public:
	TPkTwoFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_Group = 0;
		m_Point = 0;
		m_Damge = 0;
		m_BeHitDamage = 0;
		m_Result = 0;
	}
	virtual ~TPkTwoFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TPkTwoFlow"; }

public:
	UINT32 m_Group;
	UINT32 m_Point;
	UINT32 m_Result;
	UINT64  m_Damge;//伤害
	UINT64  m_BeHitDamage;//受到伤害;
};

class TArtifactFlow : public TRoleLog
{
public:
	TArtifactFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_ArtifactID = 0;
		m_QualityID = 0;
		m_EffectID = 0;
		m_Op = 0;
		m_ArtifactUid = 0;
		m_FuseResult = -1;
		m_FuseUsedStone = -1;
		m_InscriptionOp = -1;
		m_InscriptionSkillEffectID = 0;
	}
	virtual ~TArtifactFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TArtifactFlow"; }

public:
	UINT32 m_ArtifactID;
	UINT32 m_QualityID;
	UINT32 m_EffectID;
	INT32 m_Op;
	UINT64 m_ArtifactUid;
	INT32 m_FuseResult;//融合结果; 0:失败 1：成功
	INT32 m_FuseUsedStone;//融合是否使用石头; 0:未使用 1：使用
	INT32 m_InscriptionOp;//铭文技能方式 0:新增 1:替换;
	UINT32 m_InscriptionSkillEffectID;//铭文效果;
	std::vector<UINT32> m_EffectIDList;
	std::vector<TBaseAttr> m_AttrList;
};

class TLogoutBodyItem : public TRoleLog
{
public:
	TLogoutBodyItem(Role* poRole)
		:TRoleLog(poRole)
	{
		m_ItemID = 0;
		m_ItemLv = 0;
		m_ItemUid = 0;
		m_JadeSlotNum = 0;
		m_JadeTotalSlotNum = 0;
		 m_FuseLevel = 0;
		 m_FuseExp = 0;
	}
	virtual ~TLogoutBodyItem(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TLogoutBodyItem"; }

public:
	UINT32 m_ItemID;
	UINT32 m_ItemLv;
	UINT32 m_QualityID;
	UINT64 m_ItemUid;
	UINT32 m_JadeSlotNum;//已镶嵌个数
	UINT32 m_JadeTotalSlotNum;//可镶嵌空位数
	std::vector<UINT32> m_vJadeList;
	TItemAttr m_Attr;
	UINT32 m_FuseLevel;
	UINT32 m_FuseExp;
};


class TBigMeleeFightFlow : public TRoleLog
{
public:
	TBigMeleeFightFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_nGroupSvr= 0;
		m_nKiller = 0;
		m_nScore = 0;
		m_nDead = 0;
		m_nRank = 0;
		m_nJoinTime = 0;
	}
	virtual ~TBigMeleeFightFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TBigMeleeFightFlow"; }

public:
	UINT32 m_nGroupSvr; //赛区大区
	UINT32 m_nKiller; //击杀数
	UINT32 m_nScore; //积分数
	UINT32 m_nDead;//死亡数
	UINT32 m_nRank;//排名
	UINT32 m_nJoinTime;//参战时间
};

class TSpriteLogoutFlow : public TRoleLog
{
public:
	TSpriteLogoutFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_uid = 0;
		m_uExp = 0;
		m_spriteID = 0;
		m_skillID = 0;
		m_spritelevel = 0;
		m_evolutionLevel = 0;
		m_spritepowerpoint = 0;
		m_Quality = 0;
		m_IsCaptain = false;
	}
	virtual ~TSpriteLogoutFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TSpriteLogoutFlow"; }

public:
	UINT64 m_uid;
	UINT32 m_uExp;
	UINT32 m_spriteID;
	UINT32 m_skillID;
	UINT32 m_spritelevel;
	UINT32 m_evolutionLevel;
	UINT32 m_spritepowerpoint;
	UINT32 m_Quality;
	bool m_IsCaptain;
	std::vector<TBaseAttr> m_vAddAttrList;
	std::unordered_map<UINT32, double> m_attr;
	std::unordered_map<UINT32, double> m_evoAttr;
};

class TRoleHairFlow : public TRoleLog
{
public:
	TRoleHairFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_HairId = 0;
		m_HairColorId = 0;
	}
	virtual ~TRoleHairFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TRoleHairFlow"; }

public:
	UINT32 m_HairId;
	UINT32 m_HairColorId;
};

class TSharePhotoFlow : public TRoleLog
{
public:
	TSharePhotoFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_op = 0;
		m_type = 0;
	}

	virtual ~TSharePhotoFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TSharePhotoFlow"; }

public:
	INT32 m_op;
	INT32 m_type;
};


class TFashionComopseFlow : public TRoleLog
{
public:
	TFashionComopseFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_FashionId = 0;
		m_FashionId1 = 0;
		m_FashionId2 = 0;
		m_TotalSuccessRate = 0;
		m_FailRate = 0;
		m_Result = 0;
		m_FashionUid1 = 0;
		m_FashionUid2 = 0;
	}

	virtual ~TFashionComopseFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TFashionComopseFlow"; }

public:
	UINT32 m_FashionId;
	UINT32 m_FashionId1;
	UINT32 m_FashionId2;
	UINT32 m_TotalSuccessRate;//成功的概率
	UINT32 m_FailRate;//成功概率中其中(失败加成的概率);
	UINT32 m_Result;//0:成功 1:失败
	UINT64 m_FashionUid1;
	UINT64 m_FashionUid2;
};

//悬赏任务（周环）
class TWeekTaskFlow : public TRoleLog
{
public:
	TWeekTaskFlow(Role* poRole)
		:TRoleLog(poRole)
	{
		m_TaskId = 0;
		m_Quality = 0;
		m_Progress = 0;
		m_ChgValue = 0;
		m_TotalValue = 0;
		m_Op = 0;
		m_CompleteNum = 0;
		m_PreTaskId = 0;
		m_AskHelperNum = 0;
	}

	virtual ~TWeekTaskFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TWeekTaskFlow"; }

public:
	UINT32 m_TaskId;
	UINT32 m_Quality;
	UINT32 m_Progress;
	UINT32 m_MaxPreogress;
	UINT32 m_ChgValue;//悬赏修改值;
	UINT32 m_TotalValue;//悬赏总值;
	UINT32 m_Op;
	UINT32 m_CompleteNum;
	UINT32 m_PreTaskId;//刷新前的逻辑;
	UINT32 m_AskHelperNum;
};

/*----------分割线;--------*/
class TSecTalkFlow : public TCommonLog
{
public:
	explicit TSecTalkFlow(Role* poRole)
		:TCommonLog(poRole)
	{
		m_iChatType = 0;
		m_iMsgType = 0;
		m_iReceiverRoleType = 0;
		m_uReceiverRoleID = 0;
		m_iReceiverRoleLevel = 0;
		m_iSceneType = 0;
	}
	virtual ~TSecTalkFlow(){}
	virtual void Build(LogHelper& roLog);
	const char* GetTableName() const { return "SecTalkFlow";}

public:
	INT32 m_iChatType;
	INT32 m_iMsgType;
	INT32 m_iReceiverRoleType;
	INT32 m_iReceiverRoleLevel;
	INT64 m_uReceiverRoleID;
	INT32 m_iSceneType;
	std::string m_strTitleContents;
	std::string m_strChatContents;
	std::string m_strPicUrl;
	std::string m_strReceiverOpenID;
};

class TMobaBattleFlow : public TSimpleCommonLog
{
public:
	TMobaBattleFlow()
	{
		m_RoleId = 0;
		m_ServerId = GSConfig::Instance()->GetServerID();
		m_Level = 0;
		m_Profession = 0;
		m_HeroID = 0;
		m_TeamID = 0;
		m_KillCount = 0;
		m_DeathCount = 0;	
		m_AssistCount = 0;
		m_KillContinueCountMax = 0;
		m_MultiKillCountMax = 0;	
		m_MobaLevel = 0;	
		m_Damage = 0;
		m_BeHitDamage = 0;
		m_IsRunAway = false;
		m_Result = 0;
		m_MVP = 0;
		m_RoundTime = 0;	
	}
	virtual ~TMobaBattleFlow(){}
	virtual void Build(LogHelperBase& roLog);
	virtual const char* GetTableName() const { return "TMobaBattleFlow";}

public:
	UINT64 m_RoleId;
	UINT32 m_ServerId;//服务器id，该字段不需要额外赋值;
	UINT32 m_Level;//等级
	UINT32 m_Profession;//职业
	UINT32 m_HeroID;//选择的英雄id
	UINT32 m_TeamID;//队伍编号
	UINT32 m_KillCount;//击杀数
	UINT32 m_DeathCount;//死亡数	
	UINT32 m_AssistCount;//助攻数
	UINT32 m_KillContinueCountMax;//最大多杀
	UINT32 m_MultiKillCountMax;//最大连杀	
	UINT64 m_Damage;//总输出伤害;
	UINT64 m_BeHitDamage;//受到伤害
	bool m_IsRunAway;//是否逃跑
	UINT32 m_MobaLevel;//玩法等级
	UINT32 m_Result;//结果，0是败，1是胜
	UINT32 m_MVP;//0不是mvp，1是胜方mvp，2是败方mvp
	UINT32 m_RoundTime;//对局时长	
	std::string m_strOpenId;
	std::string m_strTransTag;//流水号
};

class TCustomPkResult
{
public:
	TCustomPkResult()
	{
		m_Damge = 0;
		m_BeHitDamage = 0;
		m_ServerId = 0;
		m_PlatID = 0;
		m_ProfessionID = 0;
		m_Level = 0;

		m_MatchId = 0;
		m_Matchtype = 0;
		m_MatchMode = 0;
		//m_ConfigId = 0;
		m_uRoleId = 0;
		m_isWin = 0;
		m_isSystem = 0;
		m_Score = 0;
		m_TotalScore = 0;
	}

	~TCustomPkResult(){}
	void BuildAndSend();
	const char* GetTableName() const { return "TCustomPkResult";}
	void SetRoleData(Role* role);
	void SetMatchData(UINT64 uid, INT32 type, INT32 isfair, INT32 issystem);
	void SetMatchResult(UINT64 roleid, INT32 iswin, INT32 score, INT32 allsocre);

public:
	//根据SetRoleData赋值
	UINT64 m_Damge;//伤害
	UINT64 m_BeHitDamage;//受到伤害;
	UINT32 m_ServerId;
	UINT32 m_PlatID;
	UINT32 m_ProfessionID;
	UINT32 m_Level;
	std::string m_strAppId;
	std::string m_strOpenId;

	//以下单独赋值
	UINT64 m_MatchId;//比赛ID///
	INT32 m_Matchtype;//比赛类型1:1v1, 2:2v2///
	INT32 m_MatchMode;//公平模式; 0:公平模式 1：非公平模式///
	INT32 m_isSystem;//是否系统比赛;//0:否 1：是
	//UINT32 m_ConfigId;//比赛配置ID;

	UINT64 m_uRoleId;
	INT32 m_isWin; // 0:输 1:赢
	INT32 m_Score;//本场积分;
	INT32 m_TotalScore;//总积分;
};

#endif