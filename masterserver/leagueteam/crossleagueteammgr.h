#ifndef _CROSS_LEAGUE_TEAM_MGR_H_
#define _CROSS_LEAGUE_TEAM_MGR_H_

#include "util/utilsingleton.h"
#include "leaguebattle/crossleagueteam.h"
#include "util/maplist.h"
#include "timer.h"
#include "pb/project.pb.h"

// world上跨服战队的副本, 只放内存
class CRoleSummary;
class CProtocol;
class CrossLeagueTeamMgr : public ITimer, public Singleton<CrossLeagueTeamMgr>
{
public:
	friend class SyncLeagueTeamHelper;

	CrossLeagueTeamMgr();
	virtual ~CrossLeagueTeamMgr();

	bool Init();
	void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void UpdateTeam(const KKSG::CrossLeagueTeamData& data);

	bool HasTeam(UINT64 id);
	CrossLeagueTeam* GetTeam(UINT64 id);
	CrossLeagueTeam* GetTeamByRoleId(UINT64 roleId);
	UINT64 GetRoleTeamId(UINT64 roleId);

	std::unordered_map<UINT64, CrossLeagueTeam>& GetTeams() { return m_teams; }

	void UpdateTeamRank(CrossLeagueTeam& team);

	void FillLeagueTeamDetail(CrossLeagueTeam* pLTeam, KKSG::LeagueTeamDetail& data);
	void FillLeagueMemberDetail(CRoleSummary* su, KKSG::LeagueTeamMemberDetail& data);

	void OnSeasonEnd();

	void OnRoleSummaryChanged(UINT64 roleId);
	bool IsCrossLeagueMember(UINT64 roleId);

	void BroadAllSelfServerMember(const CProtocol& ptc);

private:
	void SetRoleTeamId(UINT64 roleId, UINT64 teamId);
	void RemoveRoleTeamId(UINT64 roleId);

private:
	std::unordered_map<UINT64, CrossLeagueTeam> m_teams;
	std::unordered_map<UINT64, UINT64> m_roleId2leagueTeamId;	// roleId对应teamId

	MapList<UINT64> m_changedRoleIds;			// 本服参加跨服联赛， rolesummary有变化的玩家
};

#endif