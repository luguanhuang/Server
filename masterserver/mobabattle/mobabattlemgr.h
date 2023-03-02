#ifndef __MOBABATTLEMGR_H__
#define __MOBABATTLEMGR_H__

#include "util/utilsingleton.h"
#include "match/matchmachine.h"
#include "timer.h"
#include "table/HeroBattleMatch.h"
#include "table/MobaWeekReward.h"

typedef MobaWeekReward::RowData MobaWeekRewardConf;

class MobaMatchUnit
{
public:
	MobaMatchUnit() {};
	MobaMatchUnit(UINT64 roleid, int teamid);	

	bool operator == (const MobaMatchUnit& unit) const;

	bool IsRole() const { return m_roleid != 0; }
	bool IsTeam() const { return !IsRole(); }

	void GetRoles(std::vector<UINT64>& roles) const;
	UINT32 GetRoleCount() const;

	UINT64 GetRoleID() const { return m_roleid; }
	int GetTeamID() const { return m_teamid; }

private:
	UINT64 m_roleid;
	int m_teamid;

public:
	UINT64 matchid;
	time_t startMatchTime;			///> 开始匹配时间
	double eloPoint;				///> elo分数
	UINT32 belongPos;				///> elo分数所属表对应行数
	UINT32 matchExpand;				///> 匹配扩大级数	
	time_t expandLastTime;			///> 上次扩大级数时间
};


class MobaBattleMgr : public Singleton<MobaBattleMgr>, public ITimer
{	
public:
	MobaBattleMgr();
	~MobaBattleMgr();

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	MobaWeekRewardConf* GetMobaWeekRewardConf(UINT32 id);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	KKSG::ErrorCode AddMatchUnit(const MobaMatchUnit& unit);
	void RemoveMatchUnit(const MobaMatchUnit& unit);

	bool IsRoleIn(UINT64 roleid);
	bool IsTeamIn(int teamid);

	void CreateSceneSucceed(UINT32 groupid, UINT32 sceneid);
	void CreateSceneFailed(UINT32 groupid);

	UINT32 GetMobaBattleMatchUnitNum() { return m_allUnit.size(); }

	bool GMChangeNum(UINT32 num);	

private:
	inline UINT64 NewMatchID() { return m_matchID++; }
	inline UINT32 NewGroupID() { return m_groupID++; }

	void _DoMatch(MatchMachine& machine);

	void _InsertMatchUnit(UINT64 matchid, UINT32 belongPos, const MobaMatchUnit& unit);
	void _EraseMatchUnit(UINT64 matchid);

	void _DelMatchUnitInAllMachine(UINT64 matchid);

	UINT32 _ELOPointBelong(double point, UINT32 roleCount);

	bool _IsUnitIn(const MobaMatchUnit& unit, MobaMatchUnit** pUnit = NULL);
	bool _CanAddRoleIn(UINT64 roleid);

	void _CreateBattle(UINT32 groupid);

	template <typename T>
	void _FillRoleInfo(UINT64 roleid, UINT32 campid, T* info);	

	void _CheckExpandMatch();
	void _CheckDelayTimeout();

	void _MatchSucceed(const MobaMatchUnit& unit);

private:
	HTIMER m_handler;
	HeroBattleMatch m_oMatchTable;

	UINT32 m_groupID;
	UINT64 m_matchID;
	std::vector<MatchMachine> m_allMachine;
	std::vector<time_t> m_lastMatchTime;

	struct FightGroup
	{
		std::vector<MobaMatchUnit> fightTeam1;
		std::vector<MobaMatchUnit> fightTeam2;
		double eloPoint1;
		double eloPoint2;
	};
	std::unordered_map<UINT32, FightGroup> m_allGroup;		///> groupid -> FightGroup

	std::unordered_map<UINT64, MobaMatchUnit> m_allUnit;	///> matchid -> MobaMatchUnit

	std::unordered_map<UINT64, time_t> m_delayRole;			///> 在匹配成功创建场景的过程中role进入delay

	MobaWeekReward m_oWeekReward;
};



#endif