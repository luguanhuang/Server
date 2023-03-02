#ifndef __SCENEPKBASE_H__
#define __SCENEPKBASE_H__

#include "scenevsbase.h"

struct BPkRoleData
{
	BPkRoleData()
	{
		state = VS_ROLE_STATE_NONE;
		point = 0;
		honorpoint = 0;
		dragoncount = 0;
		result = KKSG::PkResult_Draw;
		isdone = false;

		lastRank = 0;
		curRank = 0;
		allRank = 0;
	}
	VsRoleState state;
	int point;//分数改变
	UINT32 honorpoint;
	UINT32 dragoncount;
	KKSG::PkResultType result;
	bool isdone;

	//积分排名
	UINT32 lastRank;
	UINT32 curRank;
	UINT32 allRank;
};

struct DPkRoleData : public BPkRoleData
{
	DPkRoleData()
	{
		roleID = 0;
		group = 0;
	}
	UINT64 roleID;
	UINT32 group;
};	

struct SPKOhter
{
	int pointspan;
	int prefession;
	std::string name;
	SPKOhter()
	{
		prefession = 0;
		pointspan = 0;
	}
};

struct SPkResult : public BPkRoleData
{
	int basepoint;
	SPKOhter other;
	std::map<UINT32, UINT32> items;

	SPkResult()
	{
		basepoint = 0;
	}
	
	void End(KKSG::PkResultType result);
};

class ScenePkBase: public SceneVsBase
{
public:
	ScenePkBase(Scene* scene);
	virtual ~ScenePkBase();

	virtual void InitState();

	virtual UINT32 GetLoadWaitTime();

	virtual void Update();

	void CheckSetEnd();

	virtual void FillUnitIDs(std::vector<UINT64>& unitIDs) = 0;
	virtual KKSG::PkNVNType GetPkNVNType() = 0;
	virtual BPkRoleData* GetRoleBaseData(UINT64 roleID) = 0;
	virtual void FillResult(KKSG::PkResult* result, Role* role);

	void BeginRankUpdateDelay();
	void EndRankUpdateDelay(const KKSG::QMHRPkRes& res);

protected:
	bool AddRobot(const KKSG::RoleSmallInfo& roleInfo, int lookupID, KKSG::RoleSmallInfo& robotInfo);
	UINT32 GetRobotPoint(UINT32 myPoint);
	void SendRankReq(Role* pRole, UINT32 sceneID);

protected:
	int m_rankUpdateDelay;
	UINT32 m_fightUpdateTime;
};

#endif