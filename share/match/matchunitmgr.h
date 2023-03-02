#ifndef __MATCHUNITMGR_H__
#define __MATCHUNITMGR_H__

#include "matchbase.h"

class MatchUnit;

//天梯2v2
class MatchUnitMgrPkTwo : public MatchUnitMgrBase
{
public:
	MatchUnitMgrPkTwo(int type, MatchMgrBase* pMgr)
		:MatchUnitMgrBase(type, pMgr)
	{
		m_doMatchCount = 0;	
	}

	virtual void AddMatchUnit(MatchUnit* pMatchUnit);
	virtual void DelMatchUnit(MatchUnit* pMatchUnit);
	virtual void Update();

	void AddMatchID(KMatchID matchID);
	void DelMatchID(KMatchID matchID);

	void DoMatch(std::vector<MatchUnit*>& matchUnits);

private:
	inline UINT32 MatchRobotNeedCount() { return 2; }
	inline UINT32 MatchRoleNeedCount() { return 4; }
	std::unordered_set<KMatchID> m_matchIDs;
	UINT32 m_doMatchCount;
};

//全民天梯2v2
class MatchUnitMgrCustomPkTwo : public MatchUnitMgrBase
{
public:
	MatchUnitMgrCustomPkTwo(int type, MatchMgrBase* pMgr)
		:MatchUnitMgrBase(type, pMgr)
	{
		m_doMatchCount = 0;	
	}

	virtual void AddMatchUnit(MatchUnit* pMatchUnit);
	virtual void DelMatchUnit(MatchUnit* pMatchUnit);
	virtual void Update();

	void DoMatch(UINT64 uid, std::vector<MatchUnit*>& matchUnits);

private:
	inline UINT32 MatchRobotNeedCount() { return 2; }
	inline UINT32 MatchRoleNeedCount() { return 4; }
	
	std::unordered_map<UINT64, std::unordered_set<KMatchID>> m_matchIDs;
	UINT32 m_doMatchCount;
};

class MatchUnitMgrWeekendAct : public MatchUnitMgrBase
{
public:
	MatchUnitMgrWeekendAct(int type, MatchMgrBase* pMgr)
		:MatchUnitMgrBase(type, pMgr)
	{
		m_fullCount = 4;
	}

	virtual void AddMatchUnit(MatchUnit* pMatchUnit);
	virtual void DelMatchUnit(MatchUnit* pMatchUnit);
	virtual void Update();

	//for test
	void SetFullCount(UINT32 count) { m_fullCount = count; }

private:
	std::set<Count2MatchID> m_units;
	UINT32 m_fullCount;
};

class MatchUnitMgrSurvive : public MatchUnitMgrBase
{
public:
	MatchUnitMgrSurvive(int type, MatchMgrBase* pMgr)
		:MatchUnitMgrBase(type, pMgr)
	{
	}

	virtual void AddMatchUnit(MatchUnit* pMatchUnit);
	virtual void DelMatchUnit(MatchUnit* pMatchUnit);
	virtual void Update();

private:
	std::unordered_set<KMatchID> m_units;
};

#endif
