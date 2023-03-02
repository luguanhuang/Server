#include "pch.h"
#include "dragongroup/rpcc2g_dragongrouprolelist.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/dragongroupmgr.h"

// generate by ProtoGen at date: 2017/7/28 13:12:54

RPC_SERVER_IMPLEMETION(RpcC2G_DragonGroupRoleList, DragonGroupRoleListC2S, DragonGroupRoleListS2C)

void RpcC2G_DragonGroupRoleList::OnCall(const DragonGroupRoleListC2S &roArg, DragonGroupRoleListS2C &roRes)
{
    Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
    if(!pRole)
    {
        roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
        return;
    }
    DragonGroupMgr::Instance()->RpcGetDragonGroupRoleList(pRole, roArg, roRes);
}

void RpcC2G_DragonGroupRoleList::OnDelayReplyRpc(const DragonGroupRoleListC2S &roArg, DragonGroupRoleListS2C &roRes, const CUserData &roUserData)
{
}
