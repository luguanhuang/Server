#include "pch.h"
#include "tlogger.h"
#include "unit/role.h"
#include "loghelper.h"
#include "util.h"
#include "define/itemdef.h"
#include "gamelogic/bag.h"
#include "unit/rolemanager.h"
#include "config.h"
#include "appid.h"
#include "define/itemdef.h"
#include "define/tlogenum.h"
#include "gamelogic/payv2Record.h"
#include "gamelogic/pkrecord.h"
#include "gamelogic/rolesummary.h"
#include "gamelogic/rolesummarymgr.h"
#include "gamelogic/designationRecord.h"
#include "gamelogic/reportdatamgr.h"
#include "gamelogic/pvprecord.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/spriteconfig.h"
#include "gamelogic/spriterecord.h"
#include "gamelogic/sprite.h"
#include "gamelogic/titlerecord.h"
#include "gamelogic/itemconfig.h"
#include "reportdata/reportdata_def.h"
#include "gamelogic/reportdatarecord.h"
#include "gamelogic/activityrecord.h"
#include "gamelogic/randomattrmgr.h"
#include "idutil.h"
#include "gamelogic/military_rank_record.h"
#include "gamelogic/rolefashion.h"
#include "spactivity/spactivityrecord.h"
#include "spactivity/spactivitydatasmall.h"
#include "unit/unit.h"
#include "scene/scene.h"
#include "scene/sceneteam.h"
#include "gamelogic/dragonconfig.h"


INT32 GetTLogMoneyType(UINT32 dwItemID)
{
	if (dwItemID == GOLD)
	{
		return TX_MONEY;
	}
	if (dwItemID == DIAMOND)
	{
		return TX_DIAMOND;
	}
	return TX_DRAGON_COIN;
}

INT32 GetMoneyTypeFromTxType(INT32 nType)
{
	if (nType == TX_MONEY)
	{
		return GOLD;
	}
	if (nType == TX_DIAMOND)
	{
		return DIAMOND;
	}
	return DRAGON_COIN;
}
INT32 GetChatType(INT32 iType)
{
	switch (iType)
	{
	case KKSG::WorldChannel:
		{
			return TX_Chat_World;
		}
	case KKSG::GuildChannel:
		{
			return TX_Chat_Guild;
		}
	case KKSG::PrivateChannel:
		{
			return TX_Chat_Private;
		}
	case KKSG::TeamChannel:
		{
			return TX_Chat_Team;
		}
	case KKSG::CurrentChannel:
		{
			return TX_Chat_Current;
		}
	default:
		break;
	}
	return -1;
}

void TServerStateLog::Do()
{
	LogHelper log;
	log.SetDBType(KKSG::LOG_DB_TENCENT);
	log.SetTableName("GameSvrState");
	log.AddKeyValueTime("dtEventTime", time(NULL));
	log.AddKeyValue("vGameIP", GSConfig::Instance()->GetLocalIP());
	log.AddKeyValue("iZoneAreaID", GSConfig::Instance()->GetServerID());//这个是记录服务器状态的，使用Gsconfig;
	log.Send();
}
static std::string GetGameAppID(Role* role)
{
	if (role)
	{
		if (role->GetClientConf().logintype() == KKSG::LOGIN_QQ_PF)
		{
			return MobileQQ_App_ID;
		}
		if (role->GetClientConf().logintype() == KKSG::LGOIN_WECHAT_PF)
		{
			return WeChat_App_ID;
		}
	}
	return GSConfig::Instance()->GetAppType()==KKSG::GAME_APP_QQ?MobileQQ_App_ID:WeChat_App_ID;
}
void TRoleLogOld::Do()
{
	if (NULL == m_poRole)return;
	LogHelper log;
	log.SetDBType(KKSG::LOG_DB_TENCENT);
	log.SetTableName(GetTableName());

	log.AddKeyValue("GameSvrId", ToString(m_poRole->GetServerId()));
	log.AddKeyValueTime("dtEventTime", time(NULL));
	log.AddKeyValue("vGameAppid", GetGameAppID(m_poRole));
	log.AddKeyValue("PlatID", ToString(m_poRole->GetClientConf().platid()));
	log.AddKeyValue("iZoneAreaID", m_poRole->GetServerId());

	Build(log);
	log.Send();
}

void TRoleLog::Do()
{
	if (NULL == m_poRole)return;
	LogHelper log;
	log.SetDBType(KKSG::LOG_DB_TENCENT);
	log.SetTableName(GetTableName());

	log.AddKeyValue("TransTag", m_strTransTag);
	log.AddKeyValue("GameSvrId", ToString(m_poRole->GetServerId()));
	log.AddKeyValueTime("dtEventTime", time(NULL));
	log.AddKeyValue("vGameAppid", GetGameAppID(m_poRole));
	log.AddKeyValue("PlatID", ToString(m_poRole->GetClientConf().platid()));
	log.AddKeyValue("iZoneAreaID", m_poRole->GetServerId());
	log.AddKeyValue("vopenid", m_poRole->GetAccount());

	if (strcmp(GetTableName(), "ItemEnHance") != 0)
	{
		log.AddKeyValue("vRoleID", ToString(m_poRole->GetID()));
	}

	Build(log);
	log.Send();
}

void TCommonLog::Do()
{
	if (NULL == m_poRole)return;
	LogHelper log;
	log.SetDBType(KKSG::LOG_DB_TENCENT);
	log.SetTableName(GetTableName());
	Build(log);
	log.Send();
}

static void SetLogAttr(LogHelper& roLog, const std::string& name, INT32 i, const std::string& value)
{
	std::string str = name + ToString(i);
	roLog.AddKeyValue(str.c_str(), value);
}

void TLoginLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("vopenid", m_poRole->GetAccount());
	roLog.AddKeyValue("Level", ToString(m_poRole->GetLevel()));

	roLog.AddKeyValue("PlayerFriendsNum", m_poRole->GetFriendList()->size());

	roLog.AddKeyValue("ClientVersion", m_poRole->GetClientConf().clientversion());
	roLog.AddKeyValue("SystemSoftware", m_poRole->GetClientConf().systemsoftware());
	roLog.AddKeyValue("SystemHardware", m_poRole->GetClientConf().systemhardware());
	roLog.AddKeyValue("TelecomOper", m_poRole->GetClientConf().telecomoper());
	roLog.AddKeyValue("Network", m_poRole->GetClientConf().network());
	roLog.AddKeyValue("ScreenWidth", ToString(m_poRole->GetClientConf().screenwidth()));
	roLog.AddKeyValue("ScreenHight", ToString(m_poRole->GetClientConf().screenhight()));
	roLog.AddKeyValue("Density", ToString(m_poRole->GetClientConf().density()));
	roLog.AddKeyValue("LoginChannel", m_poRole->GetClientConf().loginchannel());
	roLog.AddKeyValue("vRoleID", ToString(m_poRole->GetID()));
	roLog.AddKeyValue("vRoleName", ProcessRoleName(m_poRole->GetName()));
	roLog.AddKeyValue("CpuHardware", m_poRole->GetClientConf().cpuhardware());
	roLog.AddKeyValue("Memory", ToString(m_poRole->GetClientConf().memory()));
	roLog.AddKeyValue("GLRender", m_poRole->GetClientConf().glrender());
	roLog.AddKeyValue("GLVersion", m_poRole->GetClientConf().glversion());
	roLog.AddKeyValue("DeviceId", m_poRole->GetClientConf().deviceid());
	roLog.AddKeyValue("TransTag", m_strTransTag);
	roLog.AddKeyValue("PowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("VipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("Profession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("DragonScalesNum", ToString(m_poRole->Get<Bag>()->CountItemInBag(TX_DragonScalesID)));
	roLog.AddKeyValue("UserIP", m_poRole->GetClientConf().ip());
	roLog.AddKeyValue("Gold", ToString(m_poRole->Get<Bag>()->CountVirtualItem(GOLD)));
	roLog.AddKeyValue("DragonCoin", ToString(m_poRole->Get<Bag>()->CountVirtualItem(DRAGON_COIN)));
	roLog.AddKeyValue("Diamond", ToString(m_poRole->Get<Bag>()->CountVirtualItem(DIAMOND)));
	roLog.AddKeyValue("ChannelMask", m_poRole->GetRoleAllInfoPtr()->extrainfo().channelmask()==true?1:0);
	SpActBackFlowData* backflow = static_cast<SpActBackFlowData*>(m_poRole->Get<SpActivityRecord>()->GetSpActData(SpActivity_BackFlow));
	roLog.AddKeyValue("BackFlow", backflow?1:0);
}

void TLogoutLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("vopenid", m_poRole->GetAccount());
	roLog.AddKeyValue("OnlineTime", ToString(m_poRole->GetOnlineTimeFromLogin()));
	roLog.AddKeyValue("Level", ToString(m_poRole->GetLevel()));

	roLog.AddKeyValue("PlayerFriendsNum", m_poRole->GetFriendList()->size());

	roLog.AddKeyValue("ClientVersion", m_poRole->GetClientConf().clientversion());
	roLog.AddKeyValue("SystemSoftware", m_poRole->GetClientConf().systemsoftware());
	roLog.AddKeyValue("SystemHardware", m_poRole->GetClientConf().systemhardware());
	roLog.AddKeyValue("TelecomOper", m_poRole->GetClientConf().telecomoper());
	roLog.AddKeyValue("Network", m_poRole->GetClientConf().network());
	roLog.AddKeyValue("ScreenWidth", ToString(m_poRole->GetClientConf().screenwidth()));
	roLog.AddKeyValue("ScreenHight", ToString(m_poRole->GetClientConf().screenhight()));
	roLog.AddKeyValue("Density", ToString(m_poRole->GetClientConf().density()));
	roLog.AddKeyValue("LoginChannel", m_poRole->GetClientConf().loginchannel());

	roLog.AddKeyValue("CpuHardware", m_poRole->GetClientConf().cpuhardware());
	roLog.AddKeyValue("Memory", ToString(m_poRole->GetClientConf().memory()));
	roLog.AddKeyValue("GLRender", m_poRole->GetClientConf().glrender());
	roLog.AddKeyValue("GLVersion", m_poRole->GetClientConf().glversion());
	roLog.AddKeyValue("DeviceId", m_poRole->GetClientConf().deviceid());
	roLog.AddKeyValue("TransTag", m_strTransTag);
	roLog.AddKeyValue("vRoleID", ToString(m_poRole->GetID()));
	roLog.AddKeyValue("VipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("PowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("Profession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("LadderPoint", ToString(m_poRole->Get<CPkRecord>()->GetPoint()));
	roLog.AddKeyValue("Gold", ToString(m_poRole->Get<Bag>()->CountVirtualItem(GOLD)));
	roLog.AddKeyValue("DragonCoin", ToString(m_poRole->Get<Bag>()->CountVirtualItem(DRAGON_COIN)));
	roLog.AddKeyValue("Diamond", ToString(m_poRole->Get<Bag>()->CountVirtualItem(DIAMOND)));
	std::string od = m_poRole->GetConfig("OD_VIEW");
	INT32 iOdType = od.empty()?0:convert<INT32>(od);
	roLog.AddKeyValue("OdType", ToString(iOdType));
	roLog.AddKeyValue("PandoraHeartNum", ToString(m_poRole->Get<Bag>()->CountItemInBag(TX_PandoraHeartID)));
	roLog.AddKeyValue("AbilityDesignationID", ToString(m_poRole->Get<DesignationRecord>()->GetAbilityDesignation()));
	roLog.AddKeyValue("CoverDesignationID", ToString(m_poRole->Get<DesignationRecord>()->GetCoverDesignation()));
	std::vector<UINT64> vecSprite;
	m_poRole->Get<CSpriteRecord>()->GetInFightList(vecSprite);
	std::stringstream ss;
	for (UINT32 i = 0; i < vecSprite.size(); i ++)
	{
		Sprite* pSprite = m_poRole->Get<CSpriteRecord>()->GetSprite(vecSprite[i]);
		if (pSprite)
		{
			if (i > 0)
			{
				ss << "#";
			}
			ss << pSprite->GetSpriteID() << "_" << pSprite->GetLevel() << "_" << SpriteConfig::Instance()->GetSpriteQuality(vecSprite[i]) << "_" << pSprite->GetEvolutionLevel();
		}
	}
	roLog.AddKeyValue("InFightSprite", ss.str());

	std::vector<XItem*> items;
	m_poRole->Get<Bag>()->GetBodyEquip(items);
	std::stringstream s;
	for (UINT32 i = 0; i < items.size(); i ++)
	{
		if (i) s << "#";
		s << items[i]->itemID << "_" << items[i]->GetEnhanceLevel() << "_" << items[i]->GetQuality(items[i]->itemID);
	}
	roLog.AddKeyValue("BodyEquip", s.str());
	roLog.AddKeyValue("TitleID", ToString(m_poRole->Get<TitleRecord>()->GetTitle()));
	roLog.AddKeyValue("ActivityValue", ToString(m_poRole->Get<CActivityRecord>()->GetActivityAllValue()));
	roLog.AddKeyValue("HairColorId", ToString(m_poRole->Get<RoleFashion>()->GetDispplayHairColor()));
	roLog.AddKeyValue("MilitaryRank", ToString(m_poRole->Get<MilitaryRankRecord>()->GetMilitaryRank()));
	roLog.AddKeyValue("MilitaryExploit", ToString(m_poRole->Get<MilitaryRankRecord>()->GetMilitaryExploit()));
}

void TMoneyFlowLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("vopenid", m_poRole->GetAccount());
	roLog.AddKeyValue("Sequence", "0");
	roLog.AddKeyValue("Level", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("AfterMoney", ToString(m_poRole->Get<Bag>()->CountVirtualItem(GetMoneyTypeFromTxType(m_nMoneyType))));

	roLog.AddKeyValue("iMoney", ToString(m_nMoneyChange));
	roLog.AddKeyValue("Reason", ToString(m_nReason));
	roLog.AddKeyValue("SubReason", m_nSubReason);
	roLog.AddKeyValue("AddOrReduce", ToString(m_nAddOrReduce));
	roLog.AddKeyValue("iMoneyType", ToString(m_nMoneyType));
	roLog.AddKeyValue("TransTag", m_strTransTag);
	roLog.AddKeyValue("vRoleID", ToString(m_poRole->GetID()));
	roLog.AddKeyValue("Profession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("VipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
}

void TItemFlowLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("vopenid", m_poRole->GetAccount());
	roLog.AddKeyValue("Level", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("Sequence", "0");
	roLog.AddKeyValue("iGoodsType", ToString(m_nType));
	roLog.AddKeyValue("iGoodsId", ToString(m_nItemID));
	roLog.AddKeyValue("Count", ToString(m_nCount));
	roLog.AddKeyValue("AfterCount", ToString(m_nAfterCount));
	roLog.AddKeyValue("Reason", ToString(m_nReason));
	roLog.AddKeyValue("SubReason", ToString(m_nSubReason));
	roLog.AddKeyValue("iMoney", ToString(m_nMoneyCost));
	roLog.AddKeyValue("iMoneyType", ToString(m_nMoneyType));
	roLog.AddKeyValue("AddOrReduce", ToString(m_nAddOrReduce));
	roLog.AddKeyValue("TransTag", m_strTransTag);
	roLog.AddKeyValue("vRoleID", ToString(m_poRole->GetID()));
	roLog.AddKeyValue("VipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	m_nQuality = ItemConfig::Instance()->GetItemQuailty(m_nItemID);
	roLog.AddKeyValue("Quality", ToString(m_nQuality));
	roLog.AddKeyValue("Profession", ToString(m_poRole->GetProfession()));
}

void TPlayerExpFlowLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("vopenid", m_poRole->GetAccount());
	roLog.AddKeyValue("ExpChange", ToString(m_nExpChange));
	roLog.AddKeyValue("BeforeLevel", m_nBeforeLevel);
	roLog.AddKeyValue("AfterLevel", ToString(m_nAfterLevel));
	roLog.AddKeyValue("Time", ToString(m_nTime));
	roLog.AddKeyValue("Reason", ToString(m_nReason));
	roLog.AddKeyValue("SubReason", ToString(m_nSubReason));
	roLog.AddKeyValue("TransTag", m_strTransTag);
	roLog.AddKeyValue("vRoleID", ToString(m_poRole->GetID()));
	roLog.AddKeyValue("WorldLevelScale", ToString(m_WorldLevelBuff));
	roLog.AddKeyValue("LevelSealScale", ToString(m_LevelSealBuff));
	roLog.AddKeyValue("CurExp", ToString(m_CurExp));
	roLog.AddKeyValue("PrePowerPoint", ToString(m_PrePowerPoint));
}

void TSnsFlowLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("ActorOpenID", m_poRole->GetAccount());
	roLog.AddKeyValue("RecNum", "0");
	roLog.AddKeyValue("Count", ToString(m_nCount));
	roLog.AddKeyValue("SNSType", ToString(m_nSNSType));
	roLog.AddKeyValue("SNSSubType", ToString(m_nSNSSubType));
	roLog.AddKeyValue("TransTag", m_strTransTag);
	roLog.AddKeyValue("vRoleID", ToString(m_poRole->GetID()));
}

void TRoundFlowLog::Build(LogHelper& roLog)
{
	m_Damge = (UINT64)m_poRole->Statistics().GetTotalDamage();
	m_BeHitDamage = (UINT64)m_poRole->Statistics().GetBeHitDamage();
	if (m_nBattleType == KKSG::SCENE_NEST || m_nBattleType == KKSG::SCENE_DRAGON)
	{
		if (m_nBattleType == KKSG::SCENE_DRAGON && m_poRole->GetCurrScene() && m_poRole->GetCurrScene()->GetSceneTeam())
		{
			UINT32 expid = m_poRole->GetCurrScene()->GetSceneTeam()->GetExpID();
			auto it = DragonConfig::Instance()->GetDragonTableData(expid);
			if (it && it->DragonNestDifficulty == 0)
			{
				ReportDataMgr::Instance()->Common(m_poRole, Tx_Reportdata_SimpleDragonPass, ToString(m_nBattleID%10+1));
			}
		}
		ReportDataMgr::Instance()->NestResult(m_poRole, m_nBattleID, m_nResult);
	}
	else if (m_nBattleType == KKSG::SCENE_PK)
	{
		m_poRole->Get<CPkRecord>()->AddPlayTime(m_nRoundTime);
		m_strPkWinRate = ReportDataMgr::Instance()->GetWinRate(m_poRole->Get<CPkRecord>()->GetWinAll(), m_poRole->Get<CPkRecord>()->GetCountAll());
		m_nRoundScore = m_poRole->Get<CPkRecord>()->GetPoint();
		m_iContinueWin = m_poRole->Get<CPkRecord>()->GetContWin();
		m_iContinueWinHis = m_poRole->Get<CPkRecord>()->GetContWinAll();
	}
	else if (m_nBattleType== KKSG::SCENE_PVP)
	{
		m_poRole->Get<MyPvpRecord>()->AddPlayTime(m_nRoundTime);
		ReportDataMgr::Instance()->Common(m_poRole, Tx_Reportdata_PvpDayTimes, ToString(m_poRole->Get<MyPvpRecord>()->GetTodayPlayTimes()));
		m_strPvpWinRate = ReportDataMgr::Instance()->GetWinRate(m_poRole->Get<MyPvpRecord>()->GetWinCountAll(), m_poRole->Get<MyPvpRecord>()->GetCountAll());
	}
	else if (m_nBattleType == KKSG::SCENE_BOSSRUSH && m_nResult == TX_BATTLE_SUCCESS)
	{
		ReportDataMgr::Instance()->Common(m_poRole, Tx_Reportdata_BossRushDayRank, ToString(m_nBattleID));
	}
	else if (m_nBattleType == KKSG::SCENE_WEEK_NEST)
	{
		m_poRole->Get<CReportDataRecord>()->UpdateWeekNestFastTime(m_nResult==TX_BATTLE_SUCCESS?m_nRoundTime:0);
	}
	else if (m_nBattleType == KKSG::SCENE_ABYSS_PARTY)
	{
		m_poRole->Get<CReportDataRecord>()->UpdateAbyssCount();
	}
	else if (m_nBattleType == KKSG::SCENE_CUSTOMPK)
	{
		ReportDataMgr::Instance()->Common(m_poRole, Tx_Reportdata_QuanMinPk, ToString(1));
	}

	roLog.AddKeyValue("vopenid", m_poRole->GetAccount());
	roLog.AddKeyValue("BattleID", ToString(m_nBattleID));
	roLog.AddKeyValue("BattleType", ToString(m_nBattleType));
	roLog.AddKeyValue("RoundScore", ToString(m_nRoundScore));
	roLog.AddKeyValue("RoundTime", ToString(m_nRoundTime));
	roLog.AddKeyValue("Result", ToString(m_nResult));
	roLog.AddKeyValue("Rank", ToString(m_nRank));
	roLog.AddKeyValue("Gold", ToString(m_nGold));
	roLog.AddKeyValue("TransTag", m_strTransTag);
	roLog.AddKeyValue("vRoleID", ToString(m_poRole->GetID()));
	roLog.AddKeyValue("Level", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("VipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("Profession", ToString(m_poRole->GetProfession()));//ְҵ
	roLog.AddKeyValue("PowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("Star", ToString(m_nStar));
	roLog.AddKeyValue("Team", ToString(m_nTeam));
	roLog.AddKeyValue("TopFloor", ToString(m_nTopFloor));
	roLog.AddKeyValue("iStageTime", ToString(m_iStageTime));
	roLog.AddKeyValue("iHpPercent", ToString(m_iHpPercent));
	roLog.AddKeyValue("iReviveCount", ToString(m_iReviveCount));
	roLog.AddKeyValue("Helper", ToString(m_iHelper));
	roLog.AddKeyValue("Count", ToString(m_iCount));
	roLog.AddKeyValue("AutoFight", ToString(m_iAutoFight));
	roLog.AddKeyValue("PkWinRate", ToString(m_strPkWinRate));
	roLog.AddKeyValue("PvpWinRate", ToString(m_strPvpWinRate));

	roLog.AddKeyValue("ContinueWin",ToString(m_iContinueWin));
	roLog.AddKeyValue("ContinueWinHis",ToString(m_iContinueWinHis));
	roLog.AddKeyValue("IsCross",ToString(GSConfig::Instance()->IsCrossGS() ? 1 : 0));
	roLog.AddKeyValue("TeamID", ToString(m_TeamID));
	roLog.AddKeyValue("TeamMemberCount", ToString(m_TeamCount));
	roLog.AddKeyValue("TeamSurviveCount", ToString(m_TeamSurviveCount));
	roLog.AddKeyValue("Damge", ToString(m_Damge));
	roLog.AddKeyValue("BeHitDamage", ToString(m_BeHitDamage));
	roLog.AddKeyValue("Uid", ToString(m_Uid));
	roLog.AddKeyValue("DeathCount", ToString(m_DeathCount));
	roLog.AddKeyValue("BrID", ToString(m_BrID));
	roLog.AddKeyValue("Brbid1", ToString(m_Brbid1));
	roLog.AddKeyValue("Brbid2", ToString(m_Brbid2));
	roLog.AddKeyValue("BrrefreshCount", ToString(m_BrrefreshCount));
}
void TGuideFlowLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("vopenid", m_poRole->GetAccount());
	roLog.AddKeyValue("iGuideID", ToString(m_nGuideID));
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("vRoleID", ToString(m_poRole->GetID()));
	roLog.AddKeyValue("TransTag", m_strTransTag);
	roLog.AddKeyValue("VipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
}

void TVipLevelFlowLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("vopenid", m_poRole->GetAccount());
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iBeforeVipLevel", ToString(m_nBeforeVipLevel));
	roLog.AddKeyValue("iAfterVipLevel", ToString(m_nAfterVipLevel));
	roLog.AddKeyValue("vRoleID", ToString(m_poRole->GetID()));
	roLog.AddKeyValue("TransTag", m_strTransTag);
}

void TActivityFlowLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("vopenid", m_poRole->GetAccount());
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_nVipLevel));
	roLog.AddKeyValue("iActivityType", ToString(m_nActivityType));
	roLog.AddKeyValue("iActivityID", ToString(m_nActivityID));
	roLog.AddKeyValue("vRoleID", ToString(m_poRole->GetID()));
	roLog.AddKeyValue("TransTag", m_strTransTag);
}

void TTaskFlowLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("vopenid", m_poRole->GetAccount());
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iTaskType", ToString(m_nTaskType));
	roLog.AddKeyValue("iTaskID", ToString(m_nTaskID));
	roLog.AddKeyValue("iState", ToString(m_nState));
	roLog.AddKeyValue("vRoleID", ToString(m_poRole->GetID()));
	roLog.AddKeyValue("TransTag", m_strTransTag);
}

void TLotteryFlowLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("vopenid", m_poRole->GetAccount());
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_nVipLevel));
	roLog.AddKeyValue("iLotteryType", ToString(m_nLotteryType));
	roLog.AddKeyValue("iMoney", ToString(m_nMoney));
	roLog.AddKeyValue("vRoleID", ToString(m_poRole->GetID()));
	roLog.AddKeyValue("TransTag", m_strTransTag);
}

void TFatigueLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("Level", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("VipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("AfterFatigue", ToString(m_poRole->Get<Bag>()->CountVirtualItem(FATIGUE)));
	roLog.AddKeyValue("iFatigue", ToString(m_nFatigueChange));
	roLog.AddKeyValue("Reason", ToString(m_nReason));
	roLog.AddKeyValue("SubReason", m_nSubReason);
	roLog.AddKeyValue("AddOrReduce", ToString(m_nAddOrReduce));
	roLog.AddKeyValue("SceneType", ToString(m_nSceneType));
}

void TSweepLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("Level", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("VipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("Profession", ToString(m_poRole->GetProfession()));//ְҵ
	roLog.AddKeyValue("PowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("BattleID", ToString(m_nBattleID));
	roLog.AddKeyValue("BattleType", ToString(m_nBattleType));
	roLog.AddKeyValue("Result", ToString(m_nResult));
	roLog.AddKeyValue("Count", ToString(m_nCount));
}

void TUIGuideFlowLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("Level", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("VipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("Arg", arg);
	roLog.AddKeyValue("Position", ToString(m_nPosition));
}

void TItemDesc::SetLogHelper(LogHelper& roLog, INT32 ItemNum)
{
	for (UINT32 i = 0; i < ItemNum; i ++)
	{
		std::string strItemID = "ItemID" + ToString(i);
		std::string strItemNum = "ItemNum" + ToString(i);
		ItemDesc oitem;
		if (i < m_vList.size())
		{
			oitem.itemID = m_vList[i].itemID;
			oitem.itemCount = m_vList[i].itemCount;
		}
		roLog.AddKeyValue(strItemID.c_str(), ToString(oitem.itemID));
		roLog.AddKeyValue(strItemNum.c_str(), ToString(oitem.itemCount));
	}
}

void TItemEnHanceLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iOperType", ToString(m_nOperType));
	roLog.AddKeyValue("iChgLevel", ToString(m_nChgLevel));
	roLog.AddKeyValue("iAfterLevel", ToString(m_nAfterLevel));
	roLog.AddKeyValue("vRoleID", ToString(m_poRole->GetID()));
	roLog.AddKeyValue("iItemID", ToString(m_iItemID));
	roLog.AddKeyValue("uid", ToString(m_lUID));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));//
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("ItemID2", ToString(m_iItemID2));
	roLog.AddKeyValue("ItemLevel2", ToString(m_iLevel2));
	roLog.AddKeyValue("uid2", ToString(m_lUID2));
	m_Quality = ItemConfig::Instance()->GetItemQuailty(m_iItemID);
	roLog.AddKeyValue("Quality", ToString(m_Quality));
	m_Quality2 = ItemConfig::Instance()->GetItemQuailty(m_iItemID2);
	roLog.AddKeyValue("Quality2", ToString(m_Quality2));
	m_ItemList.SetLogHelper(roLog, TXLOG_ITEMENCHANGCE_COSTNUM);
}

void TPkLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_nVipLevel));
	roLog.AddKeyValue("iEmenyPro", m_nEmenyPro);
	roLog.AddKeyValue("EmenyName", ProcessRoleName(strEmenyName));
	roLog.AddKeyValue("iEmenyRoleID", m_nEmenyRoleID);
	roLog.AddKeyValue("iPKResultID", m_nResultID);
	roLog.AddKeyValue("iBeforePoint", m_nBeforePoint);
	roLog.AddKeyValue("iAfterPoint", m_nAfterPoint);
	roLog.AddKeyValue("iSelfPro", m_poRole->GetProfession());
	roLog.AddKeyValue("Name", ProcessRoleName(m_poRole->GetName()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("iEmenyPowerPoint", m_nEmenyPowerPoint);
}
void TOpenSystemLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iSystemID", m_nSystemID);
	roLog.AddKeyValue("Profession", ToString(m_poRole->GetProfession()));
}

void TOdViewFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iOdType", ToString(m_nOdType));
}

void TGuildFightFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("Profession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("PowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("GuildID", ToString(m_nGuildID));
	roLog.AddKeyValue("ChallengeType", ToString(m_nChallengeType));
}

void TGuildArenaFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("Profession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("PowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("GuildID", ToString(m_uGuildID));
	if (m_uRoleId2)
	{
		Role* role2 = RoleManager::Instance()->FindByRoleID(m_uRoleId2);
		if (role2)
		{
			m_uRoleId2 = role2->GetID();
			m_strOpenId2 = role2->GetAccount();
			m_iProfession2 = role2->GetProfession();
			m_iLevel2 = role2->GetLevel();
			m_iVipLevel2 = role2->Get<CPayV2Record>()->GetVipLevel();
			m_iPowerPoint2 = (UINT32)role2->GetAttr(TOTAL_POWERPOINT);
		}
	}
	roLog.AddKeyValue("RoleId2", ToString(m_uRoleId2));
	roLog.AddKeyValue("OpenId2", m_strOpenId2);
	roLog.AddKeyValue("iLevel2", ToString(m_iLevel2));
	roLog.AddKeyValue("iVipLevel2", ToString(m_iVipLevel2));
	roLog.AddKeyValue("Profession2", ToString(m_iProfession2));
	roLog.AddKeyValue("PowerPoint2", ToString(m_iPowerPoint2));
	roLog.AddKeyValue("GuildID2", ToString(m_uGuildID2));
	roLog.AddKeyValue("Point", ToString(m_iPoint));
	roLog.AddKeyValue("OperType", ToString(m_iOp));
}

void TBossRushRefreshFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("CurRefreshCount", ToString(m_nCurRefreshCount));
	roLog.AddKeyValue("iBossRank", ToString(m_iCurBossCount));
}
void TAuctionFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iItemID", ToString(m_nItemID));
	roLog.AddKeyValue("ItemName", szItemName);
	roLog.AddKeyValue("iItemCount", ToString(m_nItemCount));
	roLog.AddKeyValue("iPrice", ToString(m_nPrice));
	roLog.AddKeyValue("iOperType", ToString(m_nOperType));
} 

void TPayFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iAmount", ToString(m_iAmount));
	roLog.AddKeyValue("ParamID", m_szParamID);
	roLog.AddKeyValue("OrderID", m_szOrderID);
	roLog.AddKeyValue("UserIP", m_poRole->GetClientConf().ip());
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	SpActBackFlowData* backflow = static_cast<SpActBackFlowData*>(m_poRole->Get<SpActivityRecord>()->GetSpActData(SpActivity_BackFlow));
	roLog.AddKeyValue("BackFlow", backflow?1:0);
}

void TShopFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iShopType", ToString(m_iShopType));
	roLog.AddKeyValue("iMoneyType", ToString(m_iMoneyType));
	roLog.AddKeyValue("iMoneyValue", ToString(m_iMoneyValue));
	roLog.AddKeyValue("iMoneyValueExt", ToString(m_iMoneyValueExt));
	roLog.AddKeyValue("iItemID", ToString(m_iItemID));
	roLog.AddKeyValue("iItemCount", ToString(m_iItemCount));
	roLog.AddKeyValue("iGoodsType", ToString(m_iGoodsType));
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("iLimitFlag", ToString(m_iLimitFlag));
}

void TSendFlowerFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iSendItemID", ToString(m_iSendItemID));
	roLog.AddKeyValue("iSendCount", ToString(m_iSendCount));
	roLog.AddKeyValue("iCostID", ToString(m_iCostID));
	roLog.AddKeyValue("iCostNum", ToString(m_iCostNum));
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("ToRoleId", ToString(m_ToRoleId));
}

void TGuildCardFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iOperType", ToString(m_iOperType));
	roLog.AddKeyValue("GuildID", ToString(m_lGuildID));
	roLog.AddKeyValue("iGuildLevel", ToString(m_iGuildLevel));
}

void TAnswerFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iAnswerType", ToString(m_iAnswerType));
	roLog.AddKeyValue("iResultID", ToString(m_iResultID));
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("QuestionID", ToString(m_uQuestionID));
	roLog.AddKeyValue("QuestionNo", ToString(m_iIndex));
}

void TSpriteFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("Uid", ToString(m_uUid));
	roLog.AddKeyValue("SpriteID", ToString(m_uSpriteID));
	roLog.AddKeyValue("SkillID", ToString(m_uSkillID));
	roLog.AddKeyValue("SpriteLevel", ToString(m_uLevel));
	roLog.AddKeyValue("EvolutionLevel", ToString(m_uEvolutionLevel));
	roLog.AddKeyValue("SpriteExp", ToString(m_uExp));
	roLog.AddKeyValue("SpritePowerPoint", ToString(m_uPowerPoint));
	roLog.AddKeyValue("iOperType", ToString(m_iOperType));
	roLog.AddKeyValue("ChangeValue", ToString(m_iChangeValue));	
	roLog.AddKeyValue("Reason", ToString(m_iReason));
	roLog.AddKeyValue("SubReason", ToString(m_iSubReason));
	roLog.AddKeyValue("PassiveSkillID", m_strPassiveSkillID);
	m_Quality = SpriteConfig::Instance()->GetSpriteQuality(m_uSpriteID);
	roLog.AddKeyValue("Quality", ToString(m_Quality));
}
void TPetFlow::SetData(Pet* pet)
{
	if (NULL == pet)
	{
		return ;
	}
	m_uUid = pet->uid;
	m_uPetID = pet->petid;
	m_uLevel = pet->level;
	m_uExp = pet->exp;
	m_uSex = pet->sex;
	m_uPower = pet->power;
	m_uMood = pet->mood;
	m_uHunary = pet->hungry;
	std::stringstream ss;
	for (UINT32 i = 0; i < pet->fixedSkills.size(); ++ i)
	{
		if (i > 0)ss << ",";
		ss << pet->fixedSkills[i];
	}
	m_strFixedSkills = ss.str();
	ss.clear();
	for (UINT32 i = 0; i < pet->randSkills.size(); ++ i)
	{
		if (i > 0)ss << ",";
		ss << pet->randSkills[i];
	}
	m_strRandSkills = ss.str();
}
void TPetFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("Uid", ToString(m_uUid));
	roLog.AddKeyValue("PetID", ToString(m_uPetID));
	roLog.AddKeyValue("PetLevel", ToString(m_uLevel));
	roLog.AddKeyValue("PetExp", ToString(m_uExp));
	roLog.AddKeyValue("Sex", ToString(m_uSex));
	roLog.AddKeyValue("PetPower", ToString(m_uPower));
	roLog.AddKeyValue("Mood", ToString(m_uMood));
	roLog.AddKeyValue("Hunary", ToString(m_uHunary));
	roLog.AddKeyValue("PetSeats", ToString(m_poRole->Get<PetSys>()->GetSeat()));
	roLog.AddKeyValue("iOperType", ToString(m_iOperType));
	roLog.AddKeyValue("FollowPetId", ToString(m_poRole->Get<PetSys>()->GetFollowPetId()));
	roLog.AddKeyValue("FightPetId", ToString(m_poRole->Get<PetSys>()->GetFightPetId()));
	roLog.AddKeyValue("FixedSkills", m_strFixedSkills);
	roLog.AddKeyValue("RandSkills", m_strRandSkills);
	roLog.AddKeyValue("ChangeValue", ToString(m_iChangeValue));
}

void TSuperRiskFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("MapID", ToString(m_iMapID));
	roLog.AddKeyValue("Curx", ToString(m_iCurx));
	roLog.AddKeyValue("Cury", ToString(m_iCury));
	roLog.AddKeyValue("Direction", ToString(m_iDirection));
}

void TDesignationFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("DesignationID", ToString(m_uDesignationID));
	roLog.AddKeyValue("iOperType", ToString(m_iOperType));
}
void TSkyCityFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("iResult", ToString(m_iResult));
	roLog.AddKeyValue("Team", ToString(m_TeamID));
}

void TResWarFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	INT32 iResult = 0;
	if(m_iResult == 0)
	{
		iResult = TX_PKResult_Draw;
	}
	else if (m_iResult == -1)
	{
		iResult = TX_PKResult_Lost;
	}
	else
	{
		iResult = TX_PkResult_Win;
	}
	roLog.AddKeyValue("iResult", ToString(iResult));
}

void TSceneFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLineID", ToString(GSConfig::Instance()->GetLine()));
	roLog.AddKeyValue("iStatus", ToString(m_iStatus));
	roLog.AddKeyValue("iSceneType", ToString(m_iSceneType));
	roLog.AddKeyValue("iTotalTime", ToString(m_iTotalTime));
	roLog.AddKeyValue("iTotalRoleNum", ToString(m_iRoleNum));

	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("iWatch", ToString(m_iWatch));
}

void TGuildInheritFlow::Build(LogHelper& roLog)
{
	if (pInheritRole == NULL) return;
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("OpenId2", pInheritRole->GetAccount());
	roLog.AddKeyValue("RoleId2", ToString(pInheritRole->GetID()));
	roLog.AddKeyValue("iLevel2", ToString(pInheritRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel2", ToString(pInheritRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession2", ToString(pInheritRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint2", ToString((UINT32)pInheritRole->GetAttr(TOTAL_POWERPOINT)));
	ReportDataMgr::Instance()->Common(m_poRole, Tx_Reportdata_GuildInheritTimes, "1");
	ReportDataMgr::Instance()->Common(pInheritRole, Tx_Reportdata_GuildInheritTimes, "1");
}

void TDanceFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iOperType", ToString(m_iOp));
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
}

void TRoundUnlockLog::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("BattleType", ToString(m_BattleType));
	roLog.AddKeyValue("BattleID", ToString(m_BattleID));	
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
}

void TProfessionFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iOp", ToString(m_Op));
	roLog.AddKeyValue("iPreProfession", ToString(m_PreProfession));
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
}

void TTitleFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("TitleID", ToString(m_poRole->Get<TitleRecord>()->GetTitle()));
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
}

void TEmblemFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	ItemConf* pConf = ItemConfig::Instance()->GetItemConf(m_EmblemID);
	if (pConf)
	{
		m_EmblemLv = pConf->ReqLevel;
		m_QualityID = pConf->ItemQuality;
	}
	roLog.AddKeyValue("EmblemID", ToString(m_EmblemID));
	roLog.AddKeyValue("EmblemUid", ToString(m_EmblemUid));
	roLog.AddKeyValue("EmblemLv", ToString(m_EmblemLv));
	roLog.AddKeyValue("QualityID", ToString(m_QualityID));
	roLog.AddKeyValue("Op", ToString(m_Op));
	roLog.AddKeyValue("ThirdSlot", ToString(m_ThirdSlot));
	roLog.AddKeyValue("SmeltAttrID", ToString(m_SmeltAttrID));
	roLog.AddKeyValue("AttrNum", ToString(m_AttrNum));
	for (UINT32 i = 0; i < m_vAttrList.size(); i ++)
	{
		std::string strAttrID = "AttrID" + ToString(i);
		std::string strAttrValue = "AttrValue" + ToString(i);
		std::string strAttrFirst = "AttrFirst" + ToString(i);
		std::string strAttrSecond = "AttrSecond" + ToString(i);
		roLog.AddKeyValue(strAttrID.c_str(), ToString(m_vAttrList[i].m_AttrID));
		roLog.AddKeyValue(strAttrValue.c_str(), ToString(m_vAttrList[i].m_AttrValue));
		roLog.AddKeyValue(strAttrFirst.c_str(), ToString(m_vAttrList[i].m_AttrFirst));
		roLog.AddKeyValue(strAttrSecond.c_str(), ToString(m_vAttrList[i].m_AttrSecond));
	}

	m_ItemList.SetLogHelper(roLog, TXLOG_EMBLE_ITEMNUM);
}

void TItemSysFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("ItemID", ToString(m_ItemID));
	roLog.AddKeyValue("ItemUid", ToString(m_ItemUid));
	roLog.AddKeyValue("ItemLv", ToString(m_ItemLv));
	roLog.AddKeyValue("QualityID", ToString(m_Quality));
	roLog.AddKeyValue("Op", ToString(m_Op));
	roLog.AddKeyValue("EnchantID", ToString(m_EnchantID));
	roLog.AddKeyValue("JadeID", ToString(m_JadeID));
	roLog.AddKeyValue("PreJadeId", ToString(m_PreJadeID));
	m_Attr.SetBaseLogHelper(roLog);
	m_ItemList.SetLogHelper(roLog, TXLOG_ITEM_COSTNUM);
	roLog.AddKeyValue("FuseLevel", ToString(m_FuseLevel));
	roLog.AddKeyValue("FuseExp", ToString(m_FuseExp));
}

void TSkillFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("Page", ToString(m_Page));
	roLog.AddKeyValue("skill1", skill1);
	roLog.AddKeyValue("skill2", skill2);
}

void THeroBattleBuyFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("CostID", ToString(m_CostID));
	roLog.AddKeyValue("CostNum", ToString(m_CostNum));
	roLog.AddKeyValue("HeroID", ToString(m_HeroID));
}

void THeroBattleRoundFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("Elo", ToString(m_Elo));
	roLog.AddKeyValue("NewElo", ToString(m_NewElo));
	roLog.AddKeyValue("Result", ToString(m_Result));
	roLog.AddKeyValue("KillNum", ToString(m_KillNum));
	roLog.AddKeyValue("DeadNum", ToString(m_DeadNum));
	roLog.AddKeyValue("IsMvp", ToString(m_IsMvp));
	roLog.AddKeyValue("HaveHero", m_strHaveHero);
	roLog.AddKeyValue("FreeWeekHero", m_strFreeWeekHero);
}

void THeroBattleResultFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("UseHeroID", ToString(m_UseHeroID));
	roLog.AddKeyValue("KillNum", ToString(m_KillNum));
	roLog.AddKeyValue("LifeTime", ToString(m_LifeTime));
	roLog.AddKeyValue("Damage", ToString(m_Damage));
	roLog.AddKeyValue("Order", ToString(m_Order));
	roLog.AddKeyValue("IsChange", ToString(m_IsChange));
	roLog.AddKeyValue("IsDead", ToString(m_IsDead));
}

void TActiveDegreeFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("ActivityId", m_ActivityId);
	roLog.AddKeyValue("ValueChg", m_Value);
	roLog.AddKeyValue("TotalValue", m_TotalValue);
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
}

void TSpactivityFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("ActId", m_ActId);
	roLog.AddKeyValue("TaskId", m_TaskId);
	roLog.AddKeyValue("State", m_State);
	roLog.AddKeyValue("Progress", m_Progress);
	roLog.AddKeyValue("FinishTaskCount", m_FinishTaskCount);
	roLog.AddKeyValue("TotalTaskCount", m_TotalTaskCount);
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
}

void TInvFightFlow::Build(LogHelper& roLog)
{
	if (m_pRoleFight == NULL)return;
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iVipLevel", ToString(m_poRole->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("vopenid2", m_pRoleFight->GetAccount());
	roLog.AddKeyValue("vRoleID2", ToString(m_pRoleFight->GetID()));
	roLog.AddKeyValue("iLevel2", ToString(m_pRoleFight->GetLevel()));
	roLog.AddKeyValue("iVipLevel2", ToString(m_pRoleFight->Get<CPayV2Record>()->GetVipLevel()));
	roLog.AddKeyValue("iProfession2", ToString(m_pRoleFight->GetProfession()));
	roLog.AddKeyValue("iPowerPoint2", ToString((UINT32)m_pRoleFight->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("Result", ToString(m_Result));
	roLog.AddKeyValue("IsCross",ToString(GSConfig::Instance()->IsCrossGS() ? 1 : 0));
}

void TChangeProFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iPreProfession", ToString(m_PrePro));
	roLog.AddKeyValue("iProfession", ToString(m_CurPro));
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
}

void TAtlasFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("AtlasGroupId", ToString(m_GroupId));
	roLog.AddKeyValue("AtlasTeamId", ToString(m_TeamId));
	roLog.AddKeyValue("AtlasPowerPoint", ToString(m_AtlasPowerPoint));
	roLog.AddKeyValue("Num", ToString(m_Num));
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("Round", ToString(m_Round));
	roLog.AddKeyValue("op", ToString(m_op));
}

void TLeagueBattleFlow::Do()
{
	LogHelper log;
	log.SetDBType(KKSG::LOG_DB_TENCENT);
	log.SetTableName(GetTableName());
	log.AddKeyValueTime("dtEventTime", time(NULL));
	log.AddKeyValue("PlatID", ToString(GSConfig::Instance()->GetPlatType()));
	Build(log);
	log.Send();
}

void TLeagueBattleFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("WinLeagueId", ToString(m_winLeagueId));
	roLog.AddKeyValue("LoseLeagueId", ToString(m_loseLeagueId));
	roLog.AddKeyValue("WinInfo", m_strWinInfo);
	roLog.AddKeyValue("LoseInfo", m_strLoseInfo);
}

void TRoleChestFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("SuitId", ToString(m_SuitId));
	roLog.AddKeyValue("RealCount", ToString(m_RealCount));
	roLog.AddKeyValue("TotalCount", ToString(m_TotalCount));
}

void TItemReturnFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("ItemId", ToString(m_ItemId));
	roLog.AddKeyValue("ItemUid", ToString(m_ItemUid));
	roLog.AddKeyValue("BackItemId", ToString(m_BackItemId));
	roLog.AddKeyValue("BackItemNum", ToString(m_BackItemNum));
}

void TPkTwoFlow::Build(LogHelper& roLog)
{
	m_Damge = (UINT64)m_poRole->Statistics().GetTotalDamage();
	m_BeHitDamage = (UINT64)m_poRole->Statistics().GetBeHitDamage();
	auto it = m_poRole->Get<CPkRecord>()->GetSubInfo(KKSG::PK_2v2);
	if (it != NULL)
	{
		m_Point = it->point();
	}
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("Group", ToString(m_Group));
	roLog.AddKeyValue("Point", ToString(m_Point));
	roLog.AddKeyValue("Result", ToString(m_Result));
	roLog.AddKeyValue("Damge", ToString(m_Damge));
	roLog.AddKeyValue("BeHitDamage", ToString(m_BeHitDamage));
}

void TArtifactFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("ArtifactID", ToString(m_ArtifactID));
	roLog.AddKeyValue("ArtifactUid", ToString(m_ArtifactUid));
	roLog.AddKeyValue("QualityID", ToString(m_QualityID));
	roLog.AddKeyValue("EffectID", ToString(m_EffectID));
	roLog.AddKeyValue("Op", ToString(m_Op));
	for (UINT32 i = 0; i < TXLOG_ArtifactEffectNum; i ++)
	{
		std::string strEffectID = "EffectID" + ToString(i);
		UINT32 effectid = 0;
		if (i < m_EffectIDList.size())
		{
			effectid = m_EffectIDList[i];
		}
		roLog.AddKeyValue(strEffectID.c_str(), ToString(effectid));
	}

	for (UINT32 i = 0; i < TXLOG_ArtifactAttrNum; i ++)
	{
		std::string strArtifactAttrID = "ArtifactAttrID" + ToString(i);
		std::string strArtifactAttrValue = "ArtifactAttrValue" + ToString(i);
		std::string strArtifactAttrFirst = "ArtifactAttrFirst" + ToString(i);
		std::string strArtifactAttrSecond = "ArtifactAttrSecond" + ToString(i);
		TBaseAttr attr;
		if (i < m_AttrList.size())
		{
			attr = m_AttrList[i];
		}
		roLog.AddKeyValue(strArtifactAttrID.c_str(), ToString(attr.m_AttrID));
		roLog.AddKeyValue(strArtifactAttrValue.c_str(), ToString(attr.m_AttrValue));
		roLog.AddKeyValue(strArtifactAttrFirst.c_str(), ToString(attr.m_AttrFirst));
		roLog.AddKeyValue(strArtifactAttrSecond.c_str(), ToString(attr.m_AttrSecond));
	}
	roLog.AddKeyValue("FuseResult", ToString(m_FuseResult));
	roLog.AddKeyValue("FuseUsedStone", ToString(m_FuseUsedStone));
	roLog.AddKeyValue("InscriptionOp", ToString(m_InscriptionOp));
	roLog.AddKeyValue("InscriptionSkillEffectID", ToString(m_InscriptionSkillEffectID));
}

void TLogoutBodyItem::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("ItemID", ToString(m_ItemID));
	roLog.AddKeyValue("ItemUid", ToString(m_ItemUid));
	roLog.AddKeyValue("ItemLv", ToString(m_ItemLv));
	roLog.AddKeyValue("QualityID", ToString(m_QualityID));
	roLog.AddKeyValue("JadeSlotNum", ToString(m_JadeSlotNum));
	roLog.AddKeyValue("JadeTotalSlotNum", ToString(m_JadeTotalSlotNum));
	for (UINT32 i = 0; i < m_vJadeList.size(); i ++)//5个
	{
		std::string strJade = "Jade" + ToString(i);
		roLog.AddKeyValue(strJade.c_str(), m_vJadeList[i]);
	}
	m_Attr.SetLogHelper(roLog);
	roLog.AddKeyValue("FuseLevel", ToString(m_FuseLevel));
	roLog.AddKeyValue("FuseExp", ToString(m_FuseExp));
}


void TBigMeleeFightFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("GameZoneid", ToString(m_nGroupSvr));
	roLog.AddKeyValue("Killer", ToString(m_nKiller));
	roLog.AddKeyValue("Score", ToString(m_nScore));
	roLog.AddKeyValue("Dead", ToString(m_nDead));
	roLog.AddKeyValue("Rank", ToString(m_nRank));
	roLog.AddKeyValue("JoinTime", ToString(m_nJoinTime));
}

void TSpriteLogoutFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("Uid", ToString(m_uid));
	roLog.AddKeyValue("SpriteID", ToString(m_spriteID));
	roLog.AddKeyValue("SkillID", ToString(m_skillID));
	roLog.AddKeyValue("SpriteLevel", ToString(m_spritelevel));
	roLog.AddKeyValue("EvolutionLevel", ToString(m_evolutionLevel));
	roLog.AddKeyValue("SpriteExp", ToString(m_uExp));
	roLog.AddKeyValue("SpritePowerPoint", ToString(m_spritepowerpoint));
	roLog.AddKeyValue("Quality", ToString(m_Quality));
	roLog.AddKeyValue("IsCaptain", m_IsCaptain==true?1:0);
	for (INT32 i = 0; i < TXLOG_SPRITE_ADDATTR_NUM; i ++)
	{
		TBaseAttr attr;
		if (i < m_vAddAttrList.size())
		{
			attr = m_vAddAttrList[i];
		}
		SetLogAttr(roLog, "AddAttrID", i, ToString(attr.m_AttrID));
		SetLogAttr(roLog, "AddAttrValue", i, ToString(attr.m_AttrValue));
		SetLogAttr(roLog, "AddAttrFirst", i, ToString(attr.m_AttrFirst));
		SetLogAttr(roLog, "AddAttrSecond", i, ToString(attr.m_AttrSecond));
	}

	auto it = m_attr.begin();
	for (INT32 i = 0; i < TXLOG_SPRITE_ADDATTR_NUM; i ++)
	{
		UINT32 id = 0;
		UINT32 value = 0;
		if (it != m_attr.end())
		{
			id = it->first;
			value = UINT32(it->second);
			it ++;
		}
		SetLogAttr(roLog, "AttrID", i, ToString(id));
		SetLogAttr(roLog, "AttrValue", i, ToString(value));
	}

	auto iter = m_evoAttr.begin();
	for (INT32 i = 0; i < TXLOG_SPRITE_ATTRNUM; i ++)
	{
		UINT32 id = 0;
		UINT32 value = 0;
		if (iter != m_evoAttr.end())
		{
			id = iter->first;
			value = UINT32(iter->second);
			iter ++;
		}
		SetLogAttr(roLog, "EvoAttrID", i, ToString(id));
		SetLogAttr(roLog, "EvoAttrValue", i, ToString(value));
	}
}

void TRoleHairFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("HairId", ToString(m_HairId));
	roLog.AddKeyValue("HairColorId", ToString(m_HairColorId));
}

void TSharePhotoFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("Op", ToString(m_op));
	roLog.AddKeyValue("Type", ToString(m_type));
}

void TFashionComopseFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("FashionId", ToString(m_FashionId));
	roLog.AddKeyValue("FashionQuality", ItemConfig::Instance()->GetItemQuailty(m_FashionId));
	roLog.AddKeyValue("Result", ToString(m_Result));
	roLog.AddKeyValue("TotalSuccessRate", ToString(m_TotalSuccessRate));
	roLog.AddKeyValue("FailRate", ToString(m_FailRate));
	roLog.AddKeyValue("FashionId1", ToString(m_FashionId1));
	roLog.AddKeyValue("FashionUid1", ToString(m_FashionUid1));
	roLog.AddKeyValue("FashionQuality1", ItemConfig::Instance()->GetItemQuailty(m_FashionId1));
	roLog.AddKeyValue("FashionId2", ToString(m_FashionId2));
	roLog.AddKeyValue("FashionUid2", ToString(m_FashionUid2));
	roLog.AddKeyValue("FashionQuality2", ItemConfig::Instance()->GetItemQuailty(m_FashionId2));
}

void TWeekTaskFlow::Build(LogHelper& roLog)
{
	roLog.AddKeyValue("iLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("iProfession", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("iPowerPoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("TaskId", ToString(m_TaskId));
	roLog.AddKeyValue("Quality", ToString(m_Quality));
	roLog.AddKeyValue("Progress", ToString(m_Progress));
	roLog.AddKeyValue("MaxPreogress", ToString(m_MaxPreogress));
	roLog.AddKeyValue("ChgValue", ToString(m_ChgValue));
	roLog.AddKeyValue("TotalValue", ToString(m_TotalValue));
	roLog.AddKeyValue("Op", ToString(m_Op));
	roLog.AddKeyValue("CompleteNum", ToString(m_CompleteNum));
	roLog.AddKeyValue("PreTaskId", ToString(m_PreTaskId));
	roLog.AddKeyValue("AskHelperNum", ToString(m_AskHelperNum));
}

void TSecTalkFlow::Build(LogHelper& roLog)
{
	if (m_uReceiverRoleID)
	{
		//RoleSummary* pSum = RoleSummaryMgr::Instance()->FindRoleSummary(m_uReceiverRoleID);
		//if (pSum)
		//{
		//	m_strReceiverOpenID = pSum->GetAccount();
		//	m_iReceiverRoleType = pSum->GetProfession();
		//	m_uReceiverRoleID = pSum->GetID();
		//	m_iReceiverRoleLevel = pSum->GetLevel();
		//}
	}

	roLog.AddKeyValue("GameSvrId", ToString(m_poRole->GetServerId()));
	roLog.AddKeyValueTime("dtEventTime", time(NULL));
	roLog.AddKeyValue("GameAppID", GSConfig::Instance()->GetAppType()==KKSG::GAME_APP_QQ?MobileQQ_App_ID:WeChat_App_ID);
	roLog.AddKeyValue("OpenID", m_poRole->GetAccount());
	roLog.AddKeyValue("PlatID", ToString(m_poRole->GetClientConf().platid()));
	roLog.AddKeyValue("AreaID", ToString(GSConfig::Instance()->GetAppType()));//微信/手q/游客
	roLog.AddKeyValue("ZoneID", ToString(m_poRole->GetServerId()));
	roLog.AddKeyValue("RoleName", ProcessRoleName(m_poRole->GetName()));
	roLog.AddKeyValue("RoleId", ToString(m_poRole->GetID()));
	roLog.AddKeyValue("RoleType", ToString(m_poRole->GetProfession()));
	roLog.AddKeyValue("RoleLevel", ToString(m_poRole->GetLevel()));
	roLog.AddKeyValue("RoleBattlePoint", ToString((UINT32)m_poRole->GetAttr(TOTAL_POWERPOINT)));
	roLog.AddKeyValue("UserIP", m_poRole->GetClientConf().ip());
	roLog.AddKeyValue("GroupID", ToString(m_poRole->getGuildId()));
	roLog.AddKeyValue("PicUrl", m_poRole->GetRoleAllInfo().idiprecord().picurl());
	roLog.AddKeyValue("ReceiverOpenID", m_strReceiverOpenID);
	roLog.AddKeyValue("ReceiverRoleType", ToString(m_iReceiverRoleType));
	roLog.AddKeyValue("ReceiverRoleLevel", ToString(m_iReceiverRoleLevel));
	roLog.AddKeyValue("ChatType", ToString(m_iChatType));
	roLog.AddKeyValue("TitleContents", ProcessRoleName(m_strTitleContents));
	roLog.AddKeyValue("ChatContents", ProcessRoleName(m_strChatContents));
	roLog.AddKeyValue("MsgType", ToString(m_iMsgType));
	roLog.AddKeyValue("SceneType", ToString(m_iSceneType));
}

void TItemAttr::SetItem(XItem* pItem)
{
	if (NULL == pItem)
	{
		return;
	}
	//附魔属性;
	if (NULL != pItem->enchant)
	{
		m_EnchantAttrID = pItem->enchant->chooseAttrid;
		for (auto it = pItem->enchant->mapAllAttrs.begin(); it != pItem->enchant->mapAllAttrs.end(); it ++)
		{
			if (it->first == pItem->enchant->chooseAttrid)
			{
				m_EnchantAttrValue = it->second.attrValue;
			}
			else
			{
				TBaseAttr attr;
				attr.m_AttrID = it->first;
				attr.m_AttrValue = it->second.attrValue;
				m_vEnchantNotList.push_back(attr);
			}
		}
	}

	//随机属性;
	if (NULL != pItem->randAttr)
	{
		UINT32 count = pItem->randAttr->attrs.size();
		for (UINT32 i = 0; i < count && i < TXLOG_ITEMRANDOM_ATTRNUM; i ++)
		{
			TBaseAttr attr;
			attr.m_AttrID = pItem->randAttr->attrs[i].AttrID;
			attr.m_AttrValue = pItem->randAttr->attrs[i].AttrValue;
			const RandAttrSingle* pSingle = RandomAttrMgr::Instance()->GetRandomAttrSingle(pItem->itemID, i, attr.m_AttrID);
			if (NULL != pSingle)
			{
				attr.m_AttrFirst = pSingle->attrRange[0];
				attr.m_AttrSecond = pSingle->attrRange[1];
			}
			m_vRandomList.push_back(attr);
		}
		m_RandomAttrNum = count;
	}
	for (UINT32 i = m_vRandomList.size(); i < TXLOG_ITEMRANDOM_ATTRNUM; i ++)
	{
		TBaseAttr attr;
		m_vRandomList.push_back(attr);
	}

	//锻造属性
	if (NULL != pItem->forge)
	{
		UINT32 count = pItem->forge->attrs.size();
		for (UINT32 i = 0; i < count && i < TXLOG_ITEMFORGE_ATTRNUM; i ++)
		{
			TBaseAttr forgeattr;
			forgeattr.m_AttrID = pItem->forge->attrs[i].AttrID;
			forgeattr.m_AttrValue = pItem->forge->attrs[i].AttrValue;
			const RandAttrSingle* pSingle = RandomAttrMgr::Instance()->GetForgeAttrSingle(pItem->itemID, i, forgeattr.m_AttrID);
			if (NULL != pSingle)
			{
				forgeattr.m_AttrFirst = pSingle->attrRange[0];
				forgeattr.m_AttrSecond = pSingle->attrRange[1];
			}
			m_vForgeList.push_back(forgeattr);
		}
		m_ForgeAttrNum = count;
	}
	for (UINT32 i = m_vForgeList.size(); i < TXLOG_ITEMFORGE_ATTRNUM; i ++)
	{
		TBaseAttr forgeattr;
		m_vForgeList.push_back(forgeattr);
	}
}
void TItemAttr::SetLogEnchanNotAttr(LogHelper& roLog)
{
	roLog.AddKeyValue("EnchantNotAttrNum", ToString(m_vEnchantNotList.size()));
	for (INT32 i = 0; i < TXLOG_ENCHANTNOT_ATTRNUM; i ++)
	{
		TBaseAttr attr;
		if (i < m_vEnchantNotList.size())
		{
			attr = m_vEnchantNotList[i];
		}
		SetLogAttr(roLog, "EnchantNotAttrID", i, ToString(attr.m_AttrID));
		SetLogAttr(roLog, "EnchantNotAttrValue", i, ToString(attr.m_AttrValue));
	}
}
void TItemAttr::SetBaseLogHelper(LogHelper& roLog)
{
	roLog.AddKeyValue("EnchantAttrID", ToString(m_EnchantAttrID));
	roLog.AddKeyValue("EnchantAttrValue", ToString(m_EnchantAttrValue));
	roLog.AddKeyValue("ForgeAttrNum", ToString(m_ForgeAttrNum));
	for (UINT32 i = 0; i < m_vForgeList.size(); i ++)
	{
		SetLogAttr(roLog, "ForgeAttrID", i, ToString(m_vForgeList[i].m_AttrID));
		SetLogAttr(roLog, "ForgeAttrValue", i, ToString(m_vForgeList[i].m_AttrValue));
		SetLogAttr(roLog, "ForgeAttrFirst", i, ToString(m_vForgeList[i].m_AttrFirst));
		SetLogAttr(roLog, "ForgeAttrSecond", i, ToString(m_vForgeList[i].m_AttrSecond));
	}

	roLog.AddKeyValue("RandomAttrNum", ToString(m_RandomAttrNum));
	for (UINT32 i = 0; i < m_vRandomList.size(); i ++)
	{
		SetLogAttr(roLog, "RandomAttrID", i, ToString(m_vRandomList[i].m_AttrID));
		SetLogAttr(roLog, "RandomAttrValue", i, ToString(m_vRandomList[i].m_AttrValue));
		SetLogAttr(roLog, "RandomAttrFirst", i, ToString(m_vRandomList[i].m_AttrFirst));
		SetLogAttr(roLog, "RandomAttrSecond", i, ToString(m_vRandomList[i].m_AttrSecond));
	}
}
void TItemAttr::SetLogHelper(LogHelper& roLog)
{
	SetBaseLogHelper(roLog);
	SetLogEnchanNotAttr(roLog);
}

void TMobaBattleFlow::Build(LogHelperBase& roLog)
{
	if (m_RoleId)
	{
		m_ServerId = GetServerIDByRoleID(m_RoleId);
	}
	roLog.AddKeyValue("TransTag", m_strTransTag);
	roLog.AddKeyValue("GameSvrId", ToString(m_ServerId));
	roLog.AddKeyValue("OpenId", m_strOpenId);
	roLog.AddKeyValue("RoleId", ToString(m_RoleId));
	roLog.AddKeyValue("Level", ToString(m_Level));
	roLog.AddKeyValue("Profession", ToString(m_Profession));
	roLog.AddKeyValue("HeroID", ToString(m_HeroID));
	roLog.AddKeyValue("TeamID", ToString(m_TeamID));
	roLog.AddKeyValue("KillCount", ToString(m_KillCount));
	roLog.AddKeyValue("DeathCount", ToString(m_DeathCount));	
	roLog.AddKeyValue("AssistCount", ToString(m_AssistCount));
	roLog.AddKeyValue("m_KillContinueCountMax", ToString(m_KillContinueCountMax));
	roLog.AddKeyValue("m_MultiKillCountMax", ToString(m_MultiKillCountMax));
	roLog.AddKeyValue("Damage", ToString(m_Damage));
	roLog.AddKeyValue("BeHitDamage", ToString(m_BeHitDamage));
	roLog.AddKeyValue("IsRunAway", m_IsRunAway?1:0);
	roLog.AddKeyValue("MobaLevel", ToString(m_MobaLevel));
	roLog.AddKeyValue("Result", ToString(m_Result));
	roLog.AddKeyValue("MVP", ToString(m_MVP));
	roLog.AddKeyValue("RoundTime", ToString(m_RoundTime));

	Role* pRole = RoleManager::Instance()->FindByRoleID(m_RoleId);
	if (pRole)
	{
		ReportDataMgr::Instance()->MobaBattleResult(pRole, m_Result, m_KillCount, m_DeathCount);
	}
}

void TCustomPkResult::BuildAndSend()
{
	LogHelper log;
	log.SetDBType(KKSG::LOG_DB_TENCENT);
	log.SetTableName(GetTableName());

	log.AddKeyValue("TransTag", TTag::Instance()->GetTagString());
	log.AddKeyValue("GameSvrId", ToString(m_ServerId));
	log.AddKeyValueTime("dtEventTime", time(NULL));
	log.AddKeyValue("vGameAppid", m_strAppId);
	log.AddKeyValue("PlatID", ToString(m_PlatID));
	log.AddKeyValue("iZoneAreaID", ToString(m_ServerId));
	log.AddKeyValue("vopenid", m_strOpenId);
	log.AddKeyValue("RoleId", ToString(m_uRoleId));

	log.AddKeyValue("Level", ToString(m_Level));
	log.AddKeyValue("Profession", ToString(m_ProfessionID));
	log.AddKeyValue("Damge", ToString(m_Damge));
	log.AddKeyValue("BeHitDamage", ToString(m_BeHitDamage));

	log.AddKeyValue("MatchId", ToString(m_MatchId));
	log.AddKeyValue("MatchType", ToString(m_Matchtype));
	log.AddKeyValue("MatchMode", ToString(m_MatchMode));
	//log.AddKeyValue("ConfigId", ToString(m_ConfigId));
	log.AddKeyValue("IsWin", ToString(m_isWin));
	log.AddKeyValue("IsSystem", ToString(m_isSystem));
	log.AddKeyValue("Score", ToString(m_Score));
	log.AddKeyValue("TotalScore", ToString(m_TotalScore));

	log.Send();
}

void TCustomPkResult::SetRoleData(Role* role)
{
	if (NULL == role)
	{
		return;
	}
	m_Damge = (UINT64)role->Statistics().GetTotalDamage();
	m_BeHitDamage = (UINT64)role->Statistics().GetBeHitDamage();
	m_ServerId = role->GetServerId();
	m_PlatID = role->GetClientConf().platid();
	m_ProfessionID = role->GetProfession();
	m_Level = role->GetLevel();
	m_strAppId = GetGameAppID(role);
	m_strOpenId = role->GetAccount();
}

void TCustomPkResult::SetMatchData(UINT64 uid, INT32 type, INT32 isfair, INT32 issystem)
{
	m_MatchId = uid;//比赛ID///
	m_Matchtype = type;//比赛类型1v1,2v2///
	m_MatchMode = isfair ;//公平模式; 0:公平模式 1：非公平模式///
	m_isSystem = issystem;//是否系统比赛;//0:否 1：是
}

void TCustomPkResult::SetMatchResult(UINT64 roleid, INT32 iswin, INT32 score, INT32 allsocre)
{
	m_uRoleId = roleid;
	m_isWin = iswin;
	m_Score = score;//本场积分;
	m_TotalScore = allsocre;//总积分;
}
