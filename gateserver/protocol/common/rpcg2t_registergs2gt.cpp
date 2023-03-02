#include "pch.h"
#include "common/rpcg2t_registergs2gt.h"
#include "network/gslink.h"
#include "config.h"

// generate by ProtoGen at date: 2016/7/8 18:08:47

RPC_SERVER_IMPLEMETION(RpcG2T_RegisterGs2Gt, RegisterGs2GtArg, RegisterGs2GtRes)

void RpcG2T_RegisterGs2Gt::OnCall(const RegisterGs2GtArg &roArg, RegisterGs2GtRes &roRes)
{
	KKSG::ErrorCode nErrCode = GSLink::Instance()->AddGameServer(GetConnID(), roArg.serverid(), roArg.line());
	roRes.set_errorcode(nErrCode);
	roRes.set_gtline(GateConfig::Instance()->GetLine());
}

void RpcG2T_RegisterGs2Gt::OnDelayReplyRpc(const RegisterGs2GtArg &roArg, RegisterGs2GtRes &roRes, const CUserData &roUserData)
{
}
