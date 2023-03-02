#ifndef _LEAGUE_MATCH_BASE_H_
#define _LEAGUE_MATCH_BASE_H_

struct LeagueMatchUnit
{
	LeagueMatchUnit():leagueTeamId(0), serverId(0), teamId(0), score(0), startTime(0) {}

	UINT64 leagueTeamId;
	UINT32 serverId;
	std::string serverName;
	UINT32 teamId;
	UINT32 score;
	UINT32 startTime;
	KKSG::LeagueBattleTeamData data;
};

class LeagueMatchBase
{
public:
	LeagueMatchBase();
	virtual ~LeagueMatchBase();

	int Start(LeagueMatchUnit& unit);
	void Cancel(UINT64 ltId);
	bool IsInMatch(UINT64 ltId);
	LeagueMatchUnit* GetMatchUnit(UINT64 ltId);
	void GetMatchLeagueIds(std::vector<UINT64>& leagueIds);

	virtual void OnMatchSuccess(LeagueMatchUnit& a, LeagueMatchUnit& b) = 0;
	virtual void OnMatchTimeOut(LeagueMatchUnit& unit) = 0;

	void DoMatch();
	void DoTimeOut();
	void DoCheckDelay();
	bool IsFit(UINT32 score1, UINT32 time1, UINT32 score2, UINT32 time2);

	UINT32 NewUid();
	UINT32 GetMatchNum() { return m_allUnits.size(); }

	struct FightGroup
	{
		LeagueMatchUnit teamA;
		LeagueMatchUnit teamB;
	};

	void AddToDelay(UINT64 ltId);
	void DeleteFromDelay(UINT64 ltId);
	bool IsInDelay(UINT64 ltId);

protected:
	UINT32 m_uid;
	std::unordered_map<UINT64, LeagueMatchUnit> m_allUnits;		// 正在匹配的队伍
	std::map<std::pair<UINT32, UINT64>, UINT32>	m_sortuUnits;	// 排好序的队伍 pair(分数，id) value: 开始匹配时间
	std::map<UINT32, FightGroup> m_matchedGroup;				// 匹配好的队伍
	std::unordered_map<UINT64, UINT32> m_delayUnits;						// 匹配成功之后延迟一段时间运行下次匹配
};

#endif