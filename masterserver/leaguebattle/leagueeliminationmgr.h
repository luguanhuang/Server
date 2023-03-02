#ifndef _LEAGUE_ELIMINATION_MGR_H_
#define _LEAGUE_ELIMINATION_MGR_H_

#include "util/utilsingleton.h"
#include "timer.h"
#include "pb/enum.pb.h"
#include "scene/scenecreate.h"
#include "leaguebattle/leagueelemgrbase.h"

class CProtocol;
class LeagueEliminationMgr : public LeagueEleMgrBase, public ICreateBattleListener, public ITimer, public Singleton<LeagueEliminationMgr>
{
public:
	LeagueEliminationMgr();
	~LeagueEliminationMgr();

	bool Init();
	void Uninit();
	void SaveAll2DB();

	void LoadDataFromDB();
	void SaveDataToDB();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);

	// 初始化生成对阵图
	void InitRound();

	virtual void NotifyRoomState(LBEleminationRoom* room);
	virtual void CreateBattle(LBEleminationRoom& room);
	virtual UINT64 GetWinIdByRank(UINT64 ltId1, UINT64 ltId2);
	virtual void HandleRoomResult(UINT32 round, LBEleminationTeam& winTeam, LBEleminationTeam& loseTeam);
	virtual void OnChamption();

	void OnTeamEndBattle(UINT64 ltId);
	void OnSeasonEnd();

	void BroadCast(const CProtocol& ntf);
	void BroadCastToMember(const CProtocol& ntf);
	void SetDirty(bool dirty = true) { m_dirty = dirty; }

	void AddWatcher(UINT64 roleId);
	void RemoveWatcher(UINT64 roleId);

	void FillChamptionInfo(KKSG::LeagueTeamDetail& data);

	void FillTeamInfo(LBEleminationTeam* team, KKSG::LBEleTeamInfo& info);
	void FillRoomInfo(LBEleminationRoom* room, KKSG::LBEleRoomInfo& info);
	void FillRoundInfo(LBEleminationRound* round, KKSG::LBEleRoundInfo& info);

private:
	std::set<UINT64> m_watchRoleIds;			// 停留在对阵图的玩家
};

#endif