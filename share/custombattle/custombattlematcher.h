#ifndef __CUSTOMBATTLEMATCHERMGR_H__
#define __CUSTOMBATTLEMATCHERMGR_H__

#include "match/matchmachine.h"
#include "custombattle/custombattledef.h"
#include "custombattle/custombattlematcherpk.h"


struct CustomBattleMatchExpand
{
	time_t timestamp;
	UINT32 matchid;
	std::set<UINT32> matchids;

	CustomBattleMatchExpand()
	{
		matchid = 0;
		timestamp = 0;
	}
};

class CustomBattleMatcherMgr : public ITimer
{
public:
	CustomBattleMatcherMgr();
	~CustomBattleMatcherMgr();
public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	virtual void DoBattlePair(CustomBattleMatchSceneInfo& info, const std::vector<UINT64>& matchone, const std::vector<UINT64>& matchtwo) = 0;

	KKSG::ErrorCode AddSystemMatcher(KKSG::CustomBattleMatchInfo& info);
	KKSG::ErrorCode DelSystemMatcher(UINT64 index);

	KKSG::ErrorCode AddCustomMatcher(KKSG::CustomBattleMatchInfo& info, bool isofficial);
	KKSG::ErrorCode DelCustomMatcher(UINT64 index);

	KKSG::CustomBattleMatchInfo* GetMatchInfo(UINT64 index);

	void TryDel(UINT64 index);

protected:
	bool DoMatch(UINT32 matchid, CustomBattleMatchSceneInfo& info, MatchMachine& machine, UINT32& limit);
	void ExpanMatch();

	void RollBack(UINT32 matchid, UINT64 index);
	void DelOtherSystemMatch(UINT64 index, UINT32 matchid, UINT64 battleid);

protected:
	HTIMER m_timer;

	std::map<UINT64, KKSG::CustomBattleMatchInfo> m_index2info;

	///> 赏金比赛匹配
	std::map<UINT64, CustomBattleMatchExpand> m_index2matchids;
	std::map<UINT64, std::map<UINT32, MatchMachine>> m_systemmatchers;	
	///> 自定义PK比赛匹配
	std::map<UINT64, CustomBattleMatcherPk> m_pkcustommatchers;	

	///> 自定义PKTwo比赛匹配
	///> 走统一匹配流程
};

#endif
