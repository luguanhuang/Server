#ifndef __SCENEHORSERACE__
#define __SCENEHORSERACE__

#include "utility/state.h"
#include "scenevsbase.h"
#include "vshandler.h"
#include "pb/project.pb.h"
#include "math/XVector3.h"

class Enemy;
class Scene;
class Role;
enum eGameState
{
	eReady  = 0,	//准备
	eRace	= 1,    //比赛 
	eLeave	= 2,	//离开
	eFinal	= 3,	//到达终点
};

struct RoleRaceInfo
{
	RoleRaceInfo(UINT32 FinalWallIndex, UINT32 SprintWallIndex)
        :nRoleID(0),pos(0.0,0.0,0.0),nLaps(-1),nTime(0),szName(""),
        nJob(0),nHorse(0),nState(0),fAngle(0.0),nLastLaps(0),
        nLastRank(0),fDistance(0.0),bSend(true),nRank(0),nLv(0),nExp(0),
        nFinalWallIndex(FinalWallIndex), nSprintWallIndex(SprintWallIndex)
    {}
	UINT64 nRoleID;
	Vector3 pos;
	int nLaps;
	UINT64 nTime;
	std::string szName;
	UINT32 nJob;
	UINT32 nHorse;
	UINT32 nState;
	float fAngle;
	int nLastLaps;
	UINT32 nLastRank;
	float fDistance;
	bool bSend;
	UINT32 nRank;
	UINT32 nLv;
	UINT32 nExp;

    UINT32 nFinalWallIndex;
    UINT32 nSprintWallIndex;
};

struct RoleRaceCmp
{
	bool operator()(const RoleRaceInfo &p1, const RoleRaceInfo &p2)
	{
        if(p1.nLaps != p2.nLaps)
        {
            return p1.nLaps > p2.nLaps;
        }
		if (p1.nLaps == p1.nFinalWallIndex)
		{
            if(p1.nTime != p2.nTime)
            {
                return p1.nTime < p2.nTime;
            }

            if(p1.nRank != p2.nRank)
            {
                return p1.nRank > p2.nRank;
            }

            return p1.nRoleID > p2.nRoleID;
		}
        else if(p1.nLaps == p1.nSprintWallIndex)
		{
            return p1.fDistance < p2.fDistance;
		}
        else
		{
            return p1.fAngle >  p2.fAngle;
		}
	}
};

struct RoleRaceFind
{
public:
	RoleRaceFind(const UINT64 nRoleID):m_nRoleID(nRoleID){}
	bool operator()(RoleRaceInfo &info)
	{
		return info.nRoleID == m_nRoleID;
	}
private:
	UINT64 m_nRoleID;
};

class SceneHorseRace : public SceneVsBase
{
public:
	SceneHorseRace(Scene* pScene);
	~SceneHorseRace(){}
	virtual bool Init(const KKSG::SceneSwitchData& roData);
	virtual void InitState();
	virtual bool OnEvent(const VsEvent& event);
	VsStateType CheckAfterFightEnd();
	virtual void Update();
	void SyncRaceTime();
	void SetHorseLaps(UINT64 nRole,UINT32 nLaps);
	void SetHorseAngle(UINT64 nRole,UINT32 nAngle);
	void Check();
	void RaceEnd();
	UINT32 Riding(Role *pRole);
	void GetRank(std::vector<UINT64> &roles);
	void SendRank();
	virtual void PreDoEnterScene(Role* pRole);
	virtual VsStateType IsAllLoad();
	virtual void OnEnterP321();
	virtual void OnEnterFight();
	virtual VsStateType CheckFightEnd();
	virtual void OnEnterAfterFight();
	virtual UINT32 GetGroup(UINT64 roleid);
	void FillAllInfo(Role *pRole,KKSG::HorseReConnectRes &res);
	void OnStateEnd();
	virtual UINT32 GetUpInterval() { return 0; }
	void GetRoleRank(Role *pRole,UINT32 &nTime, UINT32 &nRank,UINT32 &nHorse);

    virtual void WKRoleEnter(Role* pRole){}
    virtual void WKRankChangeOrFinal(UINT32 rankChangeOrFinal){}

    UINT32 GetClientShowLaps(int laps);

	void SortRoleRace()
	{
		std::sort(m_vecRoleRaceInfo.begin(), m_vecRoleRaceInfo.end(),RoleRaceCmp());
	}
	std::vector<RoleRaceInfo>& GetAllRaceInfo() { return m_vecRoleRaceInfo;}
protected:
	VsRoleDataMgr m_datamgr;
	VsLoadMgr m_loadmgr;
	std::vector<UINT64> m_unitids;//为了遍历方便
	std::vector<RoleRaceInfo> m_vecRoleRaceInfo;
	bool m_bFirst;
	bool m_bReFirst;
	UINT32 m_nTime;
	UINT64 m_nStartTime;
	UINT32 m_nEndTime;
	UINT32 m_nRefreshTime;
	UINT32 m_nRank;
	UINT64 m_nMirSec;

    UINT32 m_FinalWallIndex;
    UINT32 m_SprintWallIndex;

};

#endif
