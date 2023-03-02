#ifndef __SCENEBATTLEFIGHT_H__
#define __SCENEBATTLEFIGHT_H__

#include "utility/state.h"
#include "scenevsbase.h"
#include "vshandler.h"
#include "define/itemdef.h"

struct stBFRoleInfo
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
	double hurt;
	UINT32 job;
	UINT32 nKillStreak;
	std::string svrname;
	UINT32 time;
	std::vector<ItemDesc> itemList;
	stBFRoleInfo()
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
		hurt            = 0.0;
		job             = 0;
		nKillStreak      = 0;
		svrname         = "";
		time            = 0;
	}

	void AddItem(std::vector<ItemDesc> &refItems)
	{
		std::copy(refItems.begin(),refItems.end(),std::back_inserter(itemList));
	}

};


struct BFRoleFind
{
public:
	BFRoleFind(const UINT64 nRoleID):m_nRoleID(nRoleID){}
	bool operator()(stBFRoleInfo &info)
	{
		return info.nRoleID == m_nRoleID;
	}
private:
	UINT64 m_nRoleID;
};


struct BFRoleInfoCmp
{
	bool operator()(const stBFRoleInfo &p1, const stBFRoleInfo &p2)
	{
		if(p1.nScore != p2.nScore)
		{
			return p1.nScore > p2.nScore;
		}
		if (p1.nKiller != p1.nKiller)
		{
			return p1.nKiller > p2.nKiller;
		}
		else if(p1.time == p1.time)
		{
			return p1.time < p2.time;
		}
		else
		{
			return p1.nRoleID >  p2.nRoleID;
		}
	}
};


class SceneBFFight:public SceneVsBase
{
public:
	SceneBFFight(Scene* scene);
	~SceneBFFight();
	virtual bool Init(const KKSG::CreateBattleParam& roData);
	virtual void InitState();
	VsStateType IsAllLoad();
	virtual UINT32 GetGroup(UINT64 roleid);
	virtual bool OnEvent(const VsEvent& event);
	virtual void Update();
	virtual VsStateType CheckFightEnd();
	virtual	VsStateType CheckAfterFightEnd();
	virtual void OnRoleReconnect(Role* pRole);
	virtual void OnEnemyDead(Enemy *pEnemy);
	virtual void OnHurt(Unit* pTargetUnit, Unit* pCastUnit, double value);
	void AddBuff(Enemy* pEnemy);
	virtual void OnEnterP321();
	void BattleEnd();
	void AddPoint(UINT64 nRoleID,UINT32 nPoint);
	void _CheckRoleRevive();
	bool OnRoleEnter(Role *pRole);
	void Set321State();
	stBFRoleInfo* GetBFFightRole(UINT64 roleid);
	bool OnRoleLeave(UINT64 roleid);
	bool OnRoleDie(UINT64 roleid);
	void GetAginstInfo(KKSG::BattleFieldRoleAgainst &refInfo);
	void GetRank(KKSG::BattleFieldRankRes &refInfo);
	void SyncTime();
	void SyncHangTime();
	void FillBattleResult(UINT64 nRoleID,KKSG::BattleFieldBattleResult &refData);
	void SortRoleBattle();
	void AddItem(Role *pRole,std::vector<ItemDesc> &refItems);
	void GetRoleList(std::vector<stBFRoleInfo> &refList);
	void GetItem(Role *pRole, std::vector<ItemDesc> &refItems);
	bool GetFinal();
private:
	std::vector<stBFRoleInfo> m_units;
	VsLoadMgr m_loadmgr;
	UINT32 m_nTime;
	UINT32 m_nEndTime;
	bool m_bLoaded;
	UINT32 m_nLastSynTime;
	UINT32 m_nLvIdx;
	UINT32 m_bFinal;
};


#endif
