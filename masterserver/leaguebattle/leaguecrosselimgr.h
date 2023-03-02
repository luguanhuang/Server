#ifndef _LEAGUE_CROSS_ELI_MGR_H_
#define _LEAGUE_CROSS_ELI_MGR_H_


#include "util/utilsingleton.h"
#include "pb/enum.pb.h"
#include "leaguebattle/leagueelemgrbase.h"


// 跨服淘汰赛在MS的一个副本， 只有数据， 只放内存
class CProtocol;
class LeagueCrossEliMgr : public LeagueEleMgrBase, public Singleton<LeagueCrossEliMgr>
{
public:
	LeagueCrossEliMgr();
	~LeagueCrossEliMgr();

	bool Init();
	void Uninit();

	void UpdateAllData(const KKSG::LBEleSaveData& data);
	void UpdateOneRoom(const KKSG::LBEleRoomData& room);
	void UpdateChamption(const KKSG::LBEleTeamData& champ);

	void TrySetLTeamBattleStart(LBEleminationRoom* pRoom);
	void TrySetLTeamBattleFinish(LBEleminationRoom* pRoom);

	void FillEliInfo(KKSG::GetLeagueEleInfoRes& data);
	void FillTeamInfo(LBEleminationTeam* team, KKSG::LBEleTeamInfo& info);
	void FillRoomInfo(LBEleminationRoom* room, KKSG::LBEleRoomInfo& info);
	void FillRoundInfo(LBEleminationRound* round, KKSG::LBEleRoundInfo& info);

	void FillChamptionInfo(KKSG::LeagueTeamDetail& data);

	// 无用虚函数
	virtual void NotifyRoomState(LBEleminationRoom* room);
	virtual void CreateBattle(LBEleminationRoom& room) {}
	virtual UINT64 GetWinIdByRank(UINT64 ltId1, UINT64 ltId2) { return 0; }
	virtual void HandleRoomResult(UINT32 round, LBEleminationTeam& winTeam, LBEleminationTeam& loseTeam) {}
	virtual void OnChamption() {}

	void BroadCastToMember(const CProtocol& ntf);
};

#endif