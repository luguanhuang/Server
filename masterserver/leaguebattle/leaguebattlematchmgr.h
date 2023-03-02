#ifndef _LEAGUE_BATTLE_MATCH_MGR_H_
#define _LEAGUE_BATTLE_MATCH_MGR_H_

#include "util/utilsingleton.h"
#include "timer.h"
#include "scene/scenecreate.h"
#include "leaguebattle/leaguematchbase.h"

class Team;
class LeagueBattleMatchMgr : public LeagueMatchBase, public ICreateBattleListener, 
							 public ITimer, public Singleton<LeagueBattleMatchMgr>
{
public:
	LeagueBattleMatchMgr();
	virtual ~LeagueBattleMatchMgr();

	bool Init();
	void Uninit();

	int StartMatch(UINT64 leagueTeamId, Team* team);
	int CancelMatch(Team* team);
	void ForceCancelMatch(UINT64 leagueTeamId);

	void OnCrossZoneUnusable();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);

	virtual void OnMatchSuccess(LeagueMatchUnit& a, LeagueMatchUnit& b);
	virtual void OnMatchTimeOut(LeagueMatchUnit& unit);

	void TimeOutFromWorld(UINT64 ltId, UINT32 teamId);
	void MatchSuccessFromWorld(UINT64 ltId, UINT32 teamId, UINT32 gsLine, UINT32 sceneId);

	void NotifyStartMatch(Team* team);
	void NotifyStopMatch(Team* team);
	void NotifyMatchTimeout(Team* team);
};

#endif