#include "pch.h"
#include "goalawards/rpcc2m_goalawardsgetlist.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "goalawards/goalawardsmgr.h"
// generate by ProtoGen at date: 2017/9/1 19:44:53

RPC_SERVER_IMPLEMETION(RpcC2M_GoalAwardsGetList, GoalAwardsGetList_C2M, GoalAwardsGetList_M2C)

void RpcC2M_GoalAwardsGetList::OnCall(const GoalAwardsGetList_C2M &roArg, GoalAwardsGetList_M2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    GoalAwardsMgr::Instance()->RpcGoalAwardsGetList(pRole, roArg, roRes);
}

void RpcC2M_GoalAwardsGetList::OnDelayReplyRpc(const GoalAwardsGetList_C2M &roArg, GoalAwardsGetList_M2C &roRes, const CUserData &roUserData)
{
}
