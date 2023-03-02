#ifndef _SKY_CRAFT_ELI_MGR_H_
#define _SKY_CRAFT_ELI_MGR_H_

#include "util/utilsingleton.h"
#include "timer.h"
#include "pb/enum.pb.h"
#include "scene/scenecreate.h"

#include "skycraftelidata.h"

#define SKYCRAFT_ELI_INIT_NUM 8

// 淘汰赛管理器
class CProtocol;
class SkyCraftEliMgr : public ICreateBattleListener, public ITimer, public Singleton<SkyCraftEliMgr>
{
public:
	SkyCraftEliMgr();
	virtual ~SkyCraftEliMgr();

	bool Init();
	void Uninit();

	void LoadFromDB();
	void SaveToDB();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);

	void LoadFromKKSG(const KKSG::SCEliSaveData& data);
	void SaveToKKSG(KKSG::SCEliSaveData& data);

	void InitRound();
	void OnRoundStart();
	void CheckRoundEnd();
	void ClearRoundData();

	SkyCraftEliRoom* GetRoom(UINT32 roomId);
	SkyCraftEliRoom* GetRoom(UINT32 round, UINT32 index);

	void SetLiveId(UINT32 roomId, UINT32 sceneId, UINT32 liveId);
	void OnBattleResult(UINT32 roomId, UINT32 sceneId, UINT64 winTeamId);

	void NotifyRoomState(SkyCraftEliRoom* room);

	void OnSeasonEnd();

	void BroadCast(const CProtocol& ntf);
	void SetDirty(bool dirty = true) { m_dirty = dirty; }

	void AddWatcher(UINT64 roleId);
	void RemoveWatcher(UINT64 roleId);

	void FillEliInfo(KKSG::GetSkyCraftEliInfoRes& res);

private:
	void InitRoundWithTeams(std::vector<UINT64>& teamIds);

	UINT32 GetRoomId(UINT32 round, UINT32 index);
	void GetRoundIndex(UINT32 roomId, UINT32& round, UINT32& index);
	void SetPromoteId(UINT32 currRound, UINT32 index, UINT64 winStId);

	void CheckChamption(UINT32 round, UINT64 winStId);
	void CreateBattle(SkyCraftEliRoom& room);
	UINT64 GetWinIdByRank(UINT64 stId1, UINT64 stId2);

	void HandleRoomResult(UINT32 round, UINT64 winStId, UINT64 loseStId);
	void OnChamption();
	void OnTeamEndBattle(UINT64 stId);

private:
	bool m_dirty;
	KKSG::SCEliRoundType m_curRound;
	UINT32 m_updateTime;
	UINT64 m_champtionId;
	std::map<UINT32, SkyCraftEliRound> m_rounds;

	std::set<UINT64> m_watchRoleIds;			// 停留在对阵图的玩家
};

#endif