#include "pch.h"
#include "dragongroup/rpcc2g_dragongrouprecord.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/dragongroupmgr.h"

// generate by ProtoGen at date: 2017/7/28 13:15:08

RPC_SERVER_IMPLEMETION(RpcC2G_DragonGroupRecord, DragonGroupRecordC2S, DragonGroupRecordS2C)

void RpcC2G_DragonGroupRecord::OnCall(const DragonGroupRecordC2S &roArg, DragonGroupRecordS2C &roRes)
{
    Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
    if(!pRole)
    {
        roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
        return;
    }
    DragonGroupMgr::Instance()->RpcGetDragonGroupRecord(pRole, roArg, roRes);
}

void RpcC2G_DragonGroupRecord::OnDelayReplyRpc(const DragonGroupRecordC2S &roArg, DragonGroupRecordS2C &roRes, const CUserData &roUserData)
{
}
