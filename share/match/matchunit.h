#ifndef __MATCHUNIT_H__
#define __MATCHUNIT_H__

#include "matchbase.h"

class MatchUnitPkTwo : public MatchUnit
{
public:
	MatchUnitPkTwo(KMatchID matchID, int type, const KKSG::KMatchUnit& kunit)
		:MatchUnit(matchID, type, kunit)
	{
	}

	bool MatchCross(UINT32 now);
	bool MatchRobot(UINT32 now);
	bool MatchOther(UINT32 now, MatchUnitPkTwo* other);
	UINT32 GetPoint();

private:
	KKSG::PkMatchStage* GetStage(UINT32 now);
};

class PvpMatchUnit : public MatchUnit
{
public:
	PvpMatchUnit(KMatchID matchID, int type, const KKSG::KMatchUnit& kunit);
	~PvpMatchUnit();

	void SetMatched(){ m_matched = true; }
	bool IsMatched() { return m_matched; }	

private:
	bool m_matched;
};

class MatchUnitWeekendAct : public MatchUnit
{
public:
	MatchUnitWeekendAct(KMatchID matchID, int type, const KKSG::KMatchUnit& kunit)
		:MatchUnit(matchID, type, kunit)
	{

	}
};

class MatchUnitSurvive : public MatchUnit
{
public:
	MatchUnitSurvive(KMatchID matchID, int type, const KKSG::KMatchUnit& kunit)
		:MatchUnit(matchID, type, kunit)
	{

	}
};

#endif