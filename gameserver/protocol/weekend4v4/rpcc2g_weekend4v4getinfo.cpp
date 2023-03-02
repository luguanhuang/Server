#include "pch.h"
#include "weekend4v4/rpcc2g_weekend4v4getinfo.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/weekend4v4mgr.h"

// generate by ProtoGen at date: 2017/6/6 19:41:14

RPC_SERVER_IMPLEMETION(RpcC2G_WeekEnd4v4GetInfo, WeekEnd4v4GetInfoArg, WeekEnd4v4GetInfoRes)

void RpcC2G_WeekEnd4v4GetInfo::OnCall(const WeekEnd4v4GetInfoArg &roArg, WeekEnd4v4GetInfoRes &roRes)
{
    Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
    if(!pRole)
    {
        roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
        return;
    }
    WeekEnd4v4Mgr::Instance()->FillWeekEnd4v4Info(pRole, roRes);
}

void RpcC2G_WeekEnd4v4GetInfo::OnDelayReplyRpc(const WeekEnd4v4GetInfoArg &roArg, WeekEnd4v4GetInfoRes &roRes, const CUserData &roUserData)
{
}
