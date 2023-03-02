#include "pch.h"
#include "ranklistmgr.h"
#include "ranklist.h"
#include "arenaranklist.h"
#include "worldbossranklist.h"
#include "briefranklist.h"
#include "flowerranklist.h"
#include "guildbossrank.h"
#include "pkranklist.h"
#include "towerranklist.h"
#include "tshowvoteranklist.h"
#include "network/dblink.h"
#include "role/role.h"
#include "commondef.h"
#include "config.h"
#include "firstpassranklist.h"
#include "firstpass/firstpassconfig.h"
#include "table/FirstPassTable.h"
#include "deprogressrank.h"
#include "db/mysqlmgr.h"
#include "imysql.h"
#include "spriteranklist.h"
#include "petranklist.h"
#include "guild/guildmgr.h"
#include "guild/guildboss.h"
#include "worldbossguildranklist.h"
#include "nestweekranklist.h"
#include "leagueranklist.h"
#include "crossleagueranklist.h"
#include "herobattleranklist.h"
#include "util/gettimeofday.h"
#include "ranklist/military_ranklist.h"
#include "ranklist/skycraftranklist.h"
#include "ranklist/biohelllrankList.h"
#include "global/GlobalConfigDBBuild.h"
#include "util/gametime.h"
#include "util/timespecificmgr.h"
#include "util/XCommon.h"
#include "event/eventmgr.h"
#include "ranklist/competedragonranklist.h"
#include "competedragon/competedragonconfig.h"
#include "surviveranklist.h"
#include "skycityranklist.h"

INSTANCE_SINGLETON(CRankListMgr);

CRankListMgr::CRankListMgr()
{
	m_timeHandler = INVALID_HTIMER;
	m_rank2punish[KKSG::PkRealTimeRank] = KKSG::PUNISH_USER_PK_RANK;
	m_rank2punish[KKSG::RealTimeArenaRank] = KKSG::PUNISH_USER_ARENA_RANK;
	m_rank2punish[KKSG::TowerRank] = KKSG::PUNISH_USER_TOWER;
	m_rank2punish[KKSG::FlowerRank] = KKSG::PUNISH_USER_FLOWER_RANK;
	m_rank2punish[KKSG::FlowerYesterdayRank] = KKSG::PUNISH_USER_FLOWER_RANK;
	m_rank2punish[KKSG::FlowerTotalRank] = KKSG::PUNISH_USER_FLOWER_RANK;
	m_rank2punish[KKSG::FlowerThisWeekRank] = KKSG::PUNISH_USER_FLOWER_RANK;
	m_rank2punish[KKSG::FlowerActivityRank] = KKSG::PUNISH_USER_FLOWER_RANK;
	m_rank2punish[KKSG::GuildBossRank] = KKSG::PUNISH_USER_GUILDBOSS_RANK;
	m_rank2punish[KKSG::WorldBossDamageRank] = KKSG::PUNISH_USER_WORLDBOSS_RANK;
	m_lastUpdateTime = 0;
	m_debugFlowerRankNum = 0;
}

CRankListMgr::~CRankListMgr()
{
	for (auto iter = m_rankLists.begin(); iter != m_rankLists.end(); iter++)
	{
		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
		if (iter2->second != NULL)
		{
			delete iter2->second;
			iter2->second = NULL;
		}
	}

	m_rankLists.clear();

	CTimerMgr::Instance()->KillTimer(m_timeHandler);
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_weektimeHandle);
}

bool CRankListMgr::Init()
{
	m_timeHandler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	if (INVALID_HTIMER == m_timeHandler)
	{
		SSWarn<<"m_TimeHandler is NULL"<<END;
		return false;
	}
	CRankList* pRankList = NULL;
	for (int i = 0; i <= KKSG::RankeType_MAX; ++i)
	{
		pRankList = NULL;
		switch(KKSG::RankeType(i))
		{
		case KKSG::RealTimeArenaRank:
			{
				pRankList = new CArenaRealTimeRankList(i);
				break;
			}
		case KKSG::ArenaRank:
			{
				pRankList = new CArenaRankList(i);
				break;
			}
		case KKSG::WorldBossGuildRank:
			{
				pRankList = new WorldBossGuildRankList(i);
				break;
			}
		case KKSG::WorldBossDamageRank:
			{
				pRankList = new BossDamageRankList(i);
				break;
			}
		case KKSG::PowerPointRank:
			{
				pRankList  = new CBriefRankList(i, new PowerPointCompare);
				break;
			}
		case KKSG::LevelRank:
			{
				pRankList  = new CBriefRankList(i, new LevelCompare);
				break;
			}
		case KKSG::FashionPowerPointRank:
			{
				pRankList  = new CBriefRankList(i, new PowerPointCompare);
				break;
			}
		case KKSG::FlowerRank:
		case KKSG::FlowerThisWeekRank:
		case KKSG::FlowerYesterdayRank:
		case KKSG::FlowerTotalRank:
		case KKSG::FlowerActivityRank:
			{
				pRankList  = new FlowerRankList(i);
				break;
			}
		case KKSG::GuildBossRank:
			{
				pRankList = new GuildBossRankList(i);
				break;
			}
		case KKSG::PkRealTimeRank:
			{
				pRankList = new PkRealTimeRankList(i);
				break;
			}
		case KKSG::LastWeek_PkRank:
			{
				pRankList = new LastWeekPkRankList(i);
				break;
			}
		case KKSG::PkRank2v2:
			{
				pRankList = new PkRealTimeRankList(i);
				break;
			}
		//case KKSG::PkRank:
		//	{
		//		pRankList = new PkRankList(i, CreateRankInfo<PkRankInfo>);
		//		break;
		//	}	
		//case KKSG::TShowVoteCountRank:
		//	{
		//		pRankList  = new TShowRankList(i);
		//		break;
		//	}
		case KKSG::TowerRank:
			{
				pRankList  = new TowerRankList(i);
				break;
			}
		case KKSG::FirstPassRank:
			{
				break;	
			}
		case KKSG::DEProgressRank:
			{
				pRankList = new DEPRankList(i);
				break;
			}
		case KKSG::SpritePowerPointRank:
			{
				pRankList = new SpriteRankList(i);
				break;
			}
		case KKSG::PetPowerPointRank:
			{
				pRankList = new PetRankList(i);
				break;
			}
		case KKSG::NestWeekRank:
			{
				pRankList = new NestWeekRankList(i);
				break;
			}
		case KKSG::LastWeek_NestWeekRank:
			{
				pRankList = new LastWeekNestWeekRankList(i);
				break;
			}
		case KKSG::LeagueTeamRank:
			{
				pRankList = new LeagueRankList(i);
				break;
			}
		case KKSG::LastWeek_LeagueTeamRank:
			{
				pRankList = new LastWeekLeagueRankList(i);
				break;
			}
		case KKSG::CrossLeagueRank:
			{
				pRankList = new CrossLeagueRankList(i);
				break;
			}
		case KKSG::HeroBattleRank:
			{
				pRankList = new HeroBattleRankList(i);
				break;
			}
		case KKSG::LastWeek_HeroBattleRank:
			{
				pRankList = new LastWeekHeroBattleRankList(i);
				break;
			}
		case KKSG::MilitaryRank:
			{
				pRankList = new MilitaryRankList(i);
				break;
			}
		case KKSG::SkyCraftRank:
			{
				pRankList = new SkyCraftRankList(i);
				break;
			}
		case KKSG::BioHelllRank:
			{
				pRankList = new BioHelllRankList(i);
				break;
			}
		case KKSG::CompeteDragonRank:
			{
				pRankList = new CompeteDragonRankList(i);
				break;
			}
		case KKSG::SurviveRank:
			{
				pRankList = new SurviveRankList(i);
				break;;
			}
		case KKSG::SkyCityRank:
			{
				pRankList = new SkyCityRankList(i);
				break;
			}
		default:
			{
				break;
			}
		}
		if (pRankList == NULL)
		{
			continue;
		}
		m_rankLists[pRankList->GetType()][pRankList->GetServerID()] = pRankList;
	}

	const std::vector<FirstPassTable::RowData*>& firstPassData = CFirstPassConfig::Instance()->GetFirstPassData();
	const std::set<UINT32>& allServerIDs = MSConfig::Instance()->GetServerIds();
	for (auto i = firstPassData.begin(); i != firstPassData.end(); i++)
	{
		for (auto j = allServerIDs.begin(); j != allServerIDs.end(); j++)
		{
			UINT32 type = GetFirstPassRankKey((*i)->ID);
			m_rankLists[type][*j] = new FirstPassRankList(type, *j);
		}
	}

	if (!QueryDB())
	{
		return false;
	}

	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	m_weektimeHandle = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&CRankListMgr::OnWeekUpdate, this));
	m_lastUpdateTime = GameTime::GetTime();
	return true;
}



void CRankListMgr::Uninit()
{
	if (INVALID_HTIMER != m_timeHandler)
	{	
		CTimerMgr::Instance()->KillTimer(m_timeHandler);
	}
	if (INVALID_TIMESPECIFIC!=m_weektimeHandle)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(m_weektimeHandle);
	}
}

bool CRankListMgr::QueryDB()
{
	for (auto i = m_rankLists.begin(); i != m_rankLists.end(); i++)
	{
		for (auto j = i->second.begin(); j != i->second.end(); j++)
		{
			if (!_QueryDBToRankList(j->second))
			{
				return false;
			}
		}
	}
	return true;
}

bool CRankListMgr::_QueryDBToRankList(CRankList* ranklist)
{
	if (ranklist->GetDbHandler() == NULL)
	{
		return true;
	}

	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
	std::stringstream ss;
	ss << "select data from ranklist where _id=" << ranklist->GetType() << " and serverid=" << ranklist->GetServerID();
	IMysqlRecordSet* poRes = NULL;
	if (!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table ranklist failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}
	if (poRes->GetRowNum() == 0)
	{
		LogInfo("ranklist _id = %u, serverid = %u is empty", ranklist->GetType(), ranklist->GetServerID());
		ranklist->GetDbHandler()->SetEmptyInDB();
		poRes->Release();
		return true;
	}
	if (poRes->GetRowNum() != 1)
	{
		LogError("data in table ranklist have more than one row where _id = %u, serverid = %u", ranklist->GetType(), ranklist->GetServerID());
		return false;
	}
	poRes->FetchRow();
	char* pVal = NULL;
	UINT32 dwLen = 0;
	if (poRes->GetFieldValue(0, &pVal, dwLen))
	{
		KKSG::RankList list;
		if (!list.ParseFromArray(pVal, dwLen))
		{
			LogError("ranklist which _id = %u, serverid = %u deserialize failed", ranklist->GetType(), ranklist->GetServerID());
			return false;
		}
		else
		{
			ranklist->GetDbHandler()->ReceiveFromDB(ranklist->GetType(), list);
		}
	}
	else
	{
		LogError("Read 'data' column failed, _id = %u, serverid = %u", ranklist->GetType(), ranklist->GetServerID());
		return false;
	}
	poRes->Release();

	return true;
}

void CRankListMgr::SaveAllToDB()
{
	LogInfo("start to save ranklist to db...");

	for (auto iter = m_rankLists.begin(); iter != m_rankLists.end(); iter++)
	{
		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
		{
			CRankList* ranklist = iter2->second;
			if (ranklist->GetDbHandler() != NULL)
			{
				if (ranklist->GetDbHandler()->BeforeSaveToDB())
				{
					ranklist->GetDbHandler()->SendToDB();
				}
			}
		}
	}

	LogInfo("end save rank list");
}

CRankList* CRankListMgr::GetRankList(UINT32 type, UINT32 serverid)
{
	auto iter = m_rankLists.find(type);
	if (iter == m_rankLists.end())
	{
		return NULL;
	}
	if (iter->second.size() == 1)
	{
		return iter->second.begin()->second;
	}
	auto iter2 = iter->second.find(serverid);
	if (iter2 == iter->second.end())
	{
		return NULL;
	}
	return iter2->second;
}

void CRankListMgr::OnTimer(UINT32 id, UINT32 count)
{
	for (auto iter = m_rankLists.begin(); iter != m_rankLists.end(); iter++)
	{
		for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++)
		{
			CRankList* ranklist = iter2->second;
			if (ranklist->GetDbHandler() != NULL)
			{
				if (ranklist->GetDbHandler()->CheckTimeToSaveDB())
				{
					return; //每次最多只存一个排行榜
				}
			}
		}
	}
}

void CRankListMgr::OnChangeBrief(UINT64 roleid, UINT32 ppt, UINT32 ppttime, UINT32 level, UINT32 leveltime)
{
	CBriefRankList* levellist = static_cast<CBriefRankList*>(GetRankList(KKSG::LevelRank));
	levellist->OnChange(roleid, ppt, level, leveltime);

	CBriefRankList* pointlist = static_cast<CBriefRankList*>(GetRankList(KKSG::PowerPointRank));
	pointlist->OnChange(roleid, ppt, level, ppttime);
}

void CRankListMgr::OnChangeFashionPower(UINT64 roleid, UINT32 ppt, UINT32 level, UINT32 time)
{
	CBriefRankList* fashionPowerPointList = static_cast<CBriefRankList*>(GetRankList(KKSG::FashionPowerPointRank));
	fashionPowerPointList->OnChange(roleid, ppt, level, time);
}

void CRankListMgr::OnChangeFlowerCount(UINT64 roleid, UINT32 count, std::map<UINT32, UINT32>& flowers, UINT32 time)
{
	FlowerRankList* ranklist = static_cast<FlowerRankList*>(GetRankList(KKSG::FlowerRank));
	ranklist->OnChange(roleid, count, flowers, time);
}

void CRankListMgr::OnChangeFlowerThisWeekCount(UINT64 roleid, UINT32 count, std::map<UINT32, UINT32>& flowers, UINT32 time)
{
	FlowerRankList* ranklist = static_cast<FlowerRankList*>(GetRankList(KKSG::FlowerThisWeekRank));
	ranklist->OnChange(roleid, count, flowers, time);
}

void CRankListMgr::OnChangeFlowerYestodayCount(UINT64 roleid, UINT32 yestodaycount, std::map<UINT32, UINT32>& flowers, UINT32 time)
{
	FlowerRankList* ranklistyestoday = static_cast<FlowerRankList*>(GetRankList(KKSG::FlowerYesterdayRank));
	ranklistyestoday->OnChange(roleid, yestodaycount, flowers, time);
}

void CRankListMgr::OnChangeFlowerActivityCount(UINT64 roleid, UINT32 count, std::map<UINT32, UINT32>& flowers, UINT32 time)
{
	FlowerRankList* ranklistActivity = static_cast<FlowerRankList*>(GetRankList(KKSG::FlowerActivityRank));
	ranklistActivity->OnChange(roleid, count, flowers, time);
}

void CRankListMgr::OnChangeFlowerTotalCount(UINT64 roleid, UINT32 total, std::map<UINT32, UINT32>& flowers, UINT32 time)
{
	FlowerRankList* ranklisttotal = static_cast<FlowerRankList*>(GetRankList(KKSG::FlowerTotalRank));
	ranklisttotal->OnChange(roleid, total, flowers, time);
}

void CRankListMgr::OnChangeSpritePowerPoint(UINT64 roleid, UINT32 powerpoint, UINT32 time)
{
	SpriteRankList* ranklist = static_cast<SpriteRankList*>(GetRankList(KKSG::SpritePowerPointRank));
	ranklist->OnChange(roleid, powerpoint, time);
}

void CRankListMgr::OnChangePetPowerPoint(UINT64 petuid, UINT32 petid, UINT64 roleid, UINT32 powerpoint, UINT32 time)
{
	PetRankList* ranklist = static_cast<PetRankList*>(GetRankList(KKSG::PetPowerPointRank));
	ranklist->OnChange(petuid, petid, roleid, powerpoint, time);
}

void CRankListMgr::SendTShowTop3Rank()
{
	FlowerRankList* ranklist = static_cast<FlowerRankList*>(GetRankList(KKSG::FlowerTotalRank));
	ranklist->SendTShowTop3Rank();
}

void CRankListMgr::SendTShowSelfRank(CRoleSummary* role)
{
	FlowerRankList* ranklist = static_cast<FlowerRankList*>(GetRankList(KKSG::FlowerTotalRank));
	ranklist->SendTShowTop3Rank(role);

	FlowerRankList* ranklistYes = static_cast<FlowerRankList*>(GetRankList(KKSG::FlowerYesterdayRank));
	ranklistYes->SendTShowWalkRank(role);
}

void CRankListMgr::OnChangeTowerRank(std::vector<UINT64>& roleids, UINT32 towerHardLevel, UINT32 towerFloor, UINT32 useTime, UINT32 time)
{
	TowerRankList* ranklist = static_cast<TowerRankList*>(GetRankList(KKSG::TowerRank));
	ranklist->OnChange(roleids, towerHardLevel, towerFloor, useTime, time);
}

std::map<int,int> CRankListMgr::OnChangeFirstPassRank(std::vector<UINT64>& roleids, int firstPassID, UINT32 starLevel)
{
	std::map<int,int> serverRank;
	UINT32 type = GetFirstPassRankKey(firstPassID);
	auto iter = m_rankLists.find(type);
	if (iter == m_rankLists.end())
	{
		return serverRank;
	}
	timeval now;
	gettimeofday(&now, NULL);
	//int rank = INVALID_RANK;
	std::map<UINT32, CRankList*>& rankMap = iter->second;
	for (auto i = rankMap.begin(); i != rankMap.end(); i++)
	{
		FirstPassRankList* ranklist = static_cast<FirstPassRankList*>(i->second);
		int tempRank = ranklist->OnChange(roleids, starLevel, now.tv_sec, now.tv_usec);
		serverRank[i->first] = tempRank;
	}
	return serverRank;
}

UINT32 CRankListMgr::GetFirstPassRankKey(int firstPassID)
{
	return KKSG::FirstPassRank * 1000 + firstPassID;
}

bool CRankListMgr::IsFirstPassRankKey(UINT32 type)
{
	return type / 1000 == KKSG::FirstPassRank;
}

void CRankListMgr::DeleteTheOneInRankList(UINT64 roleid, KKSG::PunishType type)
{
	switch(type)
	{
	case KKSG::PUNISH_USER_PK_RANK:
		{
			PkRealTimeRankList* ranklist = static_cast<PkRealTimeRankList*>(GetRankList(KKSG::PkRealTimeRank));
			ranklist->DeleteRole(roleid);
			break;
		}
	case KKSG::PUNISH_USER_ARENA_RANK:
		{
			CArenaRealTimeRankList* ranklist = static_cast<CArenaRealTimeRankList*>(GetRankList(KKSG::RealTimeArenaRank));
			ranklist->DeleteRole(roleid);
			break;
		}
	case KKSG::PUNISH_USER_TOWER:
		{
			TowerRankList* ranklist = static_cast<TowerRankList*>(GetRankList(KKSG::TowerRank));
			ranklist->DeleteRole(roleid);
			break;
		}
	case KKSG::PUNISH_USER_FLOWER_RANK:
		{
			FlowerRankList* ranklist = static_cast<FlowerRankList*>(GetRankList(KKSG::FlowerRank));
			ranklist->DeleteRole(roleid);
			ranklist = static_cast<FlowerRankList*>(GetRankList(KKSG::FlowerThisWeekRank));
			ranklist->DeleteRole(roleid);
			ranklist = static_cast<FlowerRankList*>(GetRankList(KKSG::FlowerYesterdayRank));
			ranklist->DeleteRole(roleid);
			ranklist = static_cast<FlowerRankList*>(GetRankList(KKSG::FlowerTotalRank));
			ranklist->DeleteRole(roleid);
			ranklist = static_cast<FlowerRankList*>(GetRankList(KKSG::FlowerActivityRank));
			ranklist->DeleteRole(roleid);
			break;
		}
	case KKSG::PUNISH_USER_GUILDBOSS_RANK:
		{
			GuildBossRankList* ranklist = static_cast<GuildBossRankList*>(GetRankList(KKSG::GuildBossRank));
			ranklist->DeleteGuild(roleid);
			break;
		}
	case KKSG::PUNISH_USER_WORLDBOSS_RANK:
		{
			BossDamageRankList* ranklist = static_cast<BossDamageRankList*>(GetRankList(KKSG::WorldBossDamageRank));
			ranklist->DeleteRole(roleid);
			break;
		}
	case KKSG::PUNISH_USER_GUILD_RANK:
		{
			Guild *pGuild = CGuildMgr::Instance()->GetGuild(roleid);
			if (pGuild)
			{
				CGuildMgr::Instance()->GetSorter().RemoveGuild(pGuild);
			}
			break;
		}
	case KKSG::PUNISH_USER_ROLE_GUILDBOSS:
		{
			CGuildBossMgr::Instance()->RemoveDamageRank(roleid);
			break;
		}
	default:
		{
			break;
		}
	}
}

UINT32 CRankListMgr::GetPunishType(KKSG::RankeType type)
{
	if(m_rank2punish.find(type) == m_rank2punish.end())
	{
		return (UINT32)(-1);
	}
	return (UINT32)m_rank2punish[type];
}

void CRankListMgr::GMClearFirstPass()
{
	for (auto i = m_rankLists.begin(); i != m_rankLists.end(); i++)
	{
		if (!IsFirstPassRankKey(i->first))
		{
			continue;
		}
		for (auto j = i->second.begin(); j != i->second.end(); j++)
		{
			j->second->RemoveAll();
		}
	}
}

LeagueRankList* CRankListMgr::GetLeagueRankList()
{
	return static_cast<LeagueRankList*>(GetRankList(KKSG::LeagueTeamRank));
}

CrossLeagueRankList* CRankListMgr::GetCrossLeagueRankList()
{
	return static_cast<CrossLeagueRankList*>(GetRankList(KKSG::CrossLeagueRank));
}

SkyCraftRankList* CRankListMgr::GetSkyCraftRankList()
{
	return static_cast<SkyCraftRankList*>(GetRankList(KKSG::SkyCraftRank));
}

SkyCityRankList* CRankListMgr::GetSkyCityRankList()
{
	return static_cast<SkyCityRankList*>(GetRankList(KKSG::SkyCityRank));
}

void CRankListMgr::DebugSetFlowerRankNum(UINT32 num)
{
	m_debugFlowerRankNum = num;
}


void CRankListMgr::OnSartUp()
{
	std::string szUpdateTime =  CGlobalConfigDBBuild::Instance()->GetConfigValue(eRankListWeekUpdateTime);
	UINT32 nRecTime    =  0;
	if(!szUpdateTime.empty())
	{
		nRecTime = convert<UINT32>(szUpdateTime);
	}
	int now = GameTime::GetTime();
	if (nRecTime&&!XCommon::IsWeekSame(nRecTime, now))
	{
		OnWeekEvent(now);
	}
	if (!nRecTime)
	{
		CGlobalConfigDBBuild::Instance()->SetConfigValue(eRankListWeekUpdateTime,ToString(XCommon::GetDayBeginTime(now)));
		CGlobalConfigDBBuild::Instance()->UpdateToDb(eRankListWeekUpdateTime);
	}

	szUpdateTime =  CGlobalConfigDBBuild::Instance()->GetConfigValue(eRankListTwoWeekUpdateTime);
	nRecTime = 0;
	if(!szUpdateTime.empty())
	{
		nRecTime = convert<UINT32>(szUpdateTime);
	}
	//需要修改
	if (nRecTime&& now - nRecTime >= 14 * 86400)
	{
		OnTwoWeekEvent(nRecTime + 14 * 86400);
	}
	if (!nRecTime)
	{
		CGlobalConfigDBBuild::Instance()->SetConfigValue(eRankListTwoWeekUpdateTime,ToString(XCommon::GetWeekBeginTime(now)));
		CGlobalConfigDBBuild::Instance()->UpdateToDb(eRankListTwoWeekUpdateTime);
	}
}


bool CRankListMgr::OnWeekUpdate()
{
	int now = GameTime::GetTime();
	if (GameTime::GetWeekDay()==1)
	{
		OnWeekEvent(now);

		std::string szUpdateTime =  CGlobalConfigDBBuild::Instance()->GetConfigValue(eRankListTwoWeekUpdateTime);
		UINT32 nRecTime = 0;
		if(!szUpdateTime.empty())
		{
			nRecTime = convert<UINT32>(szUpdateTime);
		}
		if (nRecTime&& now - nRecTime >= 14 * 86400)
		{
			OnTwoWeekEvent(nRecTime + 14 * 86400);
		}
	}
	return true;
}


bool CRankListMgr::OnWeekEvent(UINT32 nTime)
{
	TowerRankList* ranklist = static_cast<TowerRankList*>(GetRankList(KKSG::TowerRank));
	std::set<TowerRankInfo*, TowerCompare>& currankinfos = (static_cast<TowerRankList*>(ranklist))->GetCurRankInfo();
	UINT32 nRank = 0;
	for (auto i = currankinfos.begin(); i != currankinfos.end(); ++i)
	{
		TowerRankInfo* rankinfo = (*i);
		if (NULL == rankinfo)
			continue;
		if (rankinfo->GetRoleIDs().empty())
			continue;
		if (nRank>10)
			break;
		EventMgr::Instance()->AddEvent(rankinfo->GetRoleIDs().at(0), DESIGNATION_COM_TYPE_TOWERRANK,++nRank);
	}
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eRankListWeekUpdateTime,ToString(nTime));
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eRankListWeekUpdateTime);
	m_lastUpdateTime = nTime;
	return true;
}

bool CRankListMgr::OnTwoWeekEvent(UINT32 nTime)
{
	CompeteDragonRankList* ranklist = static_cast<CompeteDragonRankList*>(GetRankList(KKSG::CompeteDragonRank));
	if (!ranklist)
	{
		return false;
	}

	ranklist->OnTwoWeekEvent();

	CGlobalConfigDBBuild::Instance()->SetConfigValue(eRankListTwoWeekUpdateTime,ToString(XCommon::GetDayBeginTime(nTime)));
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eRankListTwoWeekUpdateTime);
	m_lastUpdateTime = GameTime::GetTime();
	return true;
}
