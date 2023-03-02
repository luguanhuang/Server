#include "pch.h"
#include "leagueseasonmgr.h"
#include "global/GlobalConfigDBBuild.h"
#include "table/globalconfig.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "levelseal/levelsealMgr.h"
#include "util/timespecificmgr.h"
#include "leaguebattle/leagueeliminationmgr.h"
#include "reswar/ptcm2c_halliconmntf.h"
#include "define/systemiddef.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "leagueteam/leagueteammgr.h"
#include "network/worldlink.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/leagueranklist.h"
#include "ranklist/crossleagueranklist.h"
#include "leaguebattle/rpcm2k_leaguereporttop2world.h"
#include "leagueteam/crossleagueteammgr.h"
#include "role/crossrolesummarymgr.h"
#include "role/rolesummarymgr.h"
#include "arenastar/arenastarmgr.h"

LeagueSeasonMgr::LeagueSeasonMgr()
:m_dirty(false)
,m_isSeasonOpen(false)
,m_report2World(false)
,m_lastUpdateTime(0)
,m_seasonNum(0)
,m_lastSeasonOpenTime(0)
,m_reporting(false)
,m_isRaceMatchNoLimit(false)
{

}

LeagueSeasonMgr::~LeagueSeasonMgr()
{

}

bool LeagueSeasonMgr::Init()
{
	LoadSeasonInfoFromDB();
	BuildTimeState();
	CheckSeasonOpen();
	m_todayState = GetCurState(GameTime::GetTime());

	// 注册不同阶段的每日定时器
	SetUpTimers();

	// 注册定时器
	CTimerMgr::Instance()->SetTimer(this, 0, 3000, -1, __FILE__, __LINE__);	

	return true;
}

void LeagueSeasonMgr::Uninit()
{

}

void LeagueSeasonMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 now = GameTime::GetTime();

	bool update = false;
	if ((now - m_lastUpdateTime) > 60)
	{
		CheckSeasonOpen();
		update = true;
	}

	// 生成八强对阵图
	if (m_lastUpdateTime < m_eleMapBuildTime && now >= m_eleMapBuildTime)
	{
		LeagueEliminationMgr::Instance()->InitRound();
		LeagueTeamMgr::Instance()->SendAllRankReward();
		update = true;
	}

	// 先遣时本服结束清理数据
	if (m_clearSelfServer)
	{
		if (m_lastUpdateTime < m_clearSelfTime && now >= m_clearSelfTime)
		{
			OnSeasonEnd();
		}
	}
	// 正式服有跨服玩法
	else
	{
		// 上报本服积分赛前八名
		if (now >= m_eleMapBuildTime && m_report2World == false && m_reporting == false)
		{
			ReportTop2World();
			update = true;
		}
	}

	// 赛季结束
	if (m_lastUpdateTime < m_seasonEndTime && now >= m_seasonEndTime)
	{
		OnSeasonEnd();
		update = true;
	}

	// 下个赛季开始
	if (now > m_nextSeasonOpenTime)
	{
		BuildTimeState();
		CheckSeasonOpen();
		update = true;
	}

	KKSG::LeagueBattleTimeState oldState = m_todayState;
	KKSG::LeagueBattleTimeState state = GetCurState(now);
	if (state != m_todayState || XCommon::IsDaySame(now, m_lastUpdateTime, false) == false)
	{
		m_todayState = state;
		OnStateChange(oldState, m_todayState);
		NotifyTimeState();
		update = true;
		m_dirty = true;
	}

	if (update)
	{
		m_lastUpdateTime = now;
		m_dirty = true;
	}

	if (m_dirty)
	{
		SaveSeasonInfoToDB();
	}
}

KKSG::LeagueBattleTimeState LeagueSeasonMgr::GetCurState(UINT32 now)
{
	if (m_isSeasonOpen == false)
	{
		return KKSG::LBTS_SeasonEnd;
	}
	return GetCurStateByTime(now);
}

void LeagueSeasonMgr::LoadSeasonInfoFromDB()
{
	const std::string temp = CGlobalConfigDBBuild::Instance()->GetConfigValue(eLeagueBattleTime);
	KKSG::LeagueBattleSeasonData data;
	if (data.ParseFromString(temp) == false)
	{
		LogWarn("parse league battle season failed");
	}
	m_lastUpdateTime = data.last_update_time();
	m_isSeasonOpen = data.is_season_open();
	m_seasonNum = data.season_num();
	m_lastSeasonOpenTime = data.last_season_open_time();
	m_report2World = data.report_world();
	m_lastCheckOpenTime = data.last_check_open_time();

	LogInfo("load season info, updatetime[%u], is_open[%u]", m_lastUpdateTime, m_isSeasonOpen);
}

void LeagueSeasonMgr::SaveSeasonInfoToDB()
{
	LogInfo("SaveSeasonInfoToDB, time[%u], open[%u]", m_lastUpdateTime, m_isSeasonOpen);

	KKSG::LeagueBattleSeasonData data;
	data.set_last_update_time(m_lastUpdateTime);
	data.set_is_season_open(m_isSeasonOpen);
	data.set_season_num(m_seasonNum);
	data.set_last_season_open_time(m_lastSeasonOpenTime);
	data.set_report_world(m_report2World);
	data.set_last_check_open_time(m_lastCheckOpenTime);

	std::string temp;
	data.SerializeToString(&temp);
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eLeagueBattleTime, temp);
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eLeagueBattleTime);

	m_dirty = false;
}

void LeagueSeasonMgr::CheckSeasonOpen()
{
	UINT32 year, month, day;
	UINT32 now = GameTime::GetTime();
	XCommon::GetDate(year, month, day);

	do 
	{
		// 配置关闭
		if (GetGlobalConfig().LeagueBattleIsOpen == false)
		{
			SetSeasonOpen(false);
			break;
		}

		// 本赛季结束, 下赛季开始之前
		if (now > m_seasonEndTime && now < m_nextSeasonOpenTime)
		{
			SetSeasonOpen(false);
			break;
		}

		// 在最晚可以开启时间之前
		// 解封和联赛开启时序问题，这里多检测2分钟
		if (m_lastCheckOpenTime < (m_lastCanOpenTime + 120))
		{
			m_lastCheckOpenTime = now;
			UINT32 iSealType = GetGlobalConfig().LeagueBattleLevelSealType;
			if (CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType >= iSealType
				&& CRoleSummaryMgr::Instance()->GetMaxLevel() >= CLevelSealMgr::Instance()->GetLevelSealMaxLevelByType(iSealType))
			{
				SetSeasonOpen(true);
			}
			else
			{
				SetSeasonOpen(false);
			}
			LogInfo("check open at[%u], cur seal type[%u], cur max level[%u], is_open[%u]", now, 
					CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType,
					 CRoleSummaryMgr::Instance()->GetMaxLevel(), m_isSeasonOpen);
			break;
		}
	} while (0);
}

void LeagueSeasonMgr::OnUnlockLevelSeal(UINT32 type)
{
	CheckSeasonOpen();
}

bool LeagueSeasonMgr::IsInApplyTime()
{
	if (m_isRaceMatchNoLimit)
	{
		return true;
	}
	KKSG::LeagueBattleTimeState state = GetTodayState();
	if (state != KKSG::LBTS_Apply)
	{
		return false;
	}
	return true;
}

bool LeagueSeasonMgr::IsInPointRaceMatchTime()
{
	if (m_isRaceMatchNoLimit)
	{
		return true;
	}

	if (m_todayState != KKSG::LBTS_PointRace && m_todayState != KKSG::LBTS_CrossPointRace)
	{
		return false;
	}
	UINT32 todayPassSec = XCommon::GetTodayPassSec();
	UINT32 startSec = GetGlobalConfig().LeagueBattlePointRaceTime[0] / 100 * 3600 + (GetGlobalConfig().LeagueBattlePointRaceTime[0] % 100) * 60;
	UINT32 endSec = GetGlobalConfig().LeagueBattlePointRaceTime[1] / 100 * 3600 + (GetGlobalConfig().LeagueBattlePointRaceTime[1] % 100) * 60;

	if (todayPassSec > startSec && todayPassSec < endSec)
	{
		return true;
	}
	return false;
}

UINT32 LeagueSeasonMgr::GetMatchLeftTime()
{
	if (!IsInPointRaceMatchTime())
	{
		return 0;
	}
	UINT32 todayPassSec = XCommon::GetTodayPassSec();
	UINT32 endSec = GetGlobalConfig().LeagueBattlePointRaceTime[1] / 100 * 3600 + (GetGlobalConfig().LeagueBattlePointRaceTime[1] % 100) * 60;
	UINT32 leftTime =  (endSec > todayPassSec) ? (endSec - todayPassSec) : 0;
	return leftTime;
}

UINT32 LeagueSeasonMgr::GetRankRewardLeftTime()
{
	UINT32 now = GameTime::GetTime();	
	return now < m_eleMapBuildTime ? (m_eleMapBuildTime - now) : 0;
}

UINT32 LeagueSeasonMgr::GetCrossRankRewardLeftTime()
{
	UINT32 now = GameTime::GetTime();	
	return now < m_crossEleMapBuildTime ? (m_crossEleMapBuildTime - now) : 0;
}

void LeagueSeasonMgr::SetSeasonOpen(bool open)
{
	m_isSeasonOpen = open;

	if (m_isSeasonOpen)
	{
		UINT32 now = GameTime::GetTime();
		if (!XCommon::IsMonthSame(now, m_lastSeasonOpenTime, false))
		{
			m_lastSeasonOpenTime = now;
			m_seasonNum++;
			m_dirty = true;
		}
		OnSeasonOpen();
	}
}

void LeagueSeasonMgr::OnStateChange(KKSG::LeagueBattleTimeState oldState, KKSG::LeagueBattleTimeState curState)
{
	SetUpTimers();
}

void LeagueSeasonMgr::NotifyTimeState()
{
	for (auto it = CRoleManager::Instance()->Begin(); it != CRoleManager::Instance()->End(); ++it)
	{
		CRole* pRole = *it;
		LeagueTeamMgr::Instance()->UpdateSeasonInfo(pRole);
	}
}

void LeagueSeasonMgr::OnSeasonOpen()
{
	m_report2World = false;
	m_reporting = false;
	m_dirty = true;
}

void LeagueSeasonMgr::OnSeasonEnd()
{

	LeagueTeamMgr::Instance()->OnSeasonEnd();
	LeagueEliminationMgr::Instance()->OnSeasonEnd();
	CrossLeagueTeamMgr::Instance()->OnSeasonEnd();
	CrossRoleSummaryMgr::Instance()->OnSeasonEnd();

	// 清排行榜
	CRankListMgr::Instance()->GetLeagueRankList()->RemoveAll();
	CRankListMgr::Instance()->GetCrossLeagueRankList()->RemoveAll();
}

void LeagueSeasonMgr::SetUpTimers()
{
	ClearTimers();
	KKSG::LeagueBattleTimeState curState = m_todayState;
	UINT32 year, month, day;
	XCommon::GetDate(year, month, day);

	// notice icon等推送
	m_noticeHelper.SetTimers(this);

	// 只关注逻辑定时器
	if (curState == KKSG::LBTS_Elimination)
	{
		// 淘汰赛三局比赛开始
		if (GetGlobalConfig().LeagueBattleEliminationTime.Size() < 3)
		{
			LogError("error");
			return;
		}
		for (int i = 0; i < 4; ++i)
		{
			UINT32 hour = GetGlobalConfig().LeagueBattleEliminationTime[i] / 100;
			UINT32 min = GetGlobalConfig().LeagueBattleEliminationTime[i] % 100;
			TimeSpecific t(hour, min, 0);
			UINT32 timerId = CTimeSpecificMgr::Instance()->RegisterEvent(t, std::bind(&LeagueSeasonMgr::EleminationRoundTimer, this));
			m_timers.push_back(timerId);
		}
	}
}

void LeagueSeasonMgr::ClearTimers()
{
	for (auto it = m_timers.begin(); it != m_timers.end(); ++it)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(*it);
	}
	m_timers.clear();
}


bool LeagueSeasonMgr::EleminationRoundTimer()
{
	LeagueEliminationMgr::Instance()->OnRoundStart();
	return true;
}

void LeagueSeasonMgr::OnRoleLogin(CRole* pRole)
{
	if (pRole == NULL)
	{
		return;
	}

	if (LeagueTeamMgr::Instance()->HasLeagueTeam(pRole->GetID()) == false)
	{
		return;
	}

	if (IsInPointRaceMatchTime())
	{
		PtcM2C_HallIconMNtf ntf;
		ntf.m_Data.set_state(KKSG::HICONS_BEGIN);
		ntf.m_Data.set_systemid(SYS_TEAMLEAGUE);
		pRole->Send(ntf);
	}
}

void LeagueSeasonMgr::ReportTop2World()
{
	if (!WorldLink::Instance()->IsConnected())
	{
		return;
	}

	// 取排行榜前几名
	LeagueRankList* ranklist = CRankListMgr::Instance()->GetLeagueRankList();
	std::vector<LeagueRankInfo*> ranks;
	ranklist->GetTopNRankInfo(GetGlobalConfig().LeagueRankJoinCrosssPointRace, ranks);

	LogInfo("Report Top8 2 World");
	RpcM2K_LeagueReportTop2World* rpc = RpcM2K_LeagueReportTop2World::CreateRpc();
	for (size_t i = 0; i < ranks.size(); ++i)
	{
		LeagueRankInfo* info = ranks[i];
		if (info == NULL)
		{
			continue;
		}
		LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(info->GetUniqueId());
		if (pLTeam == NULL)
		{
			continue;
		}
		LogInfo("rank[%u], leagueteamId[%llu]", i + 1, info->GetUniqueId());
		pLTeam->FillCrossLeagueTeamInitData(*rpc->m_oArg.add_teams());
	}

	WorldLink::Instance()->SendTo(*rpc);
	m_reporting = true;
}

void LeagueSeasonMgr::OnReportResult(bool success)
{
	m_reporting = false;
	if (success)
	{
		m_report2World = true;
		m_dirty = true;
		LogInfo("Report To World Success");
	}
	else
	{
		LogInfo("Report To World Failed");
	}
}
