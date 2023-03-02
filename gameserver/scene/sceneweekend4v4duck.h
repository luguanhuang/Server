#ifndef __SCENEWEEKEND4V4DUCK_H__
#define __SCENEWEEKEND4V4DUCK_H__

#include "scene/scenevsbase.h"
#include "vshandler.h"
#include "sceneweekend4v4base.h"

class SceneWeekEnd4v4Duck : public SceneWeekEnd4v4Base
{
public:
    SceneWeekEnd4v4Duck(Scene* scene);
    ~SceneWeekEnd4v4Duck();

    virtual bool OnEventRoleQuit(Role* pRole);
    virtual bool OnEventRoleDie(Role* pRole);
    virtual void OnEnemyDead(Enemy* pEnemy);

    virtual void FillBattleData(KKSG::WeekEnd4v4BattleRoleData* pProtocData, UINT32 index, const SceneWeekEnd4v4Result& roledata);

    virtual void InitRole(Role* pRole);
    virtual void ReviveRole(UINT64 roleid);
};

#endif