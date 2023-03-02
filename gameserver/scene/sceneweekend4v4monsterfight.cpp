#include "pch.h"
#include "sceneweekend4v4monsterfight.h"
#include "weekend4v4/ptcg2c_weekend4v4roledatantf.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "timeutil.h"
#include "scenevsbase.h"
#include "scenebase.h"
#include "scene.h"
#include "util/XRandom.h"
#include "util/gametime.h"
#include "gamelogic/weekend4v4mgr.h"
#include "gamelogic/weekend4v4record.h"

SceneWeekEnd4v4MonsterFight::SceneWeekEnd4v4MonsterFight(Scene* scene)
    :SceneWeekEnd4v4Base(scene, WEEKEND4V4_TYPEID_MONSTERFIGHT)
{
}

SceneWeekEnd4v4MonsterFight::~SceneWeekEnd4v4MonsterFight()
{
}

bool SceneWeekEnd4v4MonsterFight::OnEventRoleQuit(Role* pRole)
{
    return true;
}

bool SceneWeekEnd4v4MonsterFight::OnEventRoleDie(Role* pRole)
{
    PushReviveList(pRole->GetRoleID());
    UINT64 killerID = pRole->GetKillerID();
    AddScore(killerID);
    WKBoardcastRoleData(killerID, false);
    return true;
}

void SceneWeekEnd4v4MonsterFight::FillBattleData(KKSG::WeekEnd4v4BattleRoleData* pProtocData, UINT32 index, const SceneWeekEnd4v4Result& roledata)
{
    pProtocData->set_score(GetScore(roledata.GetCount()));
    pProtocData->set_killcount(roledata.GetCount());
}

UINT32 SceneWeekEnd4v4MonsterFight::GetBuffIDChangeMonster()
{
    return 70320 + (UINT32)(XRandom::randInt(0, 20));
}

void SceneWeekEnd4v4MonsterFight::InitRole(Role* pRole)
{
    if(NULL == pRole)
    {
        return;
    }
    AddBuffChangeUnbeatable(pRole, GetBuffIDChangeMonster());
}

void SceneWeekEnd4v4MonsterFight::ReviveRole(UINT64 roleid)
{
    Role* pRole = GetRole(roleid);
    if(NULL == pRole || !pRole->IsDead())
    {
        return;
    }
    TranInitPosition(roleid);
    pRole->Revive();
    AddBuffChangeUnbeatable(pRole, GetBuffIDChangeMonster());
}
