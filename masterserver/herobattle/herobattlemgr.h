#ifndef __HEROBATTLEMGR_H__
#define __HEROBATTLEMGR_H__

#include "util/utilsingleton.h"
#include "match/matchmachine.h"
#include "timer.h"
#include "table/HeroBattleMatch.h"
#include "table/HeroBattleMapCenter.h"

class HeroMatchUnit
{
public:
	HeroMatchUnit() {};
	HeroMatchUnit(UINT64 roleid, int teamid);	

	bool operator == (const HeroMatchUnit& unit) const;

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
	bool isMatchInWorld;			///> 是否在world匹配
	time_t startMatchTime;		///> 开始匹配时间
	double eloPoint;				///> elo分数
	UINT32 belongPos;				///> elo分数所属表对应行数
	UINT32 matchExpand;			///> 匹配扩大级数	
	time_t expandLastTime;		///> 上次扩大级数时间
};


class HeroBattleMgr : public Singleton<HeroBattleMgr>, public ITimer
{	
public:
	HeroBattleMgr();
	~HeroBattleMgr();

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	KKSG::ErrorCode AddMatchUnit(const HeroMatchUnit& unit);
	void RemoveMatchUnit(const HeroMatchUnit& unit);

	bool IsRoleIn(UINT64 roleid);
	bool IsTeamIn(int teamid);

	void CreateSceneSucceed(UINT32 groupid, UINT32 sceneid);
	void CreateSceneFailed(UINT32 groupid);

	void MatchSucceedFromWorld(const KKSG::HeroWorldRoomData& data);

	void OnCrossZoneUnusable();

	UINT32 GetHeroBattleMatchUnitNum() { return m_allUnit.size(); }

	bool GMChangeNum(UINT32 num);	
	bool GMSetNextSceneID(UINT32 sceneid);

private:
	inline UINT64 NewMatchID() { return m_matchID++; }
	inline UINT32 NewGroupID() { return m_groupID++; }

	void _DoMatch(MatchMachine& machine);

	void _InsertMatchUnit(UINT64 matchid, UINT32 belongPos, const HeroMatchUnit& unit);
	void _EraseMatchUnit(UINT64 matchid);

	void _DelMatchUnitInAllMachine(UINT64 matchid);

	UINT32 _ELOPointBelong(double point, UINT32 roleCount);

	bool _IsUnitIn(const HeroMatchUnit& unit, HeroMatchUnit** pUnit = NULL);
	bool _CanAddRoleIn(UINT64 roleid);

	void _CreateBattle(UINT32 groupid);

	template <typename T>
	void _FillRoleInfo(UINT64 roleid, UINT32 campid, T* info);	

	void _CheckExpandMatch();
	void _CheckMatchToWorld();
	void _CheckDelayTimeout();

	void _UnitMatchToWorld(const HeroMatchUnit& unit);
	void _UnitMatchOutWorld(const HeroMatchUnit& unit);

	void _MatchSucceed(const HeroMatchUnit& unit);

private:
	HTIMER m_handler;
	HeroBattleMatch m_oMatchTable;
	HeroBattleMapCenter m_oMapCenter;

	UINT32 m_groupID;
	UINT64 m_matchID;
	std::vector<MatchMachine> m_allMachine;
	std::vector<time_t> m_lastMatchTime;

	struct FightGroup
	{
		std::vector<HeroMatchUnit> fightTeam1;
		std::vector<HeroMatchUnit> fightTeam2;
		double eloPoint1;
		double eloPoint2;
	};
	std::unordered_map<UINT32, FightGroup> m_allGroup;		///> groupid -> FightGroup

	std::unordered_map<UINT64, HeroMatchUnit> m_allUnit;	///> matchid -> HeroMatchUnit

	std::unordered_map<UINT64, time_t> m_delayRole;			///> 在匹配成功创建场景的过程中role进入delay

	UINT32 m_GMSceneID;
};



#endif