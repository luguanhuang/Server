#include "pch.h"
#include "common/rpct2g_registergt2gs.h"
#include "gatelink.h"
#include "config.h"

// generate by ProtoGen at date: 2016/7/8 18:12:58

RPC_SERVER_IMPLEMETION(RpcT2G_RegisterGt2Gs, RegisterGt2GsArg, RegisterGt2GsRes)

void RpcT2G_RegisterGt2Gs::OnCall(const RegisterGt2GsArg &roArg, RegisterGt2GsRes &roRes)
{
	KKSG::ErrorCode nErrCode = GatetLink::Instance()->AddGateServer(GetConnID(), roArg.serverid(), roArg.line());
	roRes.set_errorcode(nErrCode);
	roRes.set_gsline(GSConfig::Instance()->GetLine());
}

void RpcT2G_RegisterGt2Gs::OnDelayReplyRpc(const RegisterGt2GsArg &roArg, RegisterGt2GsRes &roRes, const CUserData &roUserData)
{
}
