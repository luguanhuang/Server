#include "pch.h"
#include "sceneweekend4v4ghostaction.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "timeutil.h"
#include "scenevsbase.h"
#include "scenebase.h"
#include "scene.h"
#include "util/XRandom.h"
#include "util/gametime.h"
#include "weekend4v4/ptcg2c_weekend4v4roledatantf.h"
#include "gamelogic/weekend4v4mgr.h"
#include "gamelogic/weekend4v4record.h"
#include "unit/enemy.h"

SceneWeekEnd4v4GhostAction::SceneWeekEnd4v4GhostAction(Scene* scene)
    :SceneWeekEnd4v4Base(scene, WEEKEND4V4_TYPEID_GHOSTACTION)
{
}

SceneWeekEnd4v4GhostAction::~SceneWeekEnd4v4GhostAction()
{
}

bool SceneWeekEnd4v4GhostAction::OnEventRoleQuit(Role* pRole)
{
    return true;
}

bool SceneWeekEnd4v4GhostAction::OnEventRoleDie(Role* pRole)
{
    PushReviveList(pRole->GetRoleID());
    return true;
}

void SceneWeekEnd4v4GhostAction::OnEnemyDead(Enemy* pEnemy)
{
    UINT32 monstersocre = GetMonsterScore(pEnemy->GetTemplateID());
    if(0 == monstersocre)
    {
        LogError("monster score templateid %d = 0\n", pEnemy->GetTemplateID());
        return;
    }
    UINT64 killerID = pEnemy->GetKillerID();
    AddScore(killerID, monstersocre);
    WKBoardcastRoleData(killerID, false);
    LogDebug("SceneWeekEnd4v4GhostAction OnEnemyDead %llu.\n", killerID);
}

void SceneWeekEnd4v4GhostAction::FillBattleData(KKSG::WeekEnd4v4BattleRoleData* pProtocData, UINT32 index, const SceneWeekEnd4v4Result& roledata)
{
    pProtocData->set_score(GetScore(roledata.GetCount()));
    pProtocData->set_killcount(roledata.GetCount());
    LogDebug("SceneWeekEnd4v4GhostAction: roleid = %llu, score = %d\n", roledata.m_roleid, GetScore(roledata.GetCount()));
}

void SceneWeekEnd4v4GhostAction::InitRole(Role* pRole)
{
    if(NULL == pRole)
    {
        return;
    }
    AddBuffUnbeatable(pRole);
}

void SceneWeekEnd4v4GhostAction::ReviveRole(UINT64 roleid)
{
    Role* pRole = GetRole(roleid);
    if(NULL == pRole || !pRole->IsDead())
    {
        return;
    }
    TranInitPosition(roleid);
    pRole->Revive();
    AddBuffUnbeatable(pRole);
}
