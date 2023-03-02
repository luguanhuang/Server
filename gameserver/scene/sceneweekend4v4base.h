#ifndef __SCENEWEEKEND4V4BASE_H__
#define __SCENEWEEKEND4V4BASE_H__

#include "scene/scenevsbase.h"
#include "vshandler.h"
#include "gamelogic/weekend4v4mgr.h"
#include <cmath>

struct SceneWeekEnd4v4Result
{
    UINT64 m_roleid;
    std::string m_rolename;
    UINT32 m_level;
    UINT32 m_profession;
    SceneWeekEnd4v4Result(UINT64 roleid)
        : m_roleid(roleid), m_rolename(""), 
        m_level(0), m_profession(0),
        m_count(0){}
    UINT32 GetCount() const
    {
        return m_count;
    }
    void IncCount(UINT32 addscore)
    {
        m_count += addscore;
    }
private:
    UINT32 m_count;
};

struct SceneWeekEnd4v4ReviveInfo
{
    SceneWeekEnd4v4ReviveInfo(UINT64 roleid, time_t reviveTime)
        : m_roleid(roleid), m_reviveTime(reviveTime)
    {}
    UINT64 m_roleid;
    time_t m_reviveTime;
};

class Scene;
class SceneWeekEnd4v4Base : public SceneVsBase
{
public:
    SceneWeekEnd4v4Base(Scene* scene, WEEKEND4V4_TYPEID wtypeid);
    ~SceneWeekEnd4v4Base();

    virtual bool Init(const KKSG::CreateBattleParam& roParam);
	UINT64 GetVoipRoomID(UINT64 nRoleID);
	virtual void InitState();
    virtual UINT32 GetGroup(UINT64 roleid);
    virtual VsStateType IsAllLoad();
    virtual VsStateType OnLoadTimeOut();
    virtual VsStateType CheckFightEnd();
    virtual void OnStateEnd();
    virtual void OnRoleReconnect(Role* pRole);
    virtual bool OnEvent(const VsEvent& event);
    virtual void Update();

    void FillRoleResultData(KKSG::WeekEnd4v4BattleRoleData* pProtocData, UINT64 roleid);
    virtual void GetTeamScore(UINT32& redScore, UINT32& blueScore);
    UINT32 GetTeamSeconds();

    void AddItemList(KKSG::WeekEnd4v4TeamResult* teamResult, std::vector<Role*>& roRoles, UINT32 redScore, UINT32 blueScore);
    bool IsWin(Role* pRole, UINT32 redScore, UINT32 blueScore);

    void PushReviveList(UINT64 roleid);
    void AddBuffUnbeatable(Role* pRole);        // 无敌buff
    void AddBuffChangeUnbeatable(Role* pRole, UINT32 buffID);  // 变身+无敌buff

    virtual bool ResultCountOrSeconds();
    virtual bool OnEventRoleQuit(Role* pRole) = 0;
    virtual bool OnEventRoleDie(Role* pRole) = 0;
protected:
    UINT32 GetRoleIndex(UINT64 roleid);
    void SetRoleBaseInfo(Role* pRole);

    virtual void FillBattleData(KKSG::WeekEnd4v4BattleRoleData* pProtocData, UINT32 index, const SceneWeekEnd4v4Result& roledata) = 0;
    void FillBattleRoleData(KKSG::WeekEnd4v4BattleRoleData* pProtocData, UINT32 index, bool isAllInfo);
    void FillBattleAllRolesData(KKSG::WeekEnd4v4BattleAllRoleData& pProtocData, bool isAllInfo);
    void WKBoardcastRoleData(UINT64 roleid, bool isAllInfo);    // 同步某人的信息给所有人
    void WKSynAllRolesData(Role* pRole, bool isAllInfo);        // 同步所有人的信息给自己
    void WKBoardcastAllRolesData(bool isAllInfo);             // 同步所有人的信息给所有人

    void ReviviRoleList();
    virtual void InitRole(Role* pRole) = 0;
    virtual void ReviveRole(UINT64 roleid) = 0;

    virtual void AddScore(UINT64 roleid, UINT32 addscore = 1);
    virtual UINT32 GetScore(UINT32 count);

    UINT32 GetMonsterScore(UINT32 monsterid);
public:
    std::vector<SceneWeekEnd4v4Result> m_roleResult;
protected:
    WEEKEND4V4_TYPEID m_typeid;
    VsLoadMgr m_loadmgr;
    UINT32 m_roleCount;
    time_t m_startTime;
    UINT32 m_totalSeconds;
    std::list<SceneWeekEnd4v4ReviveInfo> m_deathList;
    UINT32 m_ratio;
    std::map<UINT32, UINT32> m_monsterScore;
};

#endif