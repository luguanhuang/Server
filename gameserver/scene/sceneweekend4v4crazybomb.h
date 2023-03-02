#ifndef __SCENEWEEKEND4V4CRAZYBOMB_H__
#define __SCENEWEEKEND4V4CRAZYBOMB_H__

#include "scene/scenevsbase.h"
#include "vshandler.h"
#include "sceneweekend4v4base.h"

class Scene;
class SceneWeekEnd4v4CrazyBomb : public SceneWeekEnd4v4Base
{
public:
    SceneWeekEnd4v4CrazyBomb(Scene* scene);
    ~SceneWeekEnd4v4CrazyBomb();

    virtual bool OnEventRoleQuit(Role* pRole);
    virtual bool OnEventRoleDie(Role* pRole);
    virtual void GetTeamScore(UINT32& redScore, UINT32& blueScore);

    virtual void FillBattleData(KKSG::WeekEnd4v4BattleRoleData* pProtocData, UINT32 index, const SceneWeekEnd4v4Result& roledata);

    virtual void InitRole(Role* pRole);
    virtual void ReviveRole(UINT64 roleid);
};

#endif