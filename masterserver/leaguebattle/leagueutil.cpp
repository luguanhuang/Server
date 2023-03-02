#include "pch.h"
#include "leagueutil.h"
#include "util/timespecificmgr.h"
#include "leagueseasonmgr.h"
#include "util/XCommon.h"
#include "table/globalconfig.h"
#include "reswar/ptcm2c_halliconmntf.h"
#include "define/systemiddef.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "util/gametime.h"
#include "leagueteam/leagueteammgr.h"
#include "notice/notice.h"
#include "define/noticedef.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/leagueranklist.h"
#include "ranklist/crossleagueranklist.h"
#include "leagueteam/crossleagueteammgr.h"
#include "other/serverinfomgr.h"
#include "protocol.h"
#include "leaguebattle/leagueeliminationmgr.h"
#include "leaguebattle/leaguecrosselimgr.h"

void LeagueNoticeHelper::ClearTimers()
{
	LogInfo("ClearTimer[%u]", m_timers.size());
	for (auto it = m_timers.begin(); it != m_timers.end(); ++it)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(*it);
	}
	m_timers.clear();
}

void LeagueNoticeHelper::SetTimers(LeagueSeasonMgr* mgr)
{
	ClearTimers();
	KKSG::LeagueBattleTimeState curState = mgr->GetTodayState(); 
	UINT32 now = GameTime::GetTime();
	UINT32 year, month, day;
	XCommon::GetDate(year, month, day);

	// 报名期间
	if (curState == KKSG::LBTS_Apply)
	{
		// 报名日每日公告
		for (UINT32 i = 0; i < GetGlobalConfig().LeagueApplyNoticeTime.size(); ++i)
		{
			UINT32 value = GetGlobalConfig().LeagueApplyNoticeTime[i];
			UINT32 hour = value / 100;
			UINT32 min = value % 100;
			TimeSpecific startT(hour, min, 0);
			UINT32 timerId = CTimeSpecificMgr::Instance()->RegisterEvent(startT, std::bind(&LeagueNoticeHelper::NoticeApply, this));
			m_timers.push_back(timerId);
		}
		// 最后一天报名公告
		if (XCommon::IsDaySame(now, mgr->m_lastApplyTime, false))
		{
			for (UINT32 i = 0; i < GetGlobalConfig().LeagueApplyEndNoticeTime.size(); ++i)
			{
				UINT32 value = GetGlobalConfig().LeagueApplyEndNoticeTime[i];
				UINT32 hour = value / 100;
				UINT32 min = value % 100;
				TimeSpecific startT(hour, min, 0);
				UINT32 timerId = CTimeSpecificMgr::Instance()->RegisterEvent(startT, std::bind(&LeagueNoticeHelper::NoticeApplyEnd, this));
				m_timers.push_back(timerId);
			}
		}
	}
	// 积分赛
	else if (curState == KKSG::LBTS_PointRace)
	{
		// 积分赛公告
		for (UINT32 i = 0; i < GetGlobalConfig().LeagueRaceNoticeTime.size(); ++i)
		{
			UINT32 value = GetGlobalConfig().LeagueRaceNoticeTime[i];
			UINT32 hour = value / 100;
			UINT32 min = value % 100;
			TimeSpecific startT(hour, min, 0);
			UINT32 timerId = CTimeSpecificMgr::Instance()->RegisterEvent(startT, std::bind(&LeagueNoticeHelper::NoticeRace, this));
			m_timers.push_back(timerId);
		}

		// 倒计时公告
		UINT32 hour = GetGlobalConfig().LeagueBattlePointRaceTime[0] / 100;
		UINT32 min = GetGlobalConfig().LeagueBattlePointRaceTime[0] % 100;
		UINT32 startTime = XCommon::MakeTimeNew(year, month, day, hour, min);

		UINT32 timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 30 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeRace30min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 10 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeRace10min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 3 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeRace3min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 2 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeRace2min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 1 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeRace1min, this));
		m_timers.push_back(timerId);

		// 开始公告
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime),
						std::bind(&LeagueNoticeHelper::NoticeRaceStart, this));
		m_timers.push_back(timerId);

		// 结束公告
		UINT32 endHour = GetGlobalConfig().LeagueBattlePointRaceTime[1] / 100;
		UINT32 endMin = GetGlobalConfig().LeagueBattlePointRaceTime[1] % 100;
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(endHour, endMin, 0),
						std::bind(&LeagueNoticeHelper::NoticeRaceEnd, this));
		m_timers.push_back(timerId);

		// 开始Icon
		TimeSpecific startT(hour, min, 0);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(startT, std::bind(&LeagueNoticeHelper::NotifyRacePointBeginIcon, this));
		m_timers.push_back(timerId);

		// 结束Icon
		hour = GetGlobalConfig().LeagueBattlePointRaceTime[1] / 100;
		min = GetGlobalConfig().LeagueBattlePointRaceTime[1] % 100;
		TimeSpecific endT(hour, min, 0);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(endT, std::bind(&LeagueNoticeHelper::NotiyfRacePointEndIcon, this));
		m_timers.push_back(timerId);


		// 积分赛最后一天, 预告淘汰赛 
		if (XCommon::IsDaySame(now, mgr->m_lastRacePointDayZero, false))
		{
			timerId = CTimeSpecificMgr::Instance()->RegisterEvent(endT, std::bind(&LeagueNoticeHelper::NoticePreEliMatch, this));
			m_timers.push_back(timerId);
		}
	}
	else if (curState == KKSG::LBTS_CrossPointRace)
	{
		// 积分赛公告
		for (UINT32 i = 0; i < GetGlobalConfig().LeagueRaceNoticeTime.size(); ++i)
		{
			UINT32 value = GetGlobalConfig().LeagueRaceNoticeTime[i];
			UINT32 hour = value / 100;
			UINT32 min = value % 100;
			TimeSpecific startT(hour, min, 0);
			UINT32 timerId = CTimeSpecificMgr::Instance()->RegisterEvent(startT, std::bind(&LeagueNoticeHelper::NoticeCrossRace, this));
			m_timers.push_back(timerId);
		}

		// 倒计时公告
		UINT32 hour = GetGlobalConfig().LeagueBattlePointRaceTime[0] / 100;
		UINT32 min = GetGlobalConfig().LeagueBattlePointRaceTime[0] % 100;
		UINT32 startTime = XCommon::MakeTimeNew(year, month, day, hour, min);

		UINT32 timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 30 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeCrossRace30min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 10 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeCrossRace10min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 3 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeCrossRace3min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 2 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeCrossRace2min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 1 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeCrossRace1min, this));
		m_timers.push_back(timerId);

		// 开始公告
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime),
						std::bind(&LeagueNoticeHelper::NoticeCrossRaceStart, this));
		m_timers.push_back(timerId);

		// 结束公告
		UINT32 endHour = GetGlobalConfig().LeagueBattlePointRaceTime[1] / 100;
		UINT32 endMin = GetGlobalConfig().LeagueBattlePointRaceTime[1] % 100;
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(endHour, endMin, 0),
						std::bind(&LeagueNoticeHelper::NoticeCrossRaceEnd, this));
		m_timers.push_back(timerId);


		// 开始Icon
		TimeSpecific startT(hour, min, 0);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(startT, std::bind(&LeagueNoticeHelper::NotifyCrossRacePointBeginIcon, this));
		m_timers.push_back(timerId);

		// 结束Icon
		hour = GetGlobalConfig().LeagueBattlePointRaceTime[1] / 100;
		min = GetGlobalConfig().LeagueBattlePointRaceTime[1] % 100;
		TimeSpecific endT(hour, min, 0);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(endT, std::bind(&LeagueNoticeHelper::NotiyfCrossRacePointEndIcon, this));
		m_timers.push_back(timerId);

		// 积分赛最后一天, 预告淘汰赛 
		if (XCommon::IsDaySame(now, mgr->m_lastCrossRacePointDayZero, false))
		{
			timerId = CTimeSpecificMgr::Instance()->RegisterEvent(endT, std::bind(&LeagueNoticeHelper::NoticeCrossPreEliMatch, this));
			m_timers.push_back(timerId);
		}
	}
	else if (curState == KKSG::LBTS_Elimination)
	{
		// 定时公告
		for (UINT32 i = 0; i < GetGlobalConfig().LeagueEliNoticeTime.size(); ++i)
		{
			UINT32 value = GetGlobalConfig().LeagueEliNoticeTime[i];
			UINT32 hour = value / 100;
			UINT32 min = value % 100;
			TimeSpecific startT(hour, min, 0);
			UINT32 timerId = CTimeSpecificMgr::Instance()->RegisterEvent(startT, std::bind(&LeagueNoticeHelper::NoticeEli, this));
			m_timers.push_back(timerId);
		}

		if (GetGlobalConfig().LeagueBattleEliminationTime.Size() < 3)
		{
			LogError("error");
			return;
		}

		// 第一轮淘汰赛倒计时公告
		UINT32 hour = GetGlobalConfig().LeagueBattleEliminationTime[0] / 100;
		UINT32 min = GetGlobalConfig().LeagueBattleEliminationTime[0] % 100;
		UINT32 startTime = XCommon::MakeTimeNew(year, month, day, hour, min);

		UINT32 timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 30 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeFirstEli30min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 10 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeFirstEli10min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 3 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeFirstEli3min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 2 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeFirstEli2min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 1 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeFirstEli1min, this));
		m_timers.push_back(timerId);

		// 第一轮开始公告
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime),
						std::bind(&LeagueNoticeHelper::NoticeFirstEliStart, this));
		m_timers.push_back(timerId);

		// 第二轮倒计时
		hour = GetGlobalConfig().LeagueBattleEliminationTime[1] / 100;
		min = GetGlobalConfig().LeagueBattleEliminationTime[1] % 100;
		startTime = XCommon::MakeTimeNew(year, month, day, hour, min);

		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 3 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeSecondEli3min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 2 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeSecondEli2min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 1 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeSecondEli1min, this));
		m_timers.push_back(timerId);

		// 第二轮开始公告
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime),
						std::bind(&LeagueNoticeHelper::NoticeSecondEliStart, this));
		m_timers.push_back(timerId);

		// 第三轮倒计时
		hour = GetGlobalConfig().LeagueBattleEliminationTime[2] / 100;
		min = GetGlobalConfig().LeagueBattleEliminationTime[2] % 100;
		startTime = XCommon::MakeTimeNew(year, month, day, hour, min);

		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 3 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeThirdEli3min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 2 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeThirdEli2min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 1 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeThirdEli1min, this));
		m_timers.push_back(timerId);

		// 第三轮开始公告
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime),
						std::bind(&LeagueNoticeHelper::NoticeThirdEliStart, this));
		m_timers.push_back(timerId);

		// 淘汰赛三轮开始icon
		for (int i = 0; i < 3; ++i)
		{
			hour = GetGlobalConfig().LeagueBattleEliminationTime[i] / 100;
			min = GetGlobalConfig().LeagueBattleEliminationTime[i] % 100;
			// 开始icon
			timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(hour, min, 0),
						  std::bind(&LeagueNoticeHelper::NotifyEliStartIcon, this));
			m_timers.push_back(timerId);
		}
		// 结束icon
		hour = GetGlobalConfig().LeagueBattleEliminationTime[3] / 100;
		min = GetGlobalConfig().LeagueBattleEliminationTime[3] % 100;
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(hour, min, 0),
					  std::bind(&LeagueNoticeHelper::NotifyEliEndIcon, this));
		m_timers.push_back(timerId);

		// 淘汰赛结束
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(hour, min, 0),
					  std::bind(&LeagueNoticeHelper::NoticeEliEnd, this));
		m_timers.push_back(timerId);
	}
	else if (curState == KKSG::LBTS_CrossElimination)
	{
		// 定时公告
		for (UINT32 i = 0; i < GetGlobalConfig().LeagueEliNoticeTime.size(); ++i)
		{
			UINT32 value = GetGlobalConfig().LeagueEliNoticeTime[i];
			UINT32 hour = value / 100;
			UINT32 min = value % 100;
			TimeSpecific startT(hour, min, 0);
			UINT32 timerId = CTimeSpecificMgr::Instance()->RegisterEvent(startT, std::bind(&LeagueNoticeHelper::NoticeCrossEli, this));
			m_timers.push_back(timerId);
		}

		if (GetGlobalConfig().LeagueBattleEliminationTime.Size() < 3)
		{
			LogError("error");
			return;
		}

		// 第一轮淘汰赛倒计时公告
		UINT32 hour = GetGlobalConfig().LeagueBattleEliminationTime[0] / 100;
		UINT32 min = GetGlobalConfig().LeagueBattleEliminationTime[0] % 100;
		UINT32 startTime = XCommon::MakeTimeNew(year, month, day, hour, min);

		UINT32 timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 30 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeFirstCrossEli30min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 10 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeFirstCrossEli10min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 3 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeFirstCrossEli3min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 2 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeFirstCrossEli2min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 1 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeFirstCrossEli1min, this));
		m_timers.push_back(timerId);

		// 第一轮开始公告
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime),
						std::bind(&LeagueNoticeHelper::NoticeFirstCrossEliStart, this));
		m_timers.push_back(timerId);

		// 第二轮倒计时
		hour = GetGlobalConfig().LeagueBattleEliminationTime[1] / 100;
		min = GetGlobalConfig().LeagueBattleEliminationTime[1] % 100;
		startTime = XCommon::MakeTimeNew(year, month, day, hour, min);

		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 3 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeSecondCrossEli3min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 2 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeSecondCrossEli2min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 1 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeSecondCrossEli1min, this));
		m_timers.push_back(timerId);

		// 第二轮开始公告
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime),
						std::bind(&LeagueNoticeHelper::NoticeSecondCrossEliStart, this));
		m_timers.push_back(timerId);

		// 第三轮倒计时
		hour = GetGlobalConfig().LeagueBattleEliminationTime[2] / 100;
		min = GetGlobalConfig().LeagueBattleEliminationTime[2] % 100;
		startTime = XCommon::MakeTimeNew(year, month, day, hour, min);

		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 3 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeThirdCrossEli3min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 2 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeThirdCrossEli2min, this));
		m_timers.push_back(timerId);
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime - 1 * 60),
						 std::bind(&LeagueNoticeHelper::NoticeThirdCrossEli1min, this));
		m_timers.push_back(timerId);

		// 第三轮开始公告
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(startTime),
						std::bind(&LeagueNoticeHelper::NoticeThirdCrossEliStart, this));
		m_timers.push_back(timerId);

		// 淘汰赛三轮开始icon
		for (int i = 0; i < 3; ++i)
		{
			hour = GetGlobalConfig().LeagueBattleEliminationTime[i] / 100;
			min = GetGlobalConfig().LeagueBattleEliminationTime[i] % 100;
			// 开始icon
			timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(hour, min, 0),
						  std::bind(&LeagueNoticeHelper::NotifyCrossEliStartIcon, this));
			m_timers.push_back(timerId);
		}

		// 结束icon
		hour = GetGlobalConfig().LeagueBattleEliminationTime[3] / 100;
		min = GetGlobalConfig().LeagueBattleEliminationTime[3] % 100;
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(hour, min, 0),
					  std::bind(&LeagueNoticeHelper::NotifyCrossEliEndIcon, this));
		m_timers.push_back(timerId);

		// 淘汰赛结束
		timerId = CTimeSpecificMgr::Instance()->RegisterEvent(TimeSpecific(hour, min, 0),
					  std::bind(&LeagueNoticeHelper::NoticeCrossEliEnd, this));
		m_timers.push_back(timerId);
	}

	LogInfo("RegisterTimer[%u]", m_timers.size());
}

bool LeagueNoticeHelper::NotifyRacePointBeginIcon()
{
	PtcM2C_HallIconMNtf ntf;
	ntf.m_Data.set_state(KKSG::HICONS_BEGIN);
	ntf.m_Data.set_systemid(SYS_TEAMLEAGUE);
	NotifyToLTeamMember(ntf);
	return true;
}

bool LeagueNoticeHelper::NotiyfRacePointEndIcon()
{
	PtcM2C_HallIconMNtf ntf;
	ntf.m_Data.set_state(KKSG::HICONS_END);
	ntf.m_Data.set_systemid(SYS_TEAMLEAGUE);
	NotifyToLTeamMember(ntf);
	return true;
}

bool LeagueNoticeHelper::NotifyEliStartIcon()
{
	PtcM2C_HallIconMNtf ntf;
	ntf.m_Data.set_state(KKSG::HICONS_BEGIN);
	ntf.m_Data.set_systemid(SYS_TEAMLEAGUE);
	NotifyToEliMember(ntf);
	return true;
}

bool LeagueNoticeHelper::NotifyEliEndIcon()
{
	PtcM2C_HallIconMNtf ntf;
	ntf.m_Data.set_state(KKSG::HICONS_END);
	ntf.m_Data.set_systemid(SYS_TEAMLEAGUE);
	NotifyToEliMember(ntf);
	return true;
}

bool LeagueNoticeHelper::NotifyCrossRacePointBeginIcon()
{
	PtcM2C_HallIconMNtf ntf;
	ntf.m_Data.set_state(KKSG::HICONS_BEGIN);
	ntf.m_Data.set_systemid(SYS_TEAMLEAGUE);
	NotifyToCrossMember(ntf);
	return true;
}

bool LeagueNoticeHelper::NotiyfCrossRacePointEndIcon()
{
	PtcM2C_HallIconMNtf ntf;
	ntf.m_Data.set_state(KKSG::HICONS_END);
	ntf.m_Data.set_systemid(SYS_TEAMLEAGUE);
	NotifyToCrossMember(ntf);
	return true;
}

bool LeagueNoticeHelper::NoticeApply()
{
	LogInfo("NoticeType_LeagueStartApply");
	Notice notice(NoticeType_LeagueStartApply);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeApplyEnd()
{
	LogInfo("NoticeType_LeagueEndApply");
	Notice notice(NoticeType_LeagueEndApply);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeRace()
{
	LogInfo("NoticeType_LeagueRace");
	Notice notice(NoticeType_LeagueRace);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeRace30min()
{
	LogInfo("NoticeType_LeagueRace30Min");
	Notice notice(NoticeType_LeagueRace30Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeRace10min()
{
	LogInfo("NoticeType_LeagueRace10Min");
	Notice notice(NoticeType_LeagueRace10Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeRace3min()
{
	LogInfo("NoticeType_LeagueRace3Min");
	Notice notice(NoticeType_LeagueRace3Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeRace2min()
{
	LogInfo("NoticeType_LeagueRace2Min");
	Notice notice(NoticeType_LeagueRace2Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeRace1min()
{
	LogInfo("NoticeType_LeagueRace1Min");
	Notice notice(NoticeType_LeagueRace1Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeRaceStart()
{
	Notice notice(NoticeType_LeagueRaceStart);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeRaceEnd()
{
	LeagueRankList* ranklist = CRankListMgr::Instance()->GetLeagueRankList();
	const LeagueRankInfo* info = ranklist->GetInfoByRank(1);
	if (info == NULL)
	{
		return true;
	}
	LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(info->GetUniqueId());
	if (pLTeam == NULL)
	{
		return true;
	}

	Notice notice(NoticeType_LeagueRaceEnd);
	notice.Replace("$S", pLTeam->GetName());
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticePreEliMatch()
{
	Notice notice(NoticeType_LeaguePreEle);
	notice.AddLink();
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeCrossRace()
{
	LogInfo("NoticeType_LeagueCrossRace");
	Notice notice(NoticeType_LeagueCrossRace);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeCrossRace30min()
{
	LogInfo("NoticeType_LeagueCrossRace30Min");
	Notice notice(NoticeType_LeagueCrossRace30Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeCrossRace10min()
{
	LogInfo("NoticeType_LeagueCrossRace10Min");
	Notice notice(NoticeType_LeagueCrossRace10Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeCrossRace3min()
{
	LogInfo("NoticeType_LeagueCrossRace3Min");
	Notice notice(NoticeType_LeagueCrossRace3Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeCrossRace2min()
{
	LogInfo("NoticeType_LeagueCrossRace2Min");
	Notice notice(NoticeType_LeagueCrossRace2Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeCrossRace1min()
{
	LogInfo("NoticeType_LeagueCrossRace1Min");
	Notice notice(NoticeType_LeagueCrossRace1Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeCrossRaceStart()
{
	LogInfo("NoticeType_LeagueCrossRace");
	Notice notice(NoticeType_LeagueCrossRaceStart);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeCrossRaceEnd()
{
	LogInfo("NoticeType_LeagueCrossRaceEnd");
	CrossLeagueRankList* ranklist = CRankListMgr::Instance()->GetCrossLeagueRankList();
	const CrossLeagueRankInfo* info = ranklist->GetInfoByRank(1);
	if (info == NULL)
	{
		return true;
	}
	CrossLeagueTeam* pLTeam = CrossLeagueTeamMgr::Instance()->GetTeam(info->GetUniqueId());
	if (pLTeam == NULL)
	{
		return true;
	}

	Notice notice(NoticeType_LeagueCrossRaceEnd);
	notice.Replace("$M", ZServerInfoMgr::Instance()->GetServerName(pLTeam->m_serverId));
	notice.Replace("$S", pLTeam->m_name);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeCrossPreEliMatch()
{
	LogInfo("NoticeType_LeagueCrossPreEle");
	Notice notice(NoticeType_LeagueCrossPreEle);
	notice.AddLink();
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeEli()
{
	LogInfo("NoticeType_LeagueEli");
	Notice notice(NoticeType_LeagueEli);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeFirstEli30min()
{
	LogInfo("NoticeType_LeagueFirstEli30Min");
	Notice notice(NoticeType_LeagueFirstEli30Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeFirstEli10min()
{
	LogInfo("NoticeType_LeagueFirstEli10Min");
	Notice notice(NoticeType_LeagueFirstEli10Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeFirstEli3min()
{
	LogInfo("NoticeType_LeagueFirstEli3Min");
	Notice notice(NoticeType_LeagueFirstEli3Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeFirstEli2min()
{
	LogInfo("NoticeType_LeagueFirstEli2Min");
	Notice notice(NoticeType_LeagueFirstEli2Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeFirstEli1min()
{
	LogInfo("NoticeType_LeagueFirstEli1Min");
	Notice notice(NoticeType_LeagueFirstEli1Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeFirstEliStart()
{
	LogInfo("NoticeType_LeagueFirstEliStart");
	Notice notice(NoticeType_LeagueFirstEliStart);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeSecondEli3min()
{
	LogInfo("NoticeType_LeagueSecondEli3Min");
	Notice notice(NoticeType_LeagueSecondEli3Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeSecondEli2min()
{
	LogInfo("NoticeType_LeagueSecondEli2Min");
	Notice notice(NoticeType_LeagueSecondEli2Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeSecondEli1min()
{
	LogInfo("NoticeType_LeagueSecondEli1Min");
	Notice notice(NoticeType_LeagueSecondEli1Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeSecondEliStart()
{
	LogInfo("NoticeType_LeagueSecondEliStart");
	Notice notice(NoticeType_LeagueSecondEliStart);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeThirdEli3min()
{
	LogInfo("NoticeType_LeagueThirdEli3Min");
	Notice notice(NoticeType_LeagueThirdEli3Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeThirdEli2min()
{
	LogInfo("NoticeType_LeagueThirdEli2Min");
	Notice notice(NoticeType_LeagueThirdEli2Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeThirdEli1min()
{
	LogInfo("NoticeType_LeagueThirdEli1Min");
	Notice notice(NoticeType_LeagueThirdEli1Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeThirdEliStart()
{
	LogInfo("NoticeType_LeagueThirdEliStart");
	Notice notice(NoticeType_LeagueThirdEliStart);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeEliEnd()
{
	LogInfo("NoticeType_LeagueEliEnd");
	Notice notice(NoticeType_LeagueEliEnd);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeCrossEli()
{
	LogInfo("NoticeType_LeagueCrossEli");
	Notice notice(NoticeType_LeagueCrossEli);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeFirstCrossEli30min()
{
	LogInfo("NoticeType_LeagueFirstCrossEli30Min");
	Notice notice(NoticeType_LeagueFirstCrossEli30Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeFirstCrossEli10min()
{
	LogInfo("NoticeType_LeagueFirstCrossEli10Min");
	Notice notice(NoticeType_LeagueFirstCrossEli10Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeFirstCrossEli3min()
{
	LogInfo("NoticeType_LeagueFirstCrossEli3Min");
	Notice notice(NoticeType_LeagueFirstCrossEli3Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeFirstCrossEli2min()
{	
	LogInfo("NoticeType_LeagueFirstCrossEli2Min");
	Notice notice(NoticeType_LeagueFirstCrossEli2Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeFirstCrossEli1min()
{
	LogInfo("NoticeType_LeagueFirstCrossEli1Min");
	Notice notice(NoticeType_LeagueFirstCrossEli1Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeFirstCrossEliStart()
{
	LogInfo("NoticeType_LeagueFirstCrossEliStart");
	Notice notice(NoticeType_LeagueFirstCrossEliStart);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeSecondCrossEli3min()
{
	LogInfo("NoticeType_LeagueSecondCrossEli3Min");
	Notice notice(NoticeType_LeagueSecondCrossEli3Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeSecondCrossEli2min()
{
	LogInfo("NoticeType_LeagueSecondCrossEli2Min");
	Notice notice(NoticeType_LeagueSecondCrossEli2Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeSecondCrossEli1min()
{
	LogInfo("NoticeType_LeagueSecondCrossEli1Min");
	Notice notice(NoticeType_LeagueSecondCrossEli1Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeSecondCrossEliStart()
{
	LogInfo("NoticeType_LeagueSecondCrossEliStart");
	Notice notice(NoticeType_LeagueSecondCrossEliStart);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeThirdCrossEli3min()
{
	LogInfo("NoticeType_LeagueThirdCrossEli3Min");
	Notice notice(NoticeType_LeagueThirdCrossEli3Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeThirdCrossEli2min()
{
	LogInfo("NoticeType_LeagueThirdCrossEli2Min");
	Notice notice(NoticeType_LeagueThirdCrossEli2Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeThirdCrossEli1min()
{
	LogInfo("NoticeType_LeagueThirdCrossEli1Min");
	Notice notice(NoticeType_LeagueThirdCrossEli1Min);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeThirdCrossEliStart()
{
	LogInfo("NoticeType_LeagueThirdCrossEliStart");
	Notice notice(NoticeType_LeagueThirdCrossEliStart);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NoticeCrossEliEnd()
{
	LogInfo("NoticeType_LeagueCrossEliEnd");
	Notice notice(NoticeType_LeagueCrossEliEnd);
	notice.Send();
	return true;
}

bool LeagueNoticeHelper::NotifyCrossEliStartIcon()
{
	LogInfo("NotifyCrossEliStartIcon");
	PtcM2C_HallIconMNtf ntf;
	ntf.m_Data.set_state(KKSG::HICONS_BEGIN);
	ntf.m_Data.set_systemid(SYS_TEAMLEAGUE);
	NotifyToEliMember(ntf);
	return true;
}

bool LeagueNoticeHelper::NotifyCrossEliEndIcon()
{
	LogInfo("NotifyCrossEliEndIcon");
	PtcM2C_HallIconMNtf ntf;
	ntf.m_Data.set_state(KKSG::HICONS_END);
	ntf.m_Data.set_systemid(SYS_TEAMLEAGUE);
	NotifyToEliMember(ntf);
	return true;
}

void LeagueNoticeHelper::NotifyToAll(const CProtocol& ntf)
{
	for (auto it = CRoleManager::Instance()->Begin(); it != CRoleManager::Instance()->End(); ++it)
	{
		CRole* pRole = *it;
		pRole->Send(ntf);
	}
}

void LeagueNoticeHelper::NotifyToLTeamMember(const CProtocol& ntf)
{
	for (auto it = CRoleManager::Instance()->Begin(); it != CRoleManager::Instance()->End(); ++it)
	{
		CRole* pRole = *it;
		if (!LeagueTeamMgr::Instance()->HasLeagueTeam(pRole->GetID()))
		{
			continue;
		}
		pRole->Send(ntf);
	}
}

void LeagueNoticeHelper::NotifyToEliMember(const CProtocol& ntf)
{
	if (LeagueSeasonMgr::Instance()->IsInCrossState())
	{
		LeagueCrossEliMgr::Instance()->BroadCastToMember(ntf);
	}
	else
	{
		LeagueEliminationMgr::Instance()->BroadCastToMember(ntf);
	}
}

void LeagueNoticeHelper::NotifyToCrossMember(const CProtocol& ntf)
{
	CrossLeagueTeamMgr::Instance()->BroadAllSelfServerMember(ntf);
}
