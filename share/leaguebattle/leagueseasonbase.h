#ifndef _LEAGUE_SEASON_BASE_H_
#define _LEAGUE_SEASON_BASE_H_

// 联赛赛季管理基类
class LeagueSeasonBase
{
public:
	LeagueSeasonBase();
	virtual ~LeagueSeasonBase();

	KKSG::LeagueBattleTimeState GetTodayState() { return m_todayState; }
	UINT32 GetMatchLeftTime();

	UINT32 GetPreStartTime();
	UINT32 GetNextStartTime();

	void DumpSeason(std::string& output);

	bool IsCrossPointRaceDay();
	bool IsInCrossState();
	bool IsShowEli();
	KKSG::LeagueEliType GetEliType();
	

protected:
	void BuildTimeState();
	KKSG::LeagueBattleTimeState GetCurStateByTime(UINT32 now);

protected:
	UINT32 m_seasonOpenTime;
	UINT32 m_seasonEndTime;
	UINT32 m_nextSeasonOpenTime;

	std::map<UINT32, KKSG::LeagueBattleTimeState> m_stateMap;	// 状态Map
	UINT32 m_lastCanOpenTime;									// 最后可以开启的时间(现在是等最后一个报名日零点)
	UINT32 m_lastApplyTime;										// 最后一个报名日零点
	UINT32 m_lastRacePointDayZero;								// 最后一场积分赛日期
	UINT32 m_eleMapBuildTime;									// 本服八强对阵图生成时间(同时也是本服排名奖励发放时间)
	UINT32 m_selfEndTime;										// 本服结束时间
	UINT32 m_crossStartTime;									// 跨服开始时间
	UINT32 m_crossEleMapBuildTime;								// 跨服八强对阵图生成时间(同时也是跨服排名奖励发放时间)
	UINT32 m_lastCrossRacePointDayZero;							// 最后一场跨服积分赛日期
//	UINT32 m_seasonClearDayZero;								// 赛季清空时间
	KKSG::LeagueBattleTimeState m_todayState;					// 今天的状态

	bool m_clearSelfServer;										// 本服结束后是否清理
	UINT32 m_clearSelfTime;										// 本服结束后清理时间
	UINT32 m_lastCheckOpenTime;									// 上次检查赛季开启的时间

	UINT32 m_selfEliShowStartTime;								// 本服淘汰赛展示开始时间
	UINT32 m_selfEliShowEndTime;								// 本服淘汰赛展示结束时间
	UINT32 m_crossEliShowStartTime;								// 跨服淘汰赛展示开始时间
	UINT32 m_crossEliShowEndTime;								// 跨服淘汰赛展示结束时间
};

#endif