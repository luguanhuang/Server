#include "pch.h"
#include "team/rpcm2g_teamsynm2c.h"
#include "gamelogic/teammgr.h"

// generate by ProtoGen at date: 2016/8/16 20:52:03

RPC_SERVER_IMPLEMETION(RpcM2G_TeamSynM2C, TeamSynPara, TeamSynRes)

void RpcM2G_TeamSynM2C::OnCall(const TeamSynPara &roArg, TeamSynRes &roRes)
{
	TeamCopyMgr::Instance()->SynFromMS(roArg, roRes);
}

void RpcM2G_TeamSynM2C::OnDelayReplyRpc(const TeamSynPara &roArg, TeamSynRes &roRes, const CUserData &roUserData)
{
}
