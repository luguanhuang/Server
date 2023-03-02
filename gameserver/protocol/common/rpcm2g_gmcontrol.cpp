#include "pch.h"
#include "common/rpcm2g_gmcontrol.h"
#include "unit/rolemanager.h"
#include "gamelogic/gmhandler.h"

// generate by ProtoGen at date: 2015/8/21 15:20:52

RPC_SERVER_IMPLEMETION(RpcM2G_GMControl, GMCmdArg, GMCmdRes)

void RpcM2G_GMControl::OnCall(const GMCmdArg &roArg, GMCmdRes &roRes)
{
	GMHandler::Instance()->ExecuteCmd(roArg, roRes);
}

void RpcM2G_GMControl::OnDelayReplyRpc(const GMCmdArg &roArg, GMCmdRes &roRes, const CUserData &roUserData)
{
}
