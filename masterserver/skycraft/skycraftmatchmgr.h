#ifndef _SKY_CRAFT_MATCH_MGR_H_
#define _SKY_CRAFT_MATCH_MGR_H_


#include "util/utilsingleton.h"
#include "timer.h"
#include "scene/scenecreate.h"

struct SkyMatchUnit
{
	SkyMatchUnit(): stId(0), teamId(0), score(0), startTime(0) {}
	UINT64 stId;
	UINT32 teamId;
	UINT32 score;
	UINT32 startTime;
};

class Team;
class SkyCraftMatchMgr : public ICreateBattleListener, public ITimer, public Singleton<SkyCraftMatchMgr>
{
public:
	bool Init();
	void Uninit();

	int StartMatch(UINT64 stId, Team* team);
	int CancelMatch(Team* team);
	void ForceCancelMatch(UINT64 stId);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);

	void NotifyStartMatch(Team* team);
	void NotifyStopMatch(Team* team);
	void NotifyMatchTimeout(Team* team);

private:
	void OnMatchSuccess(SkyMatchUnit& a, SkyMatchUnit& b);
	void OnMatchTimeOut(SkyMatchUnit& unit);

	int Start(SkyMatchUnit& unit);
	void Cancel(UINT64 stId);
	bool IsInMatch(UINT64 stId);

	SkyMatchUnit* GetMatchUnit(UINT64 stId);

	void GetAllMatchSkyTeamIds(std::vector<UINT64>& stIds);

	void DoMatch();
	void DoTimeOut();
	void DoCheckDelay();
	bool IsFit(UINT32 score1, UINT32 time1, UINT32 score2, UINT32 time2);

	UINT32 NewUid();
	UINT32 GetMatchNum() { return m_allUnits.size(); }

	void AddToDelay(UINT64 stId);
	void DeleteFromDelay(UINT64 stId);
	bool IsInDelay(UINT64 stId);

	struct FightGroup
	{
		SkyMatchUnit teamA;
		SkyMatchUnit teamB;
	};

private:
	UINT32 m_uid;
	std::unordered_map<UINT64, SkyMatchUnit> m_allUnits;		// 正在匹配的队伍
	std::map<std::pair<UINT32, UINT64>, UINT32>	m_sortuUnits;	// 排好序的队伍 pair(分数，id) value: 开始匹配时间

	std::map<UINT32, FightGroup> m_matchedGroup;				// 匹配好的队伍
	std::unordered_map<UINT64, UINT32> m_delayUnits;			// 匹配成功之后延迟一段时间运行下次匹配
};

#endif