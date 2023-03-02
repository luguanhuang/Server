#include "pch.h"
#include "sceneweekend4v4crazybomb.h"
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

SceneWeekEnd4v4CrazyBomb::SceneWeekEnd4v4CrazyBomb(Scene* scene)
    :SceneWeekEnd4v4Base(scene, WEEKEND4V4_TYPEID_CRAZYBOMB)
{
}

SceneWeekEnd4v4CrazyBomb::~SceneWeekEnd4v4CrazyBomb()
{
}

bool SceneWeekEnd4v4CrazyBomb::OnEventRoleQuit(Role* pRole)
{
    return true;
}

bool SceneWeekEnd4v4CrazyBomb::OnEventRoleDie(Role* pRole)
{
    PushReviveList(pRole->GetRoleID());
    AddScore(pRole->GetRoleID());
    WKBoardcastRoleData(pRole->GetRoleID(), false);
    return true;
}

void SceneWeekEnd4v4CrazyBomb::FillBattleData(KKSG::WeekEnd4v4BattleRoleData* pProtocData, UINT32 index, const SceneWeekEnd4v4Result& roledata)
{
    pProtocData->set_bekilledcount(roledata.GetCount());
    pProtocData->set_score(GetScore(roledata.GetCount()));
}

void SceneWeekEnd4v4CrazyBomb::InitRole(Role* pRole)
{
    if(NULL == pRole)
    {
        return;
    }
    AddBuffUnbeatable(pRole);
}

void SceneWeekEnd4v4CrazyBomb::ReviveRole(UINT64 roleid)
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

void SceneWeekEnd4v4CrazyBomb::GetTeamScore(UINT32& redScore, UINT32& blueScore)
{
    // RedTeam的总积分是BlueTeam的总死亡次数
    for(UINT32 i = 0; i< m_roleCount; i++)
    {
        redScore += GetScore(m_roleResult[i+m_roleCount].GetCount());
        blueScore += GetScore(m_roleResult[i].GetCount());
    }
}
