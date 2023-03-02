#ifndef _H_TLOGR_H__
#define _H_TLOGR_H__
#include "loghelper.h"
#include "util/jsonutil.h"
#include "guild/guildbonus.h"
#include "guildauct/guildauctbase.h"
#include "role/rolesummary.h"
#include "auction/auctionbase.h"
class TLogMgr
{
public:
	TLogMgr():m_iUniqueSeq(0),m_hLogTimer(INVALID_HTIMER){}
	~TLogMgr(){}
	DECLARE_SINGLETON(TLogMgr);

public:
	bool Init();
	void Uninit();
	void Logout(CRole* role);
	void DoDailyTaskLog(UINT64 roleid, UINT32 op, UINT64 helperRoleid, UINT32 prescore);
	std::string GetTagString();

private:
	INT32 m_iUniqueSeq;
	class TStateLogTimer : public ITimer
	{
	public:
		TStateLogTimer() {}
		virtual ~TStateLogTimer() {}

		virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	};

	HTIMER			m_hLogTimer;
	TStateLogTimer	m_oLogTimer;
};

class TCommonLog
{
public:
	explicit TCommonLog(std::string szTransTag) :
	m_szTransTag(szTransTag),m_iPlatID(-1),m_ServerID(MSConfig::Instance()->GetServerID()),m_uRoleID(0){}
	virtual ~TCommonLog() {}

	void Do();
	virtual void Build(LogHelper& roLog) {}
	virtual const char* GetTableName() const { return ""; }
public:
	std::string m_szTransTag;//流水号;
	INT32 m_iPlatID;
	UINT32 m_ServerID;
	UINT64 m_uRoleID;
};
class TCommonFlowLog
{
public:
	explicit TCommonFlowLog(){}
	virtual ~TCommonFlowLog(){};

	void Do();
	virtual void Build(LogHelper& roLog) {}
	virtual const char* GetTableName() const { return ""; }
};
class TGuildBossFlow : public TCommonLog
{
public:
	explicit TGuildBossFlow(std::string szTransTag)
		:TCommonLog(szTransTag)
	{
		m_uGuildID = 0;
		m_Damage = 0;
	}
	virtual ~TGuildBossFlow(){}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "GuildBossFlow"; }
public:
	UINT64 m_uGuildID;
	UINT64 m_Damage;
	std::string m_szOpenID;
};

class TGuildFlowLog : public TCommonLog
{
public:
	explicit TGuildFlowLog(std::string szTransTag)
		:TCommonLog(szTransTag)
	{
		m_iLevel = 0;
		m_iVipLevel = 0;
		m_iActType = 0;
		m_iGuildLevel = 0;
		m_iMemberNum = 0;
		m_uGuildID = 0;
		m_iPrestige = 0;
		m_iWageLevel = 0;
		m_iChangeValue = 0;
		m_iGuildExp = 0;
		m_iWeekActivity = 0;
		m_RoleIDT = 0;
		m_DismissMode = -1;
		m_CreateTime = 0;
	}
	virtual ~TGuildFlowLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "GuildFlow"; }
public:
	INT32	m_iLevel;
	INT32	m_iVipLevel;		//VIP等级
	INT32	m_iActType;			//操作类型
	INT32	m_iGuildLevel;		//公会等级
	INT32	m_iMemberNum;		//操作后公会人数(解散时成员为0)
	UINT64	m_uGuildID;
	INT32	m_iPrestige;
	INT32   m_iWageLevel;
	INT32   m_iChangeValue;
	INT32	m_iGuildExp;
	INT32   m_iWeekActivity;
	std::string m_szOpenID;
	UINT64 m_RoleIDT;//发起T人的角色id
	INT32 m_DismissMode;//解散方式 0：主动 1：系统
	INT32 m_CreateTime;
	std::string m_strName;
};
class TGuildDareFlow : public TCommonLog
{
public:
	explicit TGuildDareFlow(std::string szTransTag)
		:TCommonLog(szTransTag)
	{
		m_uGuildID = 0;
		m_iGuildLevel = 0;
		m_iDareNum = 0;
		m_iAfterDareNum = 0;
	}
	virtual ~TGuildDareFlow() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "GuildDareFlow"; }

public:
	UINT64 m_uGuildID;
	INT32 m_iGuildLevel;
	INT32 m_iDareNum;
	INT32 m_iAfterDareNum;
};

class TRegisterLog : public TCommonLog
{
public:
	explicit TRegisterLog(std::string szTransTag)
		:TCommonLog(szTransTag)
	{
	}
	virtual ~TRegisterLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "PlayerRegister"; }
public:
	std::string m_szOpenID;
	KKSG::ClientInfo m_stConf;
};

class TAccountLogin : public TCommonLog
{
public:
	explicit TAccountLogin(std::string szTransTag)
		:TCommonLog(szTransTag)
	{}

	virtual ~TAccountLogin() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "AccountLogin"; }
public:

	std::string m_szOpenID;
};

class TGuildCardMatch : public TCommonLog
{
public:
	explicit TGuildCardMatch(std::string szTransTag)
		:TCommonLog(szTransTag)
	{
		m_uGuildID = 0;
		m_uRound = 0;
		m_iResult = -1;
		m_iOp = -1;
	}
	virtual ~TGuildCardMatch() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "GuildCardMatch"; }
public:
	UINT64 m_uGuildID;
	UINT32 m_uRound;
	INT32  m_iResult;
	INT32  m_iOp;
	std::string m_strOpenId;

};

class TGardenFlow : public TCommonLog
{
public:
	explicit TGardenFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_uGardenID = 0;
		m_uFishLevel = 0;
		m_uFishExp = 0;
		m_uCookLevel = 0;
		m_uCookExp = 0;
		m_uSeedID = 0;
		m_uBanquetID = 0;
		m_uCookBookID = 0;
		m_uFoodID = 0;
		m_iResult = 0;
		m_iOp = -1;
		m_iQuestType = KKSG::MYSELF;
	}
	virtual ~TGardenFlow(){}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "GardenFlow"; }

public:
	UINT64 m_uGardenID;//家园ID
	UINT32 m_uFishLevel;//钓鱼等级
	UINT32 m_uFishExp;	//钓鱼经验
	UINT32 m_uCookLevel;//烹饪等级
	UINT32 m_uCookExp;	//烹饪经验
	UINT32 m_uSeedID;	//种子ID;
	UINT32 m_uBanquetID;//宴会ID
	UINT32 m_uCookBookID;//菜谱ID
	UINT32 m_uFoodID;	//食物ID
	INT32  m_iResult;//结果0：成功 1：失败
	INT32  m_iOp;//行为类别
	INT32  m_iQuestType; //请求类别
};

class TGuildBonusFlow : public TCommonLog
{
public:
	explicit TGuildBonusFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_iGetCount = 0;
		m_iGetAwardNum = 0;
		m_iOp = 0;
	}
	virtual ~TGuildBonusFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "GuildBonusFlow";}

public:
	GuildBonusInfo oBonusInfo;
	INT32 m_iGetCount;
	INT32 m_iGetAwardNum;
	INT32 m_iOp;//0产生红包 1领取红包
};

class TFlowerFlow : public TCommonLog
{
public:
	explicit TFlowerFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_TotalNum = 0;
		m_TodayNum = 0;
		m_YesNum = 0;
		m_ThisWeekNum = 0;
	}
	virtual ~TFlowerFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "FlowerFlow";}

public:
	UINT32 m_TotalNum;
	UINT32 m_TodayNum;
	UINT32 m_YesNum;
	UINT32 m_ThisWeekNum;
};

class TFriendFlow : public TCommonLog
{
public:
	explicit TFriendFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_Roleid2 = 0;
		m_Op = 0;
		m_FriendNum = 0;
	}
	virtual ~TFriendFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "FriendFlow";}

public:
	UINT64 m_Roleid2;
	INT32 m_Op;
	INT32 m_FriendNum;
};
class TGuildAuctionFlow : public TCommonLog
{
public:
	explicit TGuildAuctionFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_uGuildID = 0;
		m_uUID = 0;
		m_uItemID = 0;
		m_uItemCount = 0;
		m_uAuctRoleID = 0;
		m_uAuctPrice = 0;
		m_uMaxPrice = 0;
		m_uProfitNum = 0;
		m_iOperType = 0;
		m_iActType = 0;
	}

	virtual ~TGuildAuctionFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "GuildAuctionFlow"; }
	void SetData(GABaseData* data);
public:
	UINT64 m_uGuildID;
	UINT64 m_uUID;
	UINT64 m_uItemID;
	UINT32 m_uItemCount;
	UINT64 m_uAuctRoleID;
	UINT32 m_uAuctPrice;
	UINT32 m_uMaxPrice;
	UINT32 m_uProfitNum;
	INT32 m_iOperType;
	INT32 m_iActType;
};

class TGuildRankFlow : public TCommonLog
{
public:
	explicit TGuildRankFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_GuildID = 0;
		m_GuildLevel = 0;
		m_Rank = 0;
		m_Prestige = 0;
	}
	virtual ~TGuildRankFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TGuildRankFlow";}

public:
	UINT64 m_GuildID;
	UINT32 m_GuildLevel;
	UINT32 m_Rank;
	UINT32 m_Prestige;
	std::string m_strName;
};
class TFlowerRankFlow : public TCommonLog
{
public:
	explicit TFlowerRankFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_Num = 0;
		m_Type = 0;
		m_Rank = 0;
	}
	virtual ~TFlowerRankFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TFlowerRankFlow";}

public:
	UINT32 m_Num;
	UINT32 m_Type;
	UINT32 m_Rank;
	std::string m_strOpenId;
};

class TFirstPassFlow : public TCommonLog
{
public:
	explicit TFirstPassFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_SceneID= 0;
		m_ID = 0;
		m_Rank = 0;
		m_StarLv= 0;
	}
	virtual ~TFirstPassFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TFirstPassRankFlow"; }
	void SetData(std::vector<CRoleSummary*> &vList);

public:
	UINT32 m_SceneID;
	UINT32 m_ID;
	UINT32 m_Rank;
	UINT32 m_StarLv;
	std::string m_strTeam;
};

class TInvFightFlow : public TCommonLog
{
public:
	explicit TInvFightFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_FromId= 0;
		m_ToId = 0;
		m_FromLevel = 0;
		m_FromProfession = 0;
		m_FromPpt = 0;
		m_ToLevel= 0;
		m_ToProfession = 0;
		m_ToPpt = 0;
		m_InvID = 0;
		m_Type = 0;
	}
	virtual ~TInvFightFlow() {}
	virtual void Build(LogHelper& roLog);
	void SetData(const KKSG::InvFightBefESpara& oData) {m_Data = oData;}
	virtual const char* GetTableName() const { return "TInvFightFlow"; }

public:
	UINT64 m_FromId;
	UINT64 m_ToId;
	UINT32 m_FromLevel;
	UINT32 m_FromProfession;
	UINT32 m_FromPpt;
	UINT32 m_ToLevel;
	UINT32 m_ToProfession;
	UINT32 m_ToPpt;
	UINT64 m_InvID;
	UINT32 m_Type;
	std::string m_strFromOpenId;
	std::string m_strToOpenId;
	KKSG::InvFightBefESpara m_Data;
};
class TPayItem : public TCommonLog
{
public:
	TPayItem(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_FromRoleId = 0;
		m_ToRoleId = 0;
		m_Count = 0;
		m_PayDegree = 0;
	}
	virtual ~TPayItem() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TPayItemFlow"; }

public:
	UINT64 m_FromRoleId;
	UINT64 m_ToRoleId;
	INT32 m_Count;
	INT32 m_PayDegree;
	std::string m_FromOpenId;
	std::string m_ToOpenId;
	std::string m_GoodsId;
};

class TMailFlow : public TCommonLog
{
public:
	TMailFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_Roleid = 0;
		m_Op = -1;
	}
	virtual ~TMailFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TMailFlow"; }

public:
	UINT64 m_Roleid;
	INT32 m_Op;//0:收到邮件 1:提取邮件;
	std::string m_strOpenid;
	KKSG::SMail m_Data;
};

class TLeagueTeamFlow : public TCommonLog
{
public:
	TLeagueTeamFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_TeamId = 0;
		m_Type = 0;
		m_Num = 0;
	}
	virtual ~TLeagueTeamFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TLeagueTeamFlow"; }

public:
	UINT64 m_TeamId;
	INT32 m_Type;
	INT32 m_Num;
	std::vector<UINT64> m_roleIds;
	std::string m_strName;
};

class TMatchFlow : public TCommonLog
{
public:
	TMatchFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_matchID = 0;
		m_teamID = 0;
		m_Type = 0;
		m_Time = 0;
	}
	virtual ~TMatchFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TMatchFlow"; }

public:
	UINT32 m_matchID;
	INT32 m_teamID;
	INT32 m_Type;
	INT32 m_Time;
};

class TMsLogoutFlow : public TCommonLog
{
public:
	TMsLogoutFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_GuildId = 0;
		m_GuildLv = 0;
		m_GuildMemCnt = 0;
		m_GuildPrestige = 0;
		m_GuildActivity = 0;
		m_Rank = 0;
		m_PartnerID = 0;
		m_PartnerDegree = 0;
		m_CoupleID = 0;
		m_LiveNess = 0;
		m_DragonGuildMemCount = 0;
		m_DragonGuildId = 0;
	}
	virtual ~TMsLogoutFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TMsLogoutFlow"; }

public:
	UINT64 m_GuildId;
	UINT32 m_GuildLv;
	UINT32 m_GuildMemCnt;
	UINT32 m_GuildPrestige;
	UINT32 m_GuildActivity;
	UINT32 m_Rank;
	UINT64 m_PartnerID;
	UINT32 m_PartnerDegree;
	UINT64 m_CoupleID;//伴侣roleid;
	UINT32 m_LiveNess;//伴侣活跃度;
	UINT32 m_DragonGuildMemCount;//龙本小分队人数;
	UINT64 m_DragonGuildId;//龙本小分队id;
	std::string m_DragonGuildName;
	std::string m_strOpenid;
	std::string m_strGuildName;
};

class TFmFlow :  public TCommonLog
{
public:
	TFmFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_op = -1;
		m_strChat = "";
		m_strOpenId = "";
	}
	virtual ~TFmFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TFmFlow"; }

public:
	INT32 m_op; //0:join 1 leave 2 主播收到消息;
	std::string m_strOpenId;
	std::string m_strChat;
};

class TGuildCastleFlow : public TCommonLog
{
public:
	TGuildCastleFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_GuildId = 0;
		m_GuildId2 = 0;
		m_allianceId = 0;
		m_TerritoryID = 0;
		m_Result = 0;
	}
	virtual ~TGuildCastleFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TGuildCastleFlow"; }

public:
	UINT64 m_GuildId;
	UINT64 m_GuildId2;
	UINT64 m_allianceId;
	UINT32 m_TerritoryID;
	INT32 m_Result;//0输了未占领 1：赢了未占领 2：赢了占领
	std::string m_strGuildName;
	std::string m_strGuildName2;
};


class TGuildCastleRoleFlow : public TCommonLog
{
public:
	TGuildCastleRoleFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_RoleID    = 0;
		m_KillCount = 0;	//击杀数	
		m_ZlCount	= 0;	//占领数
		m_Feats		= 0;	//功勋值
	}
	virtual ~TGuildCastleRoleFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TGuildCastleRoleFlow"; }

public:
	UINT64 m_RoleID;
	UINT32 m_KillCount;	//个人击杀数
	UINT32 m_ZlCount;		//工会占领数
	UINT32 m_Feats;		//个人功勋值
};


class TPlatformFriendFlow : public TCommonLog
{
public:
	TPlatformFriendFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_RoleId2 = 0;
	}

	virtual ~TPlatformFriendFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TPlatformFriendFlow"; }

public:

	UINT64 m_RoleId2;
	std::string m_strOpenId;
	std::string m_strOpenId2;
	std::string m_strNickName;
	std::string m_strBigPic;
	std::string m_strMidPic;
	std::string m_strSmallPic;
	std::string m_strNickName2;
	std::string m_strBigPic2;
	std::string m_strMidPic2;
	std::string m_strSmallPic2;
};

class TWorldBossFlow : public TCommonLog
{
public:
	TWorldBossFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_Damge = 0.0;
		m_Level = 0;
		m_Pro = 0;
		m_PPt = 0;
	}

	virtual ~TWorldBossFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TWorldBossFlow"; }
	void SetData(CRoleSummary* data);

public:
	float m_Damge;
	UINT32 m_Level;
	UINT32 m_Pro;
	UINT32 m_PPt;
	UINT32 m_Rank;
	std::string m_strOpenId;
};

class TMentorFlow : public TCommonLog
{
public:
	TMentorFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_RoleID2 = 0;
		m_TaskId = 0;
		m_op = -1;
	}

	virtual ~TMentorFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TMentorFlow"; }

public:
	UINT64 m_RoleID2;
	INT32 m_TaskId;
	INT32 m_op;//0:拜师 1：收徒 2：解除师徒(徒弟) 3：解除师徒（师父） 4：完成师徒任务 5：出师
	std::string m_strOpenId;
	std::string m_strOpenId2;
};

class TRoleReportFlow : public TCommonLog
{
public:
	TRoleReportFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_SceneType = 0;
		m_Num = 0;
		m_ReportNum = 0;
		m_RoleGroup = 0;
		m_RoleGroup2 = 0;
		m_RoleId2 = 0;
		m_op = 0;
	}

	virtual ~TRoleReportFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TRoleReportFlow"; }

public:
	INT32 m_SceneType;
	INT32 m_Num;
	INT32 m_op;
	UINT32 m_ReportNum;
	UINT32 m_RoleGroup;
	UINT32 m_RoleGroup2;
	UINT64 m_RoleId2;
	std::string m_strOpenId;
	std::string m_strOpenId2;
	std::string m_strReason;
};

class TRoleReportBanFlow : public TCommonLog
{
public:
	TRoleReportBanFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_SceneType = 0;
		m_Time = 0;
		m_ReportNum = 0;
		m_BanEndTime = 0;
	}

	virtual ~TRoleReportBanFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TRoleReportBanFlow"; }

public:
	INT32 m_SceneType;
	UINT32 m_Time;
	UINT32 m_ReportNum;
	UINT32 m_BanEndTime;
	std::string m_strOpenId;
};

class TCustomPkFlow : public TCommonLog
{
public:
	TCustomPkFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_MatchId = 0;
		m_Op = -1;
		m_Matchtype = 0;
		m_MatchMode = -1;
		m_ConfigId = 0;
		m_isSystem = -1;
	}

	virtual ~TCustomPkFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TCustomPkFlow"; }

	void BuildCustom(const KKSG::CustomBattleConfig& config);

public:
	//创建比赛和报名的时候记录;
	//父类中的roleid必须赋值;
	UINT64 m_MatchId;//比赛ID///
	INT32 m_Op;//行为类别  0：创建比赛 1：报名比赛 2: 退出比赛
	INT32 m_Matchtype;//比赛类型1v1,2v2///
	INT32 m_MatchMode;//公平模式; 0:公平模式 1：非公平模式///
	UINT32 m_ConfigId;//比赛配置ID;
	INT32 m_isSystem;//是否系统比赛;//0:否 1：是
	std::string m_strOpenId;//帐号;//
	std::string m_strMatchName;//比赛命名名称;
};

class TMobaEloFlow : public TCommonLog
{
public:
	TMobaEloFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_Elo = 0;
		m_NewElo = 0;
	}

	virtual ~TMobaEloFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TMobaEloFlow"; }

public:
	//如需增加字段后面自行补上;
	UINT64 m_Elo;
	UINT64 m_NewElo;
	std::string m_strOpenId;//openid不需要赋值，roleid赋值即可;
};

class TAuctionExpireFlow : public TCommonLog
{
public:
	TAuctionExpireFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_AuctionID = 0;
		m_ItemUID = 0;
		m_ItemID = 0;
		m_ItemCount = 0;
		m_DealPrice = 0;
		m_DealPriceTotal = 0;
		m_Op = 0;
	}

	virtual ~TAuctionExpireFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TAuctionExpireFlow"; }
	void SetData(AuctionItem* pSaleItem);

public:
	UINT64 m_AuctionID;
	UINT64 m_ItemUID;
	UINT32 m_ItemID;
	UINT32 m_ItemCount;
	UINT32 m_DealPrice;
	UINT32 m_DealPriceTotal;
	UINT32 m_OnSaleTime;
	UINT32 m_Op;//0:过期 1：过期删除;
	std::string m_strOpenId;
};

class TMarriageFlow : public TCommonLog
{
public:
	TMarriageFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_RoleID2 = 0;
		m_Op = 0;
		m_WeddingType = 0;
		m_WeddingID = 0;
	}

	virtual ~TMarriageFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TMarriageFlow"; }

public:
	UINT64 m_RoleID2;
	UINT64 m_WeddingID;
	UINT32 m_Op;
	UINT32 m_WeddingType;

};

class TWeddingFlow : public TCommonLog
{
public:
	TWeddingFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_RoleID1 = 0;
		m_RoleID2 = 0;
		m_WeddingType = 0;
		m_WeddingID = 0;
	}

	virtual ~TWeddingFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TWeddingFlow"; }

public:
	//玩家roleid参加roleid1和roleid2的婚礼;
	UINT64 m_RoleID1;
	UINT64 m_RoleID2;
	UINT64 m_WeddingID;
	UINT32 m_WeddingType;
};

class TDragonGuildFlowLog : public TCommonLog
{
public:
	explicit TDragonGuildFlowLog(std::string szTransTag)
		:TCommonLog(szTransTag)
	{
		m_iLevel = 0;
		m_iActType = 0;
		m_iDGuildLevel = 0;
		m_iMemberNum = 0;
		m_uDGuildID = 0;
		m_iPrestige = 0;
		m_iChangeValue = 0;
		m_iDGuildExp = 0;
		m_iWeekActivity = 0;
		m_RoleIDT = 0;
		m_DismissMode = -1;
		m_CreateTime = 0;
	}
	virtual ~TDragonGuildFlowLog() {}

	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "DragonGuildFlow"; }
public:
	INT32	m_iLevel;
	INT32	m_iActType;			//操作类型
	INT32	m_iDGuildLevel;		//公会等级
	INT32	m_iMemberNum;		//操作后公会人数(解散时成员为0)
	UINT64	m_uDGuildID;
	INT32	m_iPrestige;
	INT32   m_iChangeValue;
	INT32	m_iDGuildExp;
	INT32   m_iWeekActivity;
	std::string m_szOpenID;
	UINT64 m_RoleIDT;			//发起T人的角色id
	INT32 m_DismissMode;		//解散方式	0：主动 1：系统
	INT32 m_CreateTime;
	std::string m_strName;
};

class TIssueFlow : public TCommonLog
{
public:
	TIssueFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_GroupID = 0;
		m_showStageID = 0;
		m_showFighting = 0;
		m_showFightType = 0;
		m_showTime = 0;
	}

	virtual ~TIssueFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TIssueFlow"; }

public:
	UINT64 m_GroupID;
	UINT32 m_showStageID;//副本id;
	UINT32 m_showFighting;//战力;
	UINT32 m_showFightType;//擅长类型;
	UINT32 m_showTime;//时间段;
};

class TChatGroupFlow : public TCommonLog
{
public:
	TChatGroupFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_LeaderID = 0;
		m_GroupID = 0;
		m_Op = 0;
	}

	virtual ~TChatGroupFlow() {}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TChatGroupFlow"; }

public:
	UINT64 m_LeaderID;
	UINT64 m_GroupID;
	UINT32 m_Op;
};

class TRoleBaseInfo
{
public:
	TRoleBaseInfo();
	~TRoleBaseInfo(){};

public:
	void Init();
	void GetData(UINT64 roleid);
	UINT32 GetLevel() {return m_Level;}
	UINT32 GetPowerPoint() { return m_PowerPoint;}
	UINT32 GetPro() {return m_Pro;}
	const std::string& GetAccount() { return m_strOpenId;}
private:
	UINT32 m_Level;
	UINT32 m_PowerPoint;
	UINT32 m_Pro;
	std::string m_strOpenId;
};

class TGoalAwardFlow : public TCommonLog
{
public:
	TGoalAwardFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_AwardID = 0;
		m_AwardIndex = 0;
		m_AwardType = 0;
		m_op = 0;
	}

	virtual ~TGoalAwardFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TGoalAwardFlow"; }

public:
	//m_uRoleID需要赋值;
	UINT32 m_AwardID;
	UINT32 m_AwardIndex;
	UINT32 m_AwardType;
	UINT32 m_op;//0：完成 1：已领取;
};

class TDailyTaskFlow : public TCommonLog
{
public:
	TDailyTaskFlow(std::string szTranstag)
		:TCommonLog(szTranstag)
	{
		m_score = 0;
		m_prescore = 0;
		m_op = 0;
		m_HelperRoleId = 0;
	}

	virtual ~TDailyTaskFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "TDailyTaskReFreshFlow"; }

public:
	UINT32 m_score;
	UINT32 m_prescore;
	UINT32 m_op;
	UINT64 m_HelperRoleId;
};
///---------
class TBlackListFlow : public TCommonFlowLog
{
public:
	explicit TBlackListFlow():m_iAreaID(-1),m_iGetMode(-1),m_ServerID(MSConfig::Instance()->GetServerID()){}
	virtual ~TBlackListFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "GetblackListFlow"; }

public:
	INT32 m_iAreaID;
	INT32 m_iGetMode;
	UINT32 m_ServerID;
	std::string m_strOpenId;
	std::string m_strPlayerOpenId;
};


class TAuctionStartFlow : public TCommonFlowLog
{
public:
	explicit TAuctionStartFlow():
	m_iAreaID(0),
	m_lRoleUid(0),
	m_iRoleJobID(0),
	m_iRoleLevel(0),
	m_iUseVIP(0),
	m_lAuctionID(0),
	m_iAuctionPriceType(0),
	m_iAuctionPrice1(0),
	m_iAuctionCount(0),
	m_iAuctionItemID(0),
	m_lAuctionUUID(0),
	m_iAuctionLevel(0),
	m_iAuctionQlty(0),
	m_iAuctionTime(0){}
	virtual ~TAuctionStartFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "AuctionStartFlow"; }

public:
	INT32 m_iAreaID;
	INT64 m_lRoleUid;
	INT32 m_iRoleJobID;
	INT32 m_iRoleLevel;
	INT32 m_iUseVIP;
	INT64 m_lAuctionID;
	INT32 m_iAuctionPriceType;
	INT32 m_iAuctionPrice1;
	INT32 m_iAuctionCount;
	INT32 m_iAuctionItemID;
	UINT64 m_lAuctionUUID;
	INT32 m_iAuctionLevel;
	INT32 m_iAuctionQlty;
	INT32 m_iAuctionTime;
	std::string m_strOpenId;
	std::string m_strUserIP;

};
class TAuctionEndFlow : public TCommonFlowLog
{
public:
	explicit TAuctionEndFlow():
	m_iAreaID(0),
	m_lBuyerRoleUid(0),
	m_iBuyerRoleJobID(0),
	m_iBuyerRoleLevel(0),
	m_iBuyerRoleBattlePoint(0),
	m_iBuyerUseVIP(0),
	m_lSellerRoleUid(0),
	m_iSellerRoleJobID(0),
	m_iSellerRoleLevel(0),
	m_iDealMainType(0),
	m_iDealPriceType(0),
	m_iDealCount(0),
	m_iDealPrice(0),
	m_iDealPriceTotal(0),
	m_iDealItemID(0),
	m_lDealUUID(0),
	m_iDealLevel(0),
	m_iDealQlty(0),
	m_lDealAuctionID(0){} 
	virtual ~TAuctionEndFlow(){}
	virtual void Build(LogHelper& roLog);
	virtual const char* GetTableName() const { return "AuctionEndFlow"; }

public:
	INT32 m_iAreaID;
	INT64 m_lBuyerRoleUid;
	INT32 m_iBuyerRoleJobID;
	INT32 m_iBuyerRoleLevel;
	INT32 m_iBuyerRoleBattlePoint;
	INT32 m_iBuyerUseVIP;
	INT64 m_lSellerRoleUid;
	INT32 m_iSellerRoleJobID;
	INT32 m_iSellerRoleLevel;
	INT32 m_iDealMainType;
	INT32 m_iDealPriceType;
	INT32 m_iDealCount;
	INT32 m_iDealPrice;
	INT32 m_iDealPriceTotal;
	INT32 m_iDealItemID;
	UINT64 m_lDealUUID;
	INT32 m_iDealLevel;
	INT32 m_iDealQlty;
	INT64 m_lDealAuctionID;
	std::string m_strBuyerUserIP;
	std::string m_strBuyerOpenID;
	std::string m_strSellerUserIP;
	std::string m_strSellerOpenID;
};
//IDIP tlog
class TIdipCommonLog
{
public:
	TIdipCommonLog(std::string szTableName) : m_szTableName(szTableName) {}
	virtual ~TIdipCommonLog() {}

	void Do(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson);
	virtual void Build(LogHelper& roLog) {}
	virtual const char* GetTableName() const { return m_szTableName.c_str(); }

protected:
	std::string m_szTableName;
};

class TIdipMoneyLog : public TIdipCommonLog
{
public:
	TIdipMoneyLog(std::string szName) 
		:TIdipCommonLog(szName)
		,m_uRoleId(0)
		,m_nNum(0){}
	virtual ~TIdipMoneyLog() {}
	virtual void Build(LogHelper& roLog);

public:
	UINT64 m_uRoleId;
	INT64  m_nNum;
	std::string szOpenId;
};

class TIdipDoGmMoneyLog : public TIdipCommonLog
{
public:
	TIdipDoGmMoneyLog(std::string szName) 
		:TIdipCommonLog(szName)
		,m_nNum(0){}
	virtual ~TIdipDoGmMoneyLog() {}
	virtual void Build(LogHelper& roLog);

public:
	INT64  m_nNum;
};

class TIdipDelItemLog : public TIdipCommonLog
{
public:
	TIdipDelItemLog(std::string szName) 
		:TIdipCommonLog(szName)
		,m_uItemID(0)
		,m_nNum(0){}
	virtual ~TIdipDelItemLog() {}
	virtual void Build(LogHelper& roLog);

public:
	UINT32 m_uItemID;
	INT64 m_nNum;
	UINT64 m_uRoleId;
	std::string szOpenId;
};

class TIdipSendItemLog : public TIdipCommonLog
{
public:
	TIdipSendItemLog(std::string szName) 
		:TIdipCommonLog(szName)
		,m_uItemID(0)
		,m_nNum(0)
		,m_uBind(0)
		,m_uRoleId(0)
		,EndTime(0){}
	virtual ~TIdipSendItemLog() {}
	virtual void Build(LogHelper& roLog);

public:
	UINT32 m_uItemID;
	INT64 m_nNum;
	UINT32 m_uBind;
	UINT64 m_uRoleId;
	INT32 EndTime;
	std::string szOpenId;
};

class TIdipDoGmSendItemLog : public TIdipCommonLog
{
public:
	TIdipDoGmSendItemLog(std::string szName) 
		:TIdipCommonLog(szName)
		,m_uItemID(0)
		,m_nNum(0)
		,m_uBind(0){}
	virtual ~TIdipDoGmSendItemLog() {}
	virtual void Build(LogHelper& roLog);

public:
	UINT32 m_uItemID;
	INT64 m_nNum;
	UINT32 m_uBind;
	std::string EndTime;
};

class TIdipAqDoInitialAccount : public TIdipCommonLog
{
public:
	TIdipAqDoInitialAccount(std::string szName)
		:TIdipCommonLog(szName),m_uRoleId(0){}
	virtual ~TIdipAqDoInitialAccount() {}
	virtual void Build(LogHelper& roLog);

public:
	UINT64 m_uRoleId;
	std::string m_strOpenId;
};

class TIdipBanRank : public TIdipCommonLog
{
public:
	TIdipBanRank(std::string szName)
		:TIdipCommonLog(szName)
	,m_uClearRanking(0)
	,m_uRankingType(0)
	,m_iBanTime(0)
	,m_uRoleId(0){}
	virtual ~TIdipBanRank() {}
	virtual void Build(LogHelper& roLog);

public:
	UINT32 m_uClearRanking;
	UINT32 m_uRankingType;
	UINT32 m_iBanTime;
	UINT64 m_uRoleId;
	std::string m_strOpenId;
	std::string m_strBanReason;
};

class TIdipBanPlay : public TIdipCommonLog
{
public:
	TIdipBanPlay(std::string szName)
		:TIdipCommonLog(szName)
	,m_iBanTime(0)
	,m_uBanType(0)
	,m_uRoleId(0){}
	virtual ~TIdipBanPlay() {}
	virtual void Build(LogHelper& roLog);

public:
	INT32 m_iBanTime;
	UINT32 m_uBanType;
	UINT64 m_uRoleId;
	std::string m_strOpenId;
	std::string m_strPromptText;
};

class TIdipPunish : public TIdipCommonLog
{
public:
	TIdipPunish(std::string szName)
		:TIdipCommonLog(szName)
	,m_iBanTime(0)
	,m_uRoleId(0){}
	virtual ~TIdipPunish() {}
	virtual void Build(LogHelper& roLog);

public:
	INT32  m_iBanTime;
	UINT64 m_uRoleId;
	std::string m_strOpenId;
	std::string m_strBanReason;
};

class TIdipRelievePunish : public TIdipCommonLog
{
public:
	TIdipRelievePunish(std::string szName)
		:TIdipCommonLog(szName)
	,m_uRelieveZeroProfit(0)
	,m_uRelievePlayAll(0)
	,m_uRelieveBanJoinRank(0)
	,m_uRelieveBan(0)
	,m_uRelieveMaskchat(0)
	,m_uRoleId(0){}
	virtual ~TIdipRelievePunish(){}
	virtual void Build(LogHelper& roLog);

public:
	UINT32 m_uRelieveZeroProfit;
	UINT32 m_uRelievePlayAll;
	UINT32 m_uRelieveBanJoinRank;
	UINT32 m_uRelieveBan;
	UINT32 m_uRelieveMaskchat;
	UINT64 m_uRoleId;
	std::string m_strOpenId;
};

class TIdipDoUnbanUsr : public TIdipCommonLog
{
public:
	TIdipDoUnbanUsr(std::string szName)
		:TIdipCommonLog(szName)
	,m_uRoleId(0){}
	virtual ~TIdipDoUnbanUsr(){}
	virtual void Build(LogHelper& roLog);

public:
	UINT64 m_uRoleId;
	std::string m_strOpenId;
};

class TIdipZeroProfit : public TIdipCommonLog
{
public:
	TIdipZeroProfit(std::string szName)
		:TIdipCommonLog(szName)
	,m_iZeroProfitTime(0)
	,m_uRoleId(0){}
	virtual ~TIdipZeroProfit(){}
	virtual void Build(LogHelper& roLog);

public:
	INT32 m_iZeroProfitTime;
	UINT64 m_uRoleId;
	std::string m_strOpenId;
	std::string m_strReason;
};

class TIdipUpdateScore : public TIdipCommonLog
{
public:
	TIdipUpdateScore(std::string szName)
		:TIdipCommonLog(szName)
	{
		m_uRoleId = 0;
		m_iScore = 0;
		m_iScoreType = 0;
	}
	virtual ~TIdipUpdateScore(){}
	virtual void Build(LogHelper& roLog);

public:
	UINT64 m_uRoleId;
	INT32 m_iScore;
	INT32 m_iScoreType;
	std::string m_strOpenId;
};

class TIdipDoDelItemOutBag : public TIdipCommonLog
{
public:
	TIdipDoDelItemOutBag(std::string szName)
		:TIdipCommonLog(szName)
	{
		m_Type = 0;
		m_ItemID = 0;
		m_ItemUid = 0;
		m_RoleId = 0;
		m_JadeId = 0;
	}
	virtual ~TIdipDoDelItemOutBag(){}
	virtual void Build(LogHelper& roLog);

public:
	INT32 m_Type;
	UINT32 m_ItemID;
	UINT64 m_ItemUid;
	UINT64 m_RoleId;
	INT32 m_JadeId;
	std::string m_strOpenId;
};

class TIdipXinYueRPay : public TIdipCommonLog
{
public:
	TIdipXinYueRPay(std::string szName)
		:TIdipCommonLog(szName)
	{

	}
	virtual ~TIdipXinYueRPay(){}
	virtual void Build(LogHelper& roLog);

public:
	UINT64 m_RoleId;
	UINT32 m_Num;
	std::string m_strMailTitle;
	std::string m_strMailContent;
	std::string m_strExtParam;
	std::string m_strOpenId;
};

class TIdipItemChange : public TIdipCommonLog
{
public:
	TIdipItemChange(std::string szName)
		:TIdipCommonLog(szName)
	{
		m_ItemId = 0;
		m_ItemNum = 0;
		m_RoleId = 0;
		m_Op = -1;
	}
	virtual ~TIdipItemChange(){}
	virtual void Build(LogHelper& roLog);

public:
	UINT32 m_ItemId;
	UINT32 m_ItemNum;
	UINT64 m_RoleId;
	INT32 m_Op;//0:加 1:扣
	std::string m_strOpenId;
};

class TIdipUpdateGuildInfo : public TIdipCommonLog
{
public:
	TIdipUpdateGuildInfo(std::string szName)
		:TIdipCommonLog(szName)
	{
		m_Num = 0;
		m_GuildID = 0;
	}

	virtual ~TIdipUpdateGuildInfo(){}
	virtual void Build(LogHelper& roLog);

public:
	INT32 m_Num;
	UINT64 m_GuildID;
	std::string strMailTitle;
	std::string strMailContent;
};

class TIdipClearFightPunishFlow : public TIdipCommonLog
{
public:
	TIdipClearFightPunishFlow(std::string szName)
		:TIdipCommonLog(szName)
	{
		m_RoleId = 0;
	}

	virtual ~TIdipClearFightPunishFlow(){}
	virtual void Build(LogHelper& roLog);

public:
	UINT64 m_RoleId;
	std::string m_strOpenId;
};

#endif
