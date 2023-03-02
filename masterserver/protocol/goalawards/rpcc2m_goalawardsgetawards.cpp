#include "pch.h"
#include "goalawards/rpcc2m_goalawardsgetawards.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "goalawards/goalawardsmgr.h"
// generate by ProtoGen at date: 2017/9/1 19:46:26

RPC_SERVER_IMPLEMETION(RpcC2M_GoalAwardsGetAwards, GoalAwardsGetAwards_C2M, GoalAwardsGetAwards_M2C)

void RpcC2M_GoalAwardsGetAwards::OnCall(const GoalAwardsGetAwards_C2M &roArg, GoalAwardsGetAwards_M2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    GoalAwardsMgr::Instance()->RpcGoalAwardsGetAwards(pRole, roArg, roRes);
}

void RpcC2M_GoalAwardsGetAwards::OnDelayReplyRpc(const GoalAwardsGetAwards_C2M &roArg, GoalAwardsGetAwards_M2C &roRes, const CUserData &roUserData)
{
}
