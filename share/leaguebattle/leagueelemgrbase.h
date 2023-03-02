#ifndef _LEAGUE_ELE_MGR_BASE_H_
#define _LEAGUE_ELE_MGR_BASE_H_

#include "leagueeledata.h"

#define ELI_INIT_NUM 8

// 淘汰赛管理器基类
class LeagueEleMgrBase
{
public:
	LeagueEleMgrBase();
	virtual ~LeagueEleMgrBase();

	void LoadFromKKSG(const KKSG::LBEleSaveData& data);
	void SaveToKKSG(KKSG::LBEleSaveData& data);

	LBEleminationRoom* GetRoom(UINT32 round, UINT32 index);
	LBEleminationRoom* GetRoom(UINT32 roomId);
	std::map<UINT32, LBEleminationRound>& GetRounds() { return m_rounds; }

	void InitRoundWithTeams(std::vector<LBEleminationTeam>& teams);
	void OnRoundStart();
	void CheckRoundEnd();
	void ClearRoundData();

	void SetLiveId(UINT32 roomId, UINT32 sceneId, UINT32 liveId);
	void OnBattleResult(UINT32 roomId, UINT32 sceneId, UINT64 winTeamId);
	void CheckChamption(UINT32 round, LBEleminationTeam& winTeam);

	virtual void NotifyRoomState(LBEleminationRoom* room) = 0;
	virtual void CreateBattle(LBEleminationRoom& room) = 0;
	virtual UINT64 GetWinIdByRank(UINT64 ltId1, UINT64 ltId2) = 0;
	virtual void HandleRoomResult(UINT32 round, LBEleminationTeam& winTeam, LBEleminationTeam& loseTeam) = 0;
	virtual void OnChamption() = 0;

protected:

	UINT32 GetRoomId(UINT32 round, UINT32 index);
	void GetRoundIndex(UINT32 roomId, UINT32& round, UINT32& index);

protected:
	bool m_dirty;
	UINT32 m_curRound;
	UINT32 m_updateTime;
	std::map<UINT32, LBEleminationRound> m_rounds;
	LBEleminationTeam m_chamption;
};

#endif