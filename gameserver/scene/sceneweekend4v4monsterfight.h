#ifndef __SCENEWEEKEND4V4MONSTERFIGHT_H__
#define __SCENEWEEKEND4V4MONSTERFIGHT_H__

#include "scene/scenevsbase.h"
#include "vshandler.h"
#include "sceneweekend4v4base.h"

class SceneWeekEnd4v4MonsterFight : public SceneWeekEnd4v4Base
{
public:
    SceneWeekEnd4v4MonsterFight(Scene* scene);
    ~SceneWeekEnd4v4MonsterFight();

    virtual bool OnEventRoleQuit(Role* pRole);
    virtual bool OnEventRoleDie(Role* pRole);

    virtual void FillBattleData(KKSG::WeekEnd4v4BattleRoleData* pProtocData, UINT32 index, const SceneWeekEnd4v4Result& roledata);

    UINT32 GetBuffIDChangeMonster();
    virtual void InitRole(Role* pRole);
    virtual void ReviveRole(UINT64 roleid);
};

#endif
