#ifndef __PKMGR_H__
#define __PKMGR_H__

#include "table/PkPrepareTable.h"
#include "table/PkPointTable.h"
#include "table/PkRankTable.h"
#include "table/PkParamTable.h"
#include "util/XRandom.h"
#include "timer.h"
#include <time.h>
#include "mail/maildata.h"
#include "common/roleopdelay.h"
#include "common/roleoprecordmgr.h"

enum PkRemoveReason
{
	PK_RR_ERR,
	PK_RR_STOP,
	PK_RR_DOWN,
	PK_RR_TIMEOUT,
	PK_RR_INWORLD,
};

namespace KKSG
{
	class QMHRPkArg;
	class QMHRPkRes;
	class PkBattleInfo;
}

class PkRealTimeRankList;

class PkInfo
{
public:
	PkInfo()
	{
		m_inWorld = false;
	}
	inline KKSG::PkMatchInfo& MatchInfo() { return m_info; }

	inline UINT64 RoleID() const { return m_info.roleid(); }
	inline UINT32 Point() const { return m_info.rolerec().point(); }
	inline UINT32 TimeBegin() { return m_info.timebegin(); }
	inline UINT32 RoleLevel() const { return m_info.rolelevel(); }

	UINT32 GetFitSpan(const PkInfo& other);
	bool CheckTimeout();
	bool IsEmpty();

	bool IsRobot();
	int GetLookupID();

	void CheckToWorld();
	void CheckOutWorld();
	inline bool IsInWorld() const { return m_inWorld; }

private:
	const KKSG::PkMatchStage* GetBack();
	void PopBack();

	void FillToWorldData(KKSG::PkMatchToWorldPara& data);

public:

	KKSG::PkMatchInfo m_info;
	bool m_inWorld;
};

struct EqualPkInfo
{
	UINT64 roleid;
	EqualPkInfo(UINT64 temproleid)
	{
		roleid = temproleid;	
	}
	bool operator () (const PkInfo& info)
	{
		return info.RoleID() == roleid;
	}
};

class CRole;
class PkMgr : public ITimer
{
	PkMgr();
	~PkMgr();
	DECLARE_SINGLETON(PkMgr);
public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void Update();

	bool AddRole(CRole* role, bool addrobot, const KKSG::PkMatchInfo& pkmi);
	bool RemoveRole(UINT64 roleid, PkRemoveReason reason);
	bool InPrepare(UINT64 roleid);

	void Start(PkInfo& pkinfo11, PkInfo& pkinfo22, int robotlookupid);

	UINT32 GetRank(UINT64 roleid, KKSG::PkNVNType type = KKSG::PK_1v1);

	UINT32 RankRewardLeftTime();

	void HandlerRankUpdateDelay(const KKSG::QMHRPkArg& arg, KKSG::QMHRPkRes& res);
	bool InDelay(UINT64 roleID);

	void CreateSceneBegin(const KKSG::PkBattleInfo& pkinfo);
	void CreateSceneEnd(const KKSG::PkBattleInfo& pkinfo, UINT32 sceneID);
	void OnMatchInWorld(const KKSG::PkMatchToWorldPara& data);

	bool FillBattleRoleInfo(KKSG::PkRoleInfo& data, PkInfo& info);

	void GetMatchStage(UINT32 point, KKSG::PkMatchInfo& data, KKSG::PkNVNType type = KKSG::PK_1v1);
	void TimeOutNtf(CRole* pRole);

	void OnRoleLogin(CRole* pRole);

	RoleOpRecMgr& GetRoleOpRecMgr() { return m_opPairRec; }

	void TestResetLastWeek();

	void TestPrint();

	void PkRankWrite(PkRealTimeRankList* pRank, UINT32 now);

private:

	bool MatchRole(PkInfo& pkinfo11, PkInfo& pkinfo22);
	bool MatchRobot(PkInfo& info, int& robotlookupid);

	void CheckTimeout();

private:
	void GiveReward(std::unordered_map<UINT64,UINT32>& role2rank);

	UINT32 CalculateTime(UINT32 count);

	void CheckWeekRankRewrad();
	void GetReward(UINT32 rank, std::vector<ItemDesc>& items);

	void RemoveReason(UINT64 roleid, PkRemoveReason reason);

	void CheckMatchToWorld();

	void ReadData();
	void WriteData();

	int PkTypeToRankType(KKSG::PkNVNType type);

private:
	PkPrepareTable m_table;
	PkRankTable m_ranktable;
	std::vector<PkInfo> m_preparelist;
	std::deque<PkInfo> m_preparerobotlist;

	UINT32 m_lastWeekRankRewardT;

	RoleOpDelay m_opDelay;
	RoleOpRecMgr m_opPairRec;

	HTIMER m_handler;
};

#endif
