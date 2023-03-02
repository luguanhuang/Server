#ifndef __SCENEBMRFIGHT_H__
#define __SCENEBMRFIGHT_H__

#include "utility/state.h"
#include "scenevsbase.h"
#include "vshandler.h"

class Enemy;
class Scene;
class Role;

struct stBMRoleInfo
{
	UINT64 nRoleID;
	VsRoleState state;
	UINT32 nKiller;
	UINT32 nScore;
	UINT32 nDead;
	UINT32 nTime;
	std::string szName;
	UINT32 nReliveTime;
	UINT32 nRank;
	UINT32 nJoinTime;
	stBMRoleInfo()
	{
		state			= VS_ROLE_WAITLOAD;
		nKiller			= 0;
		nScore			= 0;
		nRoleID			= 0;
		nDead			= 0;
		nTime			= 0;
		szName			= "";
		nReliveTime		= 0;
		nRank			= 0;
		nJoinTime		= 0;
	}
};


struct BMRoleFind
{
public:
	BMRoleFind(const UINT64 nRoleID):m_nRoleID(nRoleID){}
	bool operator()(stBMRoleInfo &info)
	{
		return info.nRoleID == m_nRoleID;
	}
private:
	UINT64 m_nRoleID;
};



class SceneBMFight:public SceneVsBase
{
public:
	SceneBMFight(Scene* scene);
	~SceneBMFight();
	virtual bool Init(const KKSG::CreateBattleParam& roData);
	virtual void InitState();
	virtual UINT32 GetGroup(UINT64 roleid);
	void SyncHangTime();
	virtual void Update();
	void SyncTime();
	virtual bool OnEvent(const VsEvent& event);
	void ResetPosition(UINT64 roleid);
	virtual VsStateType IsAllLoad();
	void Set321State();
	virtual VsStateType CheckFightEnd();
	virtual	VsStateType CheckAfterFightEnd();

	virtual void OnRoleReconnect(Role* pRole);
	void FillBMRankList(const KKSG::BMRankListRes &ref);
	void ReqFinalRank();
	void ReportToWorld(UINT64 dwRole,UINT32 nScore,UINT32 nKiller,UINT32 nTime,UINT32 nDeadCount,UINT32 nJob,UINT32 nLv);
	virtual void OnStateEnd();
	virtual void OnEnemyDead(Enemy *pEnemy);
	virtual void OnEnterP321();
	stBMRoleInfo* GetBMFightRole(UINT64 roleid);
	UINT32 GetStateLeftTime();
	virtual void OnEnterAfterFight();
	void PickUpPoint(UINT64 dwRoleID,UINT32 job,UINT32 nLevel,UINT32 nPoint);
	void SetRoleRankPoint(UINT64 dwRoleID, const KKSG::BMSceneRankRes& oData);
	void BattleResult();
	void DoTxLog(Role *pRole, const stBMRoleInfo& oData);
	void SetResult();
private:

	bool OnRoleEnter(Role *pRole);
	void _CheckRoleRevive();

	bool OnRoleLeave(UINT64 roleid);
	bool OnRoleDie(UINT64 roleid);


	std::vector<stBMRoleInfo> m_units;
	VsLoadMgr m_loadmgr;
	UINT32 m_nGames;
	UINT32 m_nGroupSvr;
	UINT32 m_nBattleIdx;
	UINT32 m_nLastSynTime;
	UINT32 m_nFightTime;
	UINT32 m_nTime;
	UINT32 m_nEndTime;
	UINT32 m_nLastReqRankTime;
	bool m_bLoaded;
	bool m_bResult;
};



#endif
