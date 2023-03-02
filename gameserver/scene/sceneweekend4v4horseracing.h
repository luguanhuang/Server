#ifndef __SCENEWEEKEND4V4HORSERACING_H__
#define __SCENEWEEKEND4V4HORSERACING_H__

#include "scene/scenevsbase.h"
#include "vshandler.h"
#include "scenehorserace.h"
#include "battleresultlistener.h"
#include "gamelogic/weekend4v4mgr.h"

class SceneWeekEnd4v4HorseRacing : public SceneHorseRace
{
public:
    SceneWeekEnd4v4HorseRacing(Scene* scene);
    ~SceneWeekEnd4v4HorseRacing();

    virtual bool Init(const KKSG::CreateBattleParam& roParam);
	UINT64 GetVoipRoomID(UINT64 nRoleID);
	virtual UINT32 GetGroup(UINT64 roleid);

    void AddItemList(KKSG::WeekEnd4v4TeamResult* teamResult, std::vector<Role*>& roRoles, UINT32 redScore, UINT32 blueScore);
    bool IsWin(Role* pRole, UINT32 redScore, UINT32 blueScore);
private:
    UINT32 GetRoleIndex(UINT64 roleid);
    UINT32 m_wkRoleCount;
    time_t m_wkStartTime;
    time_t m_wkUpdateTime;
    WEEKEND4V4_TYPEID m_typeid;
    void SetRoleName(UINT64 roleid, const std::string& rolename);
public:
    void GetTeamScore(UINT32& redScore, UINT32& blueScore);
    UINT32 GetTeamSeconds();
    void FillResult(NormalBattleResultListener& listener);

    virtual void WKRoleEnter(Role* pRole);
    virtual void WKRankChangeOrFinal(UINT32 rankChangeOrFinal);

    virtual void OnRoleReconnect(Role* pRole);
    void FillBattleRoleData(KKSG::WeekEnd4v4BattleRoleData* pProtocData, UINT64 roleid, bool isAllInfo);
    void FillBattleAllRolesData(KKSG::WeekEnd4v4BattleAllRoleData& pProtocData, bool isAllInfo);
    void WKBoardcastRoleData(UINT64 roleid, bool isAllInfo);    // 同步某人的信息给所有人
    void WKSynAllRolesData(Role* pRole, bool isAllInfo);        // 同步所有人的信息给自己
    void WKBoardcastAllRolesData(bool isAllInfo);               // 同步所有人的信息给所有人
};

#endif
