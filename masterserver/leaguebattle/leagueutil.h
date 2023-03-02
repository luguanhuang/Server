#ifndef _LEAGUE_UTIL_H_
#define _LEAGUE_UTIL_H_

class LeagueSeasonMgr;

class CProtocol;
// 推送和icon发送器
class LeagueNoticeHelper
{
public:
	void SetTimers(LeagueSeasonMgr* mgr);
	void ClearTimers();

	// 报名
	bool NoticeApply();				// 报名期间notice
	bool NoticeApplyEnd();			// 报名结束notice

	// 积分赛
	bool NoticeRace();				// 积分赛公告
	bool NoticeRace30min();
	bool NoticeRace10min();
	bool NoticeRace3min();
	bool NoticeRace2min();
	bool NoticeRace1min();
	bool NoticeRaceStart();
	bool NoticeRaceEnd();
	bool NoticePreEliMatch();	// 预告淘汰赛

	// 淘汰赛
	bool NoticeEli();
	// 第一轮淘汰赛倒计时
	bool NoticeFirstEli30min();
	bool NoticeFirstEli10min();
	bool NoticeFirstEli3min();
	bool NoticeFirstEli2min();
	bool NoticeFirstEli1min();
	bool NoticeFirstEliStart();

	// 第二轮淘汰赛倒计时
	bool NoticeSecondEli3min();
	bool NoticeSecondEli2min();
	bool NoticeSecondEli1min();
	bool NoticeSecondEliStart();

	// 第三轮淘汰赛倒计时
	bool NoticeThirdEli3min();
	bool NoticeThirdEli2min();
	bool NoticeThirdEli1min();
	bool NoticeThirdEliStart();
	bool NoticeEliEnd();

	// 淘汰赛Icon
	bool NotifyEliStartIcon();
	bool NotifyEliEndIcon();


	// 跨服积分赛
	bool NoticeCrossRace();				// 积分赛公告
	bool NoticeCrossRace30min();
	bool NoticeCrossRace10min();
	bool NoticeCrossRace3min();
	bool NoticeCrossRace2min();
	bool NoticeCrossRace1min();
	bool NoticeCrossRaceStart();
	bool NoticeCrossRaceEnd();
	bool NoticeCrossPreEliMatch();	// 预告淘汰赛

	// 跨服淘汰赛
	bool NoticeCrossEli();
	// 第一轮跨服淘汰赛倒计时
	bool NoticeFirstCrossEli30min();
	bool NoticeFirstCrossEli10min();
	bool NoticeFirstCrossEli3min();
	bool NoticeFirstCrossEli2min();
	bool NoticeFirstCrossEli1min();
	bool NoticeFirstCrossEliStart();

	// 第二轮跨服淘汰赛倒计时
	bool NoticeSecondCrossEli3min();
	bool NoticeSecondCrossEli2min();
	bool NoticeSecondCrossEli1min();
	bool NoticeSecondCrossEliStart();

	// 第三轮跨服淘汰赛倒计时
	bool NoticeThirdCrossEli3min();
	bool NoticeThirdCrossEli2min();
	bool NoticeThirdCrossEli1min();
	bool NoticeThirdCrossEliStart();
	bool NoticeCrossEliEnd();

	// 跨服淘汰赛Icon
	bool NotifyCrossEliStartIcon();
	bool NotifyCrossEliEndIcon();


	bool NotifyRacePointBeginIcon();
	bool NotiyfRacePointEndIcon();
	bool NotifyCrossRacePointBeginIcon();
	bool NotiyfCrossRacePointEndIcon();

	void NotifyToAll(const CProtocol& ntf);
	void NotifyToLTeamMember(const CProtocol& ntf);
	void NotifyToEliMember(const CProtocol& ntf);
	void NotifyToCrossMember(const CProtocol& ntf);


private:
	std::list<UINT32> m_timers;
};

#endif