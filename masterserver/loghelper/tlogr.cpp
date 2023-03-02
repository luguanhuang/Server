#include "pch.h"
#include "loghelper.h"
#include "tlogr.h"
#include "util.h"
#include "config.h"
#include "appid.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "idutil.h"
#include "tlogwriter.h"
#include "role/role.h"
#include "guild/guildactivity.h"
#include "partner/partnermgr.h"
#include "marriage/marriagemgr.h"
#include "marriage/marriage.h"
#include "dragonguild/dragonguildmgr.h"
#include "dragonguild/dragonguild.h"
#include "task/roletaskmgr.h"

//#define TOLOG_TEST
#define TEST_LOG_INTERVAL 5 * 60 * 1000
#define DO_TLOG(logType) { logType oLog(TLogMgr::Instance()->GetTagString()); oLog.Do(); }
INSTANCE_SINGLETON(TLogMgr)
bool TLogMgr::Init()
{
	m_hLogTimer = CTimerMgr::Instance()->SetTimer(&m_oLogTimer, 0, TEST_LOG_INTERVAL, -1, __FILE__, __LINE__);
	if (INVALID_HTIMER == m_hLogTimer)
	{
		SSWarn<<"m_hLogTimer is NULL"<<END;
		return false;
	}

	return true;
}
void TLogMgr::Uninit()
{
	if(m_hLogTimer != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_hLogTimer);
		m_hLogTimer = INVALID_HTIMER;
	}
}
void TLogMgr::TStateLogTimer::OnTimer(UINT32 dwID, UINT32 dwCount)
{
#ifdef TOLOG_TEST
	DO_TLOG(TGuildFlowLog);
	DO_TLOG(TGuildDareFlow);
	DO_TLOG(TGuildBossFlow);
	DO_TLOG(TGuildCardMatch);
	DO_TLOG(TGardenFlow);
	DO_TLOG(TGuildAuctionFlow);
	DO_TLOG(TGuildBonusFlow);
	DO_TLOG(TAccountLogin);
	DO_TLOG(TFlowerFlow);
	DO_TLOG(TFriendFlow);
#endif
}
std::string TLogMgr::GetTagString()
{
	char pBuf[32] = {0};
	sprintf(pBuf, "TPID%04dMS%08XSQ%04X", MSConfig::Instance()->GetServerID()%10000, (INT32)time(NULL), (++m_iUniqueSeq &0xffff));
	return pBuf;
}

void TLogMgr::Logout(CRole* role)
{
	if (NULL == role)
	{
		return;
	}
	TMsLogoutFlow oLog(GetTagString());
	oLog.m_uRoleID = role->GetID();
	oLog.m_strOpenid = role->GetAccount();
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (pGuild)
	{
		oLog.m_GuildId = pGuild->GetID();
		oLog.m_GuildLv = pGuild->GetLevel();
		oLog.m_GuildPrestige = pGuild->GetPrestige();
		oLog.m_strGuildName = pGuild->GetName();
		oLog.m_GuildMemCnt = pGuild->GetMemberCount();
		UINT32 rank = CGuildMgr::Instance()->GetRankNum(GuildSortByPrestige, pGuild->GetID());
		if (rank)
		{
			oLog.m_Rank = rank;
		}
		CGuildActivity* pActicity = pGuild->Get<CGuildActivity>();
		if (pActicity)
		{
			oLog.m_GuildActivity = pActicity->GetActivity();
		}
	}
	Partner* pPartner = PartnerMgr::Instance()->GetPartner(role->GetID());
	if (pPartner)
	{
		oLog.m_PartnerID = pPartner->GetID();
		oLog.m_PartnerDegree = pPartner->GetDegree();
	}
	Marriage* marriage = MarriageMgr::Instance()->GetMarriage(role->GetID());
	if (marriage)
	{
		oLog.m_CoupleID = marriage->GetCoupleID();
		oLog.m_LiveNess = marriage->GetLiveNess();
	}
	DragonGuild* pDragonGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(role->GetID());
	if (pDragonGuild)
	{
		oLog.m_DragonGuildId = pDragonGuild->GetID();
		oLog.m_DragonGuildMemCount = pDragonGuild->GetMemberCount();
		oLog.m_DragonGuildName = pDragonGuild->GetName();
	}
	oLog.Do();
}

void TLogMgr::DoDailyTaskLog(UINT64 roleid, UINT32 op, UINT64 helperRoleid, UINT32 prescore)
{
	TDailyTaskFlow oLog(GetTagString());
	oLog.m_uRoleID = roleid;
	CRoleTask* pRTask = RoleTaskMgr::Instance()->GetRoleTask(roleid);
	if (pRTask)
	{
		oLog.m_score = pRTask->m_currScore;
	}
	oLog.m_prescore = prescore;
	oLog.m_op = op;
	oLog.m_HelperRoleId = helperRoleid;
	oLog.Do();
}

void TCommonLog::Do()
{
	LogHelper log;
	log.SetDBType(KKSG::LOG_DB_TENCENT);
	log.SetTableName(GetTableName());
	if (strcmp(GetTableName(), "GuildFlow") != 0 && strcmp(GetTableName(), "PlayerRegister") != 0)
	{
		log.AddKeyValue("TransTag", m_szTransTag);
	}
	if (m_uRoleID)
	{
		m_ServerID = GetServerIDByRoleID(m_uRoleID);
	}
	log.AddKeyValue("GameSvrId", ToString(m_ServerID));
	log.AddKeyValueTime("dtEventTime", time(NULL));
	log.AddKeyValue("vGameAppid", MSConfig::Instance()->GetAppType()==KKSG::GAME_APP_QQ?MobileQQ_App_ID:WeChat_App_ID);
	log.AddKeyValue("PlatID", m_iPlatID>=0?ToString(m_iPlatID):ToString((INT32)MSConfig::Instance()->GetPlatType()));
	log.AddKeyValue("iZoneAreaID", ToString(m_ServerID));

	Build(log);
	log.Send();
}
void TCommonFlowLog::Do()
{
	LogHelper log;
	log.SetDBType(KKSG::LOG_DB_TENCENT);
	log.SetTableName(GetTableName());
	Build(log);
	log.Send();
}
void TGuildBossFlow::Build(LogHelper& roLog)
{
	m_ServerID =  GetServerIDByRoleID(m_uRoleID);
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_uRoleID);
	if (pSum)
	{
		m_szOpenID = pSum->GetAccount();
	}
	roLog.AddKeyValue("vopenid", m_szOpenID);
	roLog.AddKeyValue("vRoleID", ToString(m_uRoleID));
	roLog.AddKeyValue("GuildID", ToString(m_uGuildID));
	roLog.AddKeyValue("Damage", ToString(m_Damage));
}

void TGuildFlowLog::Build(LogHelper& roLog)
{
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_uRoleID);
	if (pSum)
	{
		m_szOpenID = pSum->GetAccount();
		m_iLevel = pSum->GetLevel();
		m_iVipLevel = pSum->GetVipLevel();

	}
	roLog.AddKeyValue("vopenid", m_szOpenID);
	roLog.AddKeyValue("iLevel", ToString(m_iLevel));
	roLog.AddKeyValue("iVipLevel", ToString(m_iVipLevel));
	roLog.AddKeyValue("iActType", ToString(m_iActType));
	roLog.AddKeyValue("GuildID", ToString(m_uGuildID));
	roLog.AddKeyValue("iGuildLevel", ToString(m_iGuildLevel));
	roLog.AddKeyValue("iMemberNum", ToString(m_iMemberNum));
	roLog.AddKeyValue("vRoleID", ToString(m_uRoleID));
	roLog.AddKeyValue("TransTag", m_szTransTag);
	roLog.AddKeyValue("Prestige", ToString(m_iPrestige));
	roLog.AddKeyValue("WageLevel", ToString(m_iWageLevel));
	roLog.AddKeyValue("ChangeValue", ToString(m_iChangeValue));
	roLog.AddKeyValue("GuildExp", ToString(m_iGuildExp));
	roLog.AddKeyValue("WeekActivity", ToString(m_iWeekActivity));
	roLog.AddKeyValue("vRoleIDT", ToString(m_RoleIDT));
	roLog.AddKeyValue("DismissMode", ToString(m_DismissMode));
	roLog.AddKeyValue("CreateTime", ToString(m_CreateTime));
	roLog.AddKeyValue("GuildName", ProcessRoleName(m_strName));
}

void TGuildDareFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("GuildID", ToString(m_uGuildID));
	roLog.AddKeyValue("iGuildLevel", ToString(m_iGuildLevel));
	roLog.AddKeyValue("iDareNum", ToString(m_iDareNum));
	roLog.AddKeyValue("iAfterDareNum", ToString(m_iAfterDareNum));
}

void TRegisterLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("vopenid", m_szOpenID);
	roLog.AddKeyValue("ClientVersion", m_stConf.clientversion());
	roLog.AddKeyValue("SystemSoftware", m_stConf.systemsoftware());
	roLog.AddKeyValue("SystemHardware", m_stConf.systemhardware());
	roLog.AddKeyValue("TelecomOper", m_stConf.telecomoper());
	roLog.AddKeyValue("Network", m_stConf.network());
	roLog.AddKeyValue("ScreenWidth", ToString(m_stConf.screenwidth()));
	roLog.AddKeyValue("ScreenHight", ToString(m_stConf.screenhight()));
	roLog.AddKeyValue("Density", ToString(m_stConf.density()));
	roLog.AddKeyValue("RegChannel", m_stConf.loginchannel());

	roLog.AddKeyValue("CpuHardware", m_stConf.cpuhardware());
	roLog.AddKeyValue("Memory", ToString(m_stConf.memory()));
	roLog.AddKeyValue("GLRender", m_stConf.glrender());
	roLog.AddKeyValue("GLVersion", m_stConf.glversion());
	roLog.AddKeyValue("DeviceId", m_stConf.deviceid());
	roLog.AddKeyValue("TransTag", m_szTransTag);
	roLog.AddKeyValue("vRoleID", ToString(m_uRoleID));
}

void TAccountLogin::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("vopenid", m_szOpenID);
}

void TGuildCardMatch::Build(LogHelper& roLog)
{
	m_ServerID =  GetServerIDByRoleID(m_uRoleID);
	roLog.AddKeyValue("vopenid", m_strOpenId);
	roLog.AddKeyValue("vRoleID", ToString(m_uRoleID));
	roLog.AddKeyValue("GuildID", ToString(m_uGuildID));
	roLog.AddKeyValue("Round", ToString(m_uRound));
	roLog.AddKeyValue("Result", ToString(m_iResult));
	roLog.AddKeyValue("OperType", ToString(m_iOp));
}

void TGardenFlow::Build(LogHelper& roLog)
{
	m_ServerID =  GetServerIDByRoleID(m_uRoleID);
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_uRoleID);
	roLog.AddKeyValue("OpenId", pSum?pSum->GetAccount():"");
	roLog.AddKeyValue("RoleID", ToString(m_uRoleID));
	roLog.AddKeyValue("FriendRoleID", ToString(m_uGardenID));
	roLog.AddKeyValue("FishLevel", ToString(m_uFishLevel));
	roLog.AddKeyValue("FishExp", ToString(m_uFishExp));
	roLog.AddKeyValue("CookLevel", ToString(m_uCookLevel));
	roLog.AddKeyValue("CookExp", ToString(m_uCookExp));
	roLog.AddKeyValue("SeedID", ToString(m_uSeedID));
	roLog.AddKeyValue("BanquetID", ToString(m_uBanquetID));
	roLog.AddKeyValue("CookBookID", ToString(m_uCookBookID));
	roLog.AddKeyValue("FoodID", ToString(m_uFoodID));
	roLog.AddKeyValue("iResult", ToString(m_iResult));
	roLog.AddKeyValue("OperType", ToString(m_iOp));
	roLog.AddKeyValue("QuestType", ToString(m_iQuestType));
}
void TGuildBonusFlow::Build(LogHelper& roLog)
{
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_uRoleID);
	roLog.AddKeyValue("OpenId", pSum?pSum->GetAccount():"");
	roLog.AddKeyValue("RoleID", ToString(m_uRoleID));
	roLog.AddKeyValue("OperType", ToString(m_iOp));
	roLog.AddKeyValue("BonusID", ToString(oBonusInfo.bonusID));
	roLog.AddKeyValue("BonusType", oBonusInfo.bonusType);
	roLog.AddKeyValue("MaxPeopleNum", ToString(oBonusInfo.maxPeopleNum));
	roLog.AddKeyValue("BonusNum", ToString(oBonusInfo.bonusNum));//红包奖励数;
	roLog.AddKeyValue("SendTime", ToString(oBonusInfo.sendTime));
	roLog.AddKeyValue("ContentType", ToString(oBonusInfo.templateID));//红包类型
	roLog.AddKeyValue("SourceRoleID", ToString(oBonusInfo.sourceCRoleID));
	roLog.AddKeyValue("LeaderID", ToString(oBonusInfo.leaderID));
	roLog.AddKeyValue("LuckestID", ToString(oBonusInfo.luckestID));
	roLog.AddKeyValue("GetCount", ToString(m_iGetCount));
	roLog.AddKeyValue("iGetAwardNum", ToString(m_iGetAwardNum));
}

void TFlowerFlow::Build(LogHelper& roLog)
{
	m_ServerID = GetServerIDByRoleID(m_uRoleID);
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_uRoleID);
	roLog.AddKeyValue("OpenId", pSum?pSum->GetAccount():"");
	roLog.AddKeyValue("RoleID", ToString(m_uRoleID));
	roLog.AddKeyValue("TotalNum", ToString(m_TotalNum));
	roLog.AddKeyValue("TodayNum", ToString(m_TodayNum));
	roLog.AddKeyValue("YesNum", ToString(m_YesNum));
	roLog.AddKeyValue("ThisWeekNum", ToString(m_ThisWeekNum));
}

void TFriendFlow::Build(LogHelper& roLog)
{
	m_ServerID = GetServerIDByRoleID(m_uRoleID);
	CRoleSummary* pSum1 = CRoleSummaryMgr::Instance()->GetRoleSummary(m_uRoleID);
	CRoleSummary* pSum2 = CRoleSummaryMgr::Instance()->GetRoleSummary(m_Roleid2);
	roLog.AddKeyValue("iOp", ToString(m_Op));
	roLog.AddKeyValue("OpenId", pSum1?pSum1->GetAccount():"");
	roLog.AddKeyValue("RoleID", ToString(m_uRoleID));
	roLog.AddKeyValue("iLevel", pSum1?pSum1->GetLevel():0);
	roLog.AddKeyValue("iVipLevel", pSum1?pSum1->GetVipLevel():0);
	roLog.AddKeyValue("iProfession", pSum1?pSum1->GetProfession():0);
	roLog.AddKeyValue("iPowerPoint", pSum1?pSum1->GetPowerPoint():0);
	roLog.AddKeyValue("OpenId2", pSum2?pSum2->GetAccount():"");
	roLog.AddKeyValue("RoleID2", ToString(m_Roleid2));
	roLog.AddKeyValue("iLevel2", pSum2?pSum2->GetLevel():0);
	roLog.AddKeyValue("iVipLevel2", pSum2?pSum2->GetVipLevel():0);
	roLog.AddKeyValue("iProfession2", pSum2?pSum2->GetProfession():0);
	roLog.AddKeyValue("iPowerPoint2", pSum2?pSum2->GetPowerPoint():0);
	roLog.AddKeyValue("FriendNum", ToString(m_FriendNum));
}

void TGuildAuctionFlow::Build(LogHelper& roLog)
{
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_uRoleID);
	roLog.AddKeyValue("OpenId", pSum?pSum->GetAccount():"");
	roLog.AddKeyValue("RoleID", ToString(m_uRoleID));
	roLog.AddKeyValue("GuildID", ToString(m_uGuildID));
	roLog.AddKeyValue("UID", ToString(m_uUID));
	roLog.AddKeyValue("ItemID", ToString(m_uItemID));
	roLog.AddKeyValue("ItemCount", ToString(m_uItemCount));
	roLog.AddKeyValue("AuctRoleID", ToString(m_uAuctRoleID));
	roLog.AddKeyValue("AuctPrice", ToString(m_uAuctPrice));
	roLog.AddKeyValue("MaxPirce", ToString(m_uMaxPrice));
	roLog.AddKeyValue("ProfitNum", ToString(m_uProfitNum));
	roLog.AddKeyValue("ActType", ToString(m_iActType));
	roLog.AddKeyValue("OperType", ToString(m_iOperType));
}
void TGuildAuctionFlow::SetData(GABaseData* data)
{
	if (NULL == data)
	{
		return ;
	}
	m_uGuildID = data->GetGuildID();
	m_uUID = data->GetUID();//系统生成唯一ID
	m_iActType = data->GetActType();//活动类型
	m_uItemID = data->GetItemID();//物品ID
	m_uItemCount= data->GetItemCount();//物品数量
	m_uAuctRoleID = data->GetAuctRoleID();//最后一个竞价人
	m_uAuctPrice = data->GetAuctPrice();//当前竞拍价格
	m_uMaxPrice = data->GetMaxPrice();//一口价
}

void TGuildRankFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("GuildID", ToString(m_GuildID));
	roLog.AddKeyValue("GuildLv", ToString(m_GuildLevel));
	roLog.AddKeyValue("Rank", ToString(m_Rank));
	roLog.AddKeyValue("Prestige", ToString(m_Prestige));
	roLog.AddKeyValue("GuildName", ProcessRoleName(m_strName));
}

void TFlowerRankFlow::Build(LogHelper& roLog)
{
	m_ServerID = GetServerIDByRoleID(m_uRoleID);
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_uRoleID);
	roLog.AddKeyValue("OpenId", pSum?pSum->GetAccount():"");
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	roLog.AddKeyValue("Num", ToString(m_Num));
	roLog.AddKeyValue("Type", ToString(m_Type));
	roLog.AddKeyValue("Rank", ToString(m_Rank));
}

void TFirstPassFlow::SetData(std::vector<CRoleSummary*> &vList)
{
	std::ostringstream ss;
	UINT32 Count = 0;
	for (UINT32 i = 0; i < vList.size(); i ++)
	{
		if (vList[i])
		{
			if (Count > 0)
			{
				ss << "#";
			}
			ss << vList[i]->GetAccount() << "=" << vList[i]->GetID() << "=" <<vList[i]->GetLevel() << "=" << vList[i]->GetProfession() << "=" << vList[i]->GetPowerPoint();
			Count++;
		}
	}
	m_strTeam = ss.str();
}

void TFirstPassFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("ID", ToString(m_ID));
	roLog.AddKeyValue("SceneID", ToString(m_SceneID));
	roLog.AddKeyValue("Rank", ToString(m_Rank));
	roLog.AddKeyValue("StarLv", ToString(m_StarLv));
	roLog.AddKeyValue("TeamInfo", m_strTeam);
}

void TInvFightFlow::Build(LogHelper& roLog)
{
	CRoleSummary* pFromSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_FromId);
	if (pFromSum)
	{
		m_strFromOpenId = pFromSum->GetAccount();
	}
	CRoleSummary* pToSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_ToId);
	if (pToSum)
	{
		m_strToOpenId = pToSum->GetAccount();
	}
	for (INT32 i = 0; i < m_Data.roles_size(); i ++)
	{
		if (m_FromId == m_Data.roles(i).roleid())
		{
			m_FromLevel = m_Data.roles(i).rolelevel();
			m_FromProfession = m_Data.roles(i).roleprofession();
			m_FromPpt = m_Data.roles(i).roleppt();
		}
		else if (m_ToId == m_Data.roles(i).roleid())
		{
			m_ToLevel = m_Data.roles(i).rolelevel();
			m_ToProfession = m_Data.roles(i).roleprofession();
			m_ToPpt = m_Data.roles(i).roleppt();
		}
	}
	roLog.AddKeyValue("OpenId", m_strFromOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_FromId));
	roLog.AddKeyValue("iLevel", ToString(m_FromLevel));
	roLog.AddKeyValue("iProfession", ToString(m_FromProfession));
	roLog.AddKeyValue("iPowerPoint", ToString(m_FromPpt));
	roLog.AddKeyValue("OpenId2", m_strToOpenId);
	roLog.AddKeyValue("RoleId2", ToString(m_ToId));
	roLog.AddKeyValue("iLevel2", ToString(m_ToLevel));
	roLog.AddKeyValue("iProfession2", ToString(m_ToProfession));
	roLog.AddKeyValue("iPowerPoint2", ToString(m_ToPpt));
}

void TPayItem::Build(LogHelper& roLog)
{
	m_ServerID = GetServerIDByRoleID(m_FromRoleId);
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_ToRoleId);
	if (pSum)
	{
		m_ToOpenId = pSum->GetAccount();
	}
	roLog.AddKeyValue("OpenId", m_FromOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_FromRoleId));
	roLog.AddKeyValue("ToOpenId", m_ToOpenId);
	roLog.AddKeyValue("ToRoleId", ToString(m_ToRoleId));
	roLog.AddKeyValue("ParamId", m_GoodsId);
	roLog.AddKeyValue("Amount", ToString(m_PayDegree));
	roLog.AddKeyValue("Count", ToString(m_Count));

}

void TMailFlow::Build(LogHelper& roLog)
{
	m_ServerID = GetServerIDByRoleID(m_Roleid);
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_Roleid);
	if (pSum)
	{
		m_strOpenid = pSum->GetAccount();
	}
	roLog.AddKeyValue("OpenId", m_strOpenid);
	roLog.AddKeyValue("RoleId", ToString(m_Roleid));
	roLog.AddKeyValue("Op", ToString(m_Op));
	roLog.AddKeyValue("MailId", ToString(m_Data.uid()));
	roLog.AddKeyValue("Stime", ToString(m_Data.timestamp()));
	roLog.AddKeyValue("Etime", ToString(m_Data.timeexpire()));
	roLog.AddKeyValue("Reason", ToString(m_Data.reason()));
	roLog.AddKeyValue("SubReason", ToString(m_Data.subreason()));
	roLog.AddKeyValue("Title", ProcessRoleName(m_Data.title()));
	roLog.AddKeyValue("Content", ProcessRoleName(m_Data.content()));
	std::ostringstream ss;
	for (INT32 i = 0; i < m_Data.items_size(); i ++)
	{
		if (i > 0)
		{
			ss << "#";
		}
		ss  << m_Data.items(i).itemid() << "=" << m_Data.items(i).itemcount();
	}

	for (INT32 i = 0; i < m_Data.xitems_size(); i ++)
	{
		if (i > 0)
		{
			ss << "#";
		}
		ss << m_Data.xitems(i).itemid() << "=" << m_Data.xitems(i).itemcount();
	}
	roLog.AddKeyValue("ItemInfo", ss.str());
}


void TLeagueTeamFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("TeamId", ToString(m_TeamId));
	roLog.AddKeyValue("Name", ProcessRoleName(m_strName));
	std::ostringstream ss;
	for (UINT32 i = 0; i < m_roleIds.size(); i ++)
	{
		if (i > 0)
		{
			ss << "#";
		}
		ss << m_roleIds[i];
	}
	roLog.AddKeyValue("RoleIds", ss.str());
	roLog.AddKeyValue("RoleNum", ToString(m_Num));
	roLog.AddKeyValue("Type", ToString(m_Type));
}

void TMatchFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("MatchID", ToString(m_matchID));
	roLog.AddKeyValue("TeamID", ToString(m_teamID));
	roLog.AddKeyValue("RoleID", ToString(m_uRoleID));
	roLog.AddKeyValue("type", ToString(m_Type));
	roLog.AddKeyValue("Time", ToString(m_Time));
}

void TMsLogoutFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	roLog.AddKeyValue("Openid", m_strOpenid);
	roLog.AddKeyValue("GuildId", ToString(m_GuildId));
	roLog.AddKeyValue("GuildName", ProcessRoleName(m_strGuildName));
	roLog.AddKeyValue("GuildLv", ToString(m_GuildLv));
	roLog.AddKeyValue("GuildMemCnt", ToString(m_GuildMemCnt));
	roLog.AddKeyValue("GuildPrestige", ToString(m_GuildPrestige));
	roLog.AddKeyValue("GuildActivity", ToString(m_GuildActivity));
	roLog.AddKeyValue("Rank", ToString(m_Rank));
	roLog.AddKeyValue("PartnerID", ToString(m_PartnerID));
	roLog.AddKeyValue("PartnerDegree", ToString(m_PartnerDegree));
	roLog.AddKeyValue("CoupleID", ToString(m_CoupleID));
	roLog.AddKeyValue("LiveNess", ToString(m_LiveNess));
	roLog.AddKeyValue("DragonGuildId", ToString(m_DragonGuildId));
	roLog.AddKeyValue("DragonGuildMemCount", ToString(m_DragonGuildMemCount));
	roLog.AddKeyValue("DragonGuildName", ProcessRoleName(m_DragonGuildName));
}

void TFmFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	roLog.AddKeyValue("Openid", m_strOpenId);
	roLog.AddKeyValue("Op", ToString(m_op));
	roLog.AddKeyValue("Chat", ProcessRoleName(m_strChat));
}

void TGuildCastleFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("TerritoryID", ToString(m_TerritoryID));
	roLog.AddKeyValue("GuildId", ToString(m_GuildId));
	roLog.AddKeyValue("GuildName", ProcessRoleName(m_strGuildName));
	roLog.AddKeyValue("allianceId", ToString(m_allianceId));
	roLog.AddKeyValue("GuildId2", ToString(m_GuildId2));
	roLog.AddKeyValue("GuildName2", ProcessRoleName(m_strGuildName2));
	roLog.AddKeyValue("Result", ToString(m_Result));
}

void TPlatformFriendFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("RoleId2", ToString(m_RoleId2));
	roLog.AddKeyValue("OpenId2", m_strOpenId2);
	roLog.AddKeyValue("NickName", ProcessRoleName(m_strNickName));
	roLog.AddKeyValue("BigPic", ProcessRoleName(m_strBigPic));
	roLog.AddKeyValue("MidPic", ProcessRoleName(m_strMidPic));
	roLog.AddKeyValue("SmallPic", ProcessRoleName(m_strSmallPic));
	roLog.AddKeyValue("NickName2", ProcessRoleName(m_strNickName2));
	roLog.AddKeyValue("BigPic2", ProcessRoleName(m_strBigPic2));
	roLog.AddKeyValue("MidPic2", ProcessRoleName(m_strMidPic2));
	roLog.AddKeyValue("SmallPic2", ProcessRoleName(m_strSmallPic2));
}

void TWorldBossFlow::SetData(CRoleSummary* data)
{
	if (data)
	{
		m_strOpenId = data->GetAccount();
		m_Level = data->GetLevel();
		m_Pro = data->GetProfession();
		m_PPt = data->GetPowerPoint();
	}
}
void TWorldBossFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	roLog.AddKeyValue("OpendId", m_strOpenId);
	roLog.AddKeyValue("Level", ToString(m_Level));
	roLog.AddKeyValue("Pro", ToString(m_Pro));
	roLog.AddKeyValue("PPt", ToString(m_PPt));
	roLog.AddKeyValue("Rank", ToString(m_Rank));
	roLog.AddKeyValue("Damge", ToString(m_Damge));
}

void TMentorFlow::Build(LogHelper& roLog)
{
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_uRoleID);
	if (pSum)m_strOpenId = pSum->GetAccount();
	pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_RoleID2);
	if (pSum)m_strOpenId2 = pSum->GetAccount();
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	roLog.AddKeyValue("OpendId", m_strOpenId);
	roLog.AddKeyValue("RoleId2", ToString(m_RoleID2));
	roLog.AddKeyValue("OpendId2", m_strOpenId2);
	roLog.AddKeyValue("Op", ToString(m_op));
	roLog.AddKeyValue("TaskId", ToString(m_TaskId));
}

void TRoleReportFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_uRoleID);
	if (pSum)m_strOpenId = pSum->GetAccount();
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("Group", ToString(m_RoleGroup));
	roLog.AddKeyValue("ReportRoleId", ToString(m_RoleId2));
	roLog.AddKeyValue("ReportOpenId", m_strOpenId2);
	roLog.AddKeyValue("ReportGroup2", ToString(m_RoleGroup2));
	roLog.AddKeyValue("SceneType", ToString(m_SceneType));
	roLog.AddKeyValue("Num", ToString(m_Num));
	roLog.AddKeyValue("ReportNum", ToString(m_ReportNum));
	roLog.AddKeyValue("Reason", m_strReason);
	roLog.AddKeyValue("Op", ToString(m_op));
}

void TRoleReportBanFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_uRoleID);
	if (pSum)m_strOpenId = pSum->GetAccount();
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("SceneType", ToString(m_SceneType));
	roLog.AddKeyValue("Time", ToString(m_Time));
	roLog.AddKeyValue("ReportNum", ToString(m_ReportNum));
	roLog.AddKeyValue("BanEndTime", ToString(m_BanEndTime));
}

void TCustomPkFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("MatchId", ToString(m_MatchId));
	roLog.AddKeyValue("Op", ToString(m_Op));
	roLog.AddKeyValue("MatchType", ToString(m_Matchtype));
	roLog.AddKeyValue("MatchMode", ToString(m_MatchMode));
	roLog.AddKeyValue("ConfigId", ToString(m_ConfigId));
	roLog.AddKeyValue("IsSystem", ToString(m_isSystem));
	roLog.AddKeyValue("MatchName", ProcessRoleName(m_strMatchName));
}

void TMobaEloFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_uRoleID);
	if (pSum)m_strOpenId = pSum->GetAccount();
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("Elo", ToString(m_Elo));
	roLog.AddKeyValue("NewElo", ToString(m_NewElo));
}

void TAuctionExpireFlow::SetData(AuctionItem* pSaleItem)
{
	m_uRoleID = pSaleItem->GetRoleID();
	m_AuctionID = pSaleItem->GetUID();
	m_ItemUID = pSaleItem->GetItemUID();
	m_ItemID = pSaleItem->GetItemID();
	m_ItemCount = pSaleItem->GetItemCount();
	m_DealPrice = pSaleItem->GetPerPrice();
	m_DealPriceTotal = pSaleItem->GetPerPrice() * pSaleItem->GetItemCount();
	m_OnSaleTime = pSaleItem->GetOnSaleTime();
}

void TAuctionExpireFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_uRoleID);
	if (pSum)m_strOpenId = pSum->GetAccount();
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("AuctionID", ToString(m_AuctionID));
	roLog.AddKeyValue("ItemId", ToString(m_ItemID));
	roLog.AddKeyValue("ItemUID", ToString(m_ItemUID));
	roLog.AddKeyValue("ItemCount", ToString(m_ItemCount));
	roLog.AddKeyValue("DealPrice", ToString(m_DealPrice));
	roLog.AddKeyValue("DealPriceTotal", ToString(m_DealPriceTotal));
	roLog.AddKeyValue("OnSaleTime", ToString(m_OnSaleTime));
	roLog.AddKeyValue("Op", ToString(m_Op));
}

void TMarriageFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	TRoleBaseInfo data;
	data.GetData(m_uRoleID);
	roLog.AddKeyValue("OpenId", data.GetAccount());
	roLog.AddKeyValue("Level", ToString(data.GetLevel()));
	roLog.AddKeyValue("PowerPoint", ToString(data.GetPowerPoint()));
	roLog.AddKeyValue("Pro", ToString(data.GetPro()));
	data.GetData(m_RoleID2);
	roLog.AddKeyValue("RoleId2", ToString(m_RoleID2));
	roLog.AddKeyValue("OpenId2", data.GetAccount());
	roLog.AddKeyValue("Level2", ToString(data.GetLevel()));
	roLog.AddKeyValue("PowerPoint2", ToString(data.GetPowerPoint()));
	roLog.AddKeyValue("Pro2", ToString(data.GetPro()));
	roLog.AddKeyValue("Op", ToString(m_Op));
	roLog.AddKeyValue("WeddingType", ToString(m_WeddingType));
	roLog.AddKeyValue("WeddingID", ToString(m_WeddingID));
}

void TWeddingFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	TRoleBaseInfo data;
	data.GetData(m_uRoleID);
	roLog.AddKeyValue("OpenId", data.GetAccount());
	roLog.AddKeyValue("Level", ToString(data.GetLevel()));
	roLog.AddKeyValue("PowerPoint", ToString(data.GetPowerPoint()));
	roLog.AddKeyValue("Pro", ToString(data.GetPro()));
	data.GetData(m_RoleID1);
	roLog.AddKeyValue("RoleId1", ToString(m_RoleID1));
	roLog.AddKeyValue("OpenId1", data.GetAccount());
	roLog.AddKeyValue("Level1", ToString(data.GetLevel()));
	roLog.AddKeyValue("PowerPoint1", ToString(data.GetPowerPoint()));
	roLog.AddKeyValue("Pro1", ToString(data.GetPro()));
	data.GetData(m_RoleID2);
	roLog.AddKeyValue("RoleId2", ToString(m_RoleID2));
	roLog.AddKeyValue("OpenId2", data.GetAccount());
	roLog.AddKeyValue("Level2", ToString(data.GetLevel()));
	roLog.AddKeyValue("PowerPoint2", ToString(data.GetPowerPoint()));
	roLog.AddKeyValue("Pro2", ToString(data.GetPro()));
	roLog.AddKeyValue("WeddingType", ToString(m_WeddingType));
	roLog.AddKeyValue("WeddingID", ToString(m_WeddingID));

}

void TDragonGuildFlowLog::Build(LogHelper& roLog)
{
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(m_uRoleID);
	if (pSum)
	{
		m_szOpenID = pSum->GetAccount();
		m_iLevel = pSum->GetLevel();
	}

	roLog.AddKeyValue("vopenid", m_szOpenID);
	roLog.AddKeyValue("iLevel", ToString(m_iLevel));
	roLog.AddKeyValue("iActType", ToString(m_iActType));
	roLog.AddKeyValue("DGuildID", ToString(m_uDGuildID));
	roLog.AddKeyValue("iDGuildLevel", ToString(m_iDGuildLevel));
	roLog.AddKeyValue("iMemberNum", ToString(m_iMemberNum));
	roLog.AddKeyValue("vRoleID", ToString(m_uRoleID));
	roLog.AddKeyValue("TransTag", m_szTransTag);
	roLog.AddKeyValue("ChangeValue", ToString(m_iChangeValue));
	roLog.AddKeyValue("GuildExp", ToString(m_iDGuildExp));
	roLog.AddKeyValue("vRoleIDT", ToString(m_RoleIDT));
	roLog.AddKeyValue("DismissMode", ToString(m_DismissMode));
	roLog.AddKeyValue("CreateTime", ToString(m_CreateTime));
	roLog.AddKeyValue("GuildName", ProcessRoleName(m_strName));
}


void TIssueFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	TRoleBaseInfo data;
	data.GetData(m_uRoleID);
	roLog.AddKeyValue("OpenId", data.GetAccount());
	roLog.AddKeyValue("Level", ToString(data.GetLevel()));
	roLog.AddKeyValue("PowerPoint", ToString(data.GetPowerPoint()));
	roLog.AddKeyValue("Pro", ToString(data.GetPro()));
	roLog.AddKeyValue("GroupID", ToString(m_GroupID));
	roLog.AddKeyValue("showStageID", ToString(m_showStageID));
	roLog.AddKeyValue("showFighting", ToString(m_showFighting));
	roLog.AddKeyValue("showFightType", ToString(m_showFightType));
	roLog.AddKeyValue("showTime", ToString(m_showTime));
}

void TChatGroupFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	TRoleBaseInfo data;
	data.GetData(m_uRoleID);
	roLog.AddKeyValue("OpenId", data.GetAccount());
	roLog.AddKeyValue("Level", ToString(data.GetLevel()));
	roLog.AddKeyValue("PowerPoint", ToString(data.GetPowerPoint()));
	roLog.AddKeyValue("Pro", ToString(data.GetPro()));
	roLog.AddKeyValue("GroupID", ToString(m_GroupID));
	roLog.AddKeyValue("LeaderID", ToString(m_LeaderID));
	roLog.AddKeyValue("Op", ToString(m_Op));
}

void TGoalAwardFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	TRoleBaseInfo data;
	data.GetData(m_uRoleID);
	roLog.AddKeyValue("OpenId", data.GetAccount());
	roLog.AddKeyValue("Level", ToString(data.GetLevel()));
	roLog.AddKeyValue("PowerPoint", ToString(data.GetPowerPoint()));
	roLog.AddKeyValue("Pro", ToString(data.GetPro()));
	roLog.AddKeyValue("Op", ToString(m_op));
	roLog.AddKeyValue("AwardID", ToString(m_AwardID));
	roLog.AddKeyValue("AwardIndex", ToString(m_AwardIndex));
	roLog.AddKeyValue("AwardType", ToString(m_AwardType));
}

void TDailyTaskFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_uRoleID));
	TRoleBaseInfo data;
	data.GetData(m_uRoleID);
	roLog.AddKeyValue("OpenId", data.GetAccount());
	roLog.AddKeyValue("Level", ToString(data.GetLevel()));
	roLog.AddKeyValue("PowerPoint", ToString(data.GetPowerPoint()));
	roLog.AddKeyValue("Pro", ToString(data.GetPro()));
	data.GetData(m_HelperRoleId);
	roLog.AddKeyValue("HelperRoleId", ToString(m_HelperRoleId));
	roLog.AddKeyValue("HelperOpenId", data.GetAccount());
	roLog.AddKeyValue("Score", ToString(m_score));
	roLog.AddKeyValue("PreScore", ToString(m_prescore));
	roLog.AddKeyValue("Op", ToString(m_op));
}

TRoleBaseInfo::TRoleBaseInfo()
{
	Init();
}

void TRoleBaseInfo::Init()
{
	m_Level = 0;
	m_PowerPoint = 0;
	m_Pro = 0;
	m_strOpenId = "";
}

void TRoleBaseInfo::GetData(UINT64 roleid)
{
	Init();
	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
	if (pSum)
	{
		m_Level = pSum->GetLevel();
		m_PowerPoint = pSum->GetPowerPoint();
		m_Pro = pSum->GetProfession();
		m_strOpenId = pSum->GetAccount();
	}
}

void TCustomPkFlow::BuildCustom(const KKSG::CustomBattleConfig& config)
{
	m_Matchtype = config.fighttype();
	m_MatchMode = config.isfair() ? 0 : 1;
	m_ConfigId = config.configid();//比赛配置ID;
	m_isSystem = config.issystem();//是否系统比赛;//0:否 1：是
	m_strMatchName = config.name();//比赛命名名称;
}

//----
void TBlackListFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("GameSvrId", ToString(m_ServerID));
	roLog.AddKeyValueTime("dtEventTime", time(NULL));
	roLog.AddKeyValue("GameAppID", MSConfig::Instance()->GetAppType()==KKSG::GAME_APP_QQ?MobileQQ_App_ID:WeChat_App_ID);
	roLog.AddKeyValue("OpenID", m_strOpenId);
	roLog.AddKeyValue("PlatID", ToString((INT32)MSConfig::Instance()->GetPlatType()));
	roLog.AddKeyValue("AreaID", ToString(MSConfig::Instance()->GetAppType()));
	roLog.AddKeyValue("ZoneID", ToString(m_ServerID));
	roLog.AddKeyValue("PlayerOpenID", m_strPlayerOpenId);
	roLog.AddKeyValue("GetMode", ToString(m_iGetMode));
}

void TAuctionStartFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("GameSvrId", ToString(GetServerIDByRoleID(m_lRoleUid)));
	roLog.AddKeyValueTime("dtEventTime", time(NULL));
	roLog.AddKeyValue("GameAppID", MSConfig::Instance()->GetAppType()==KKSG::GAME_APP_QQ?MobileQQ_App_ID:WeChat_App_ID);
	roLog.AddKeyValue("OpenID", m_strOpenId);
	roLog.AddKeyValue("PlatID", ToString((INT32)MSConfig::Instance()->GetPlatType()));
	roLog.AddKeyValue("AreaID", ToString(MSConfig::Instance()->GetAppType()));
	roLog.AddKeyValue("ZoneID", ToString(GetServerIDByRoleID(m_lRoleUid)));
	roLog.AddKeyValue("RoleUid", ToString(m_lRoleUid));
	roLog.AddKeyValue("RoleJobID", ToString(m_iRoleJobID));
	roLog.AddKeyValue("RoleLevel", ToString(m_iRoleLevel));
	roLog.AddKeyValue("UserIP", m_strUserIP);
	roLog.AddKeyValue("UseVIP", ToString(m_iUseVIP));
	roLog.AddKeyValue("AuctionID", ToString(m_lAuctionID));
	roLog.AddKeyValue("AuctionPriceType", ToString(m_iAuctionPriceType));
	roLog.AddKeyValue("AuctionPrice1", ToString(m_iAuctionPrice1));
	roLog.AddKeyValue("AuctionPrice2", ToString(m_iAuctionPrice1));
	roLog.AddKeyValue("AuctionCount", ToString(m_iAuctionCount));
	roLog.AddKeyValue("AuctionItemID", ToString(m_iAuctionItemID));
	roLog.AddKeyValue("AuctionUUID", ToString(m_lAuctionUUID));
	roLog.AddKeyValue("AuctionLevel", ToString(m_iAuctionLevel));
	roLog.AddKeyValue("AuctionQlty", ToString(m_iAuctionQlty));
	roLog.AddKeyValue("AuctionTime", ToString(m_iAuctionTime));
}

void TAuctionEndFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("GameSvrId", ToString(GetServerIDByRoleID(m_lBuyerRoleUid)));
	roLog.AddKeyValueTime("dtEventTime", time(NULL));
	roLog.AddKeyValue("GameAppID", MSConfig::Instance()->GetAppType()==KKSG::GAME_APP_QQ?MobileQQ_App_ID:WeChat_App_ID);
	roLog.AddKeyValue("OpenID", m_strBuyerOpenID);
	roLog.AddKeyValue("PlatID", ToString((INT32)MSConfig::Instance()->GetPlatType()));
	roLog.AddKeyValue("AreaID", ToString(MSConfig::Instance()->GetAppType()));
	roLog.AddKeyValue("ZoneID", ToString(GetServerIDByRoleID(m_lBuyerRoleUid)));
	roLog.AddKeyValue("BuyerOpenID", m_strBuyerOpenID);
	roLog.AddKeyValue("BuyerRoleUid", ToString(m_lBuyerRoleUid));
	roLog.AddKeyValue("BuyerRoleJobID", ToString(m_iBuyerRoleJobID));
	roLog.AddKeyValue("BuyerRoleLevel", ToString(m_iBuyerRoleLevel));
	roLog.AddKeyValue("BuyerRoleBattlePoint", ToString(m_iBuyerRoleBattlePoint));
	roLog.AddKeyValue("BuyerUserIP", m_strBuyerUserIP);
	roLog.AddKeyValue("BuyerUseVIP", ToString(m_iBuyerUseVIP));
	roLog.AddKeyValue("SellerOpenID", ToString(m_strSellerOpenID));
	roLog.AddKeyValue("SellerRoleUid", ToString(m_lSellerRoleUid));
	roLog.AddKeyValue("SellerRoleJobID", ToString(m_iSellerRoleJobID));
	roLog.AddKeyValue("SellerRoleLevel", ToString(m_iSellerRoleLevel));
	roLog.AddKeyValue("SellerUserIP", m_strSellerUserIP);
	roLog.AddKeyValue("DealAuctionID", ToString(m_lDealAuctionID));
	roLog.AddKeyValue("DealMainType", ToString(m_iDealMainType));
	roLog.AddKeyValue("DealPriceType", ToString(m_iDealPriceType));
	roLog.AddKeyValue("DealCount", ToString(m_iDealCount));
	roLog.AddKeyValue("DealPrice", ToString(m_iDealPrice));
	roLog.AddKeyValue("DealPriceTotal", ToString(m_iDealPriceTotal));
	roLog.AddKeyValue("DealItemID", ToString(m_iDealItemID));
	roLog.AddKeyValue("DealUUID", ToString(m_lDealUUID));
	roLog.AddKeyValue("DealLevel", ToString(m_iDealLevel));
	roLog.AddKeyValue("DealQlty", ToString(m_iDealQlty));
}
void TIdipCommonLog::Do(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson)
{
	LogHelper log;
	log.SetDBType(KKSG::LOG_DB_TENCENT);
	log.SetTableName(GetTableName());
	log.AddKeyValueTime("dtEventTime", time(NULL));
	log.AddKeyValue("CmdID", ToString(JsonUtil::GetJsonInt(roHeadJson, "Cmdid")));
	log.AddKeyValue("AreaID", ToString(JsonUtil::GetJsonUint(roBodyJson, "AreaId")));
	log.AddKeyValue("Partition", ToString(JsonUtil::GetJsonUint(roBodyJson, "Partition")));
	log.AddKeyValue("PlatId", ToString(JsonUtil::GetJsonUint(roBodyJson, "PlatId")));
	log.AddKeyValue("Source", ToString(JsonUtil::GetJsonUint(roBodyJson, "Source")));
	log.AddKeyValue("Serial", JsonUtil::GetJsonString(roBodyJson, "Serial"));

	Build(log);
	log.Send();
}

void TIdipMoneyLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("OpenId", szOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_uRoleId));
	roLog.AddKeyValue("Num", ToString(m_nNum));
}

void TIdipDoGmMoneyLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("Num", ToString(m_nNum));
}

void TIdipDelItemLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("OpenId", szOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_uRoleId));
	roLog.AddKeyValue("ItemID", ToString(m_uItemID));
	roLog.AddKeyValue("Num", ToString(m_nNum));
}

void TIdipSendItemLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("OpenId", szOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_uRoleId));
	roLog.AddKeyValue("ItemID", ToString(m_uItemID));
	roLog.AddKeyValue("Num", ToString(m_nNum));
	roLog.AddKeyValue("Bind", ToString(m_uBind));
	roLog.AddKeyValue("EndTime", ToString(EndTime));
}

void TIdipDoGmSendItemLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("ItemID", ToString(m_uItemID));
	roLog.AddKeyValue("Num", ToString(m_nNum));
	roLog.AddKeyValue("Bind", ToString(m_uBind));
	roLog.AddKeyValue("EndTime", ToString(EndTime));
}
void TIdipAqDoInitialAccount::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_uRoleId));
}

void TIdipBanRank::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_uRoleId));
	roLog.AddKeyValue("ClearRanking", ToString(m_uClearRanking));
	roLog.AddKeyValue("RankingType", ToString(m_uRankingType));
	roLog.AddKeyValue("BanTime", ToString(m_iBanTime));
	roLog.AddKeyValue("BanReason", m_strBanReason);
}

void TIdipBanPlay::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_uRoleId));
	roLog.AddKeyValue("BanTime", ToString(m_iBanTime));
	roLog.AddKeyValue("BanType", ToString(m_uBanType));
	roLog.AddKeyValue("PromptText", m_strPromptText);
}

void TIdipPunish::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_uRoleId));
	roLog.AddKeyValue("BanTime", ToString(m_iBanTime));
	roLog.AddKeyValue("BanReason", ToString(m_strBanReason));
}
void TIdipRelievePunish::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_uRoleId));
	roLog.AddKeyValue("RelieveZeroProfit", ToString(m_uRelieveZeroProfit));
	roLog.AddKeyValue("RelievePlayAll", ToString(m_uRelievePlayAll));
	roLog.AddKeyValue("RelieveBanJoinRank", ToString(m_uRelieveBanJoinRank));
	roLog.AddKeyValue("RelieveBan", ToString(m_uRelieveBan));
	roLog.AddKeyValue("RelieveMaskchat", ToString(m_uRelieveMaskchat));
}

void TIdipDoUnbanUsr::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_uRoleId));
}

void TIdipZeroProfit::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_uRoleId));
	roLog.AddKeyValue("ZeroProfitTime", ToString(m_iZeroProfitTime));
	roLog.AddKeyValue("Reason", m_strReason);
}

void TIdipUpdateScore::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_uRoleId));
	roLog.AddKeyValue("ScoreType", ToString(m_iScoreType));
	roLog.AddKeyValue("Num", ToString(m_iScore));
}

void TIdipDoDelItemOutBag::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_RoleId));
	roLog.AddKeyValue("Type", ToString(m_Type));
	roLog.AddKeyValue("ItemId", ToString(m_ItemID));
	roLog.AddKeyValue("ItemUId", ToString(m_ItemUid));
	roLog.AddKeyValue("DragonJadeId", ToString(m_JadeId));
}

void TIdipXinYueRPay::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_RoleId));
	roLog.AddKeyValue("Diamond", ToString(m_Num));
	roLog.AddKeyValue("ExtendParameter", m_strExtParam);
	roLog.AddKeyValue("MailTitle", m_strMailTitle);
	roLog.AddKeyValue("MailContent", m_strMailContent);
}

void TIdipItemChange::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_RoleId));
	roLog.AddKeyValue("ItemID", ToString(m_ItemId));
	roLog.AddKeyValue("ItemNum", ToString(m_ItemNum));
	roLog.AddKeyValue("Op", ToString(m_Op));
}

void TIdipUpdateGuildInfo::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("Num", ToString(m_Num));
	roLog.AddKeyValue("MailTitle", ProcessRoleName(strMailTitle));
	roLog.AddKeyValue("MailContent", ProcessRoleName(strMailContent));
	roLog.AddKeyValue("GuildID", ToString(m_GuildID));
}

void TIdipClearFightPunishFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_RoleId));
	roLog.AddKeyValue("OpenId", m_strOpenId);
}

void TGuildCastleRoleFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("RoleId", ToString(m_RoleID));
	UINT64 guildid = CGuildMgr::Instance()->GetGuildIdByRoleId(m_RoleID);
	roLog.AddKeyValue("GuildId", ToString(guildid));
	roLog.AddKeyValue("KillCount", ToString(m_KillCount));
	roLog.AddKeyValue("ZlCount", ToString(m_ZlCount));
	roLog.AddKeyValue("Feats", ToString(m_Feats));
}
