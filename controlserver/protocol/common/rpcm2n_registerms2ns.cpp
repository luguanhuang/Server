#include "pch.h"
#include "common/rpcm2n_registerms2ns.h"
#include "config.h"

// generate by ProtoGen at date: 2017/6/9 17:30:23

RPC_SERVER_IMPLEMETION(RpcM2N_registerms2ns, RegisterMs2NsArg, RegisterMs2NsRes)

void RpcM2N_registerms2ns::OnCall(const RegisterMs2NsArg &roArg, RegisterMs2NsRes &roRes)
{
	//roRes.set_backflowlevel(ControlConfig::Instance()->GetBackFlowLevel());
}

void RpcM2N_registerms2ns::OnDelayReplyRpc(const RegisterMs2NsArg &roArg, RegisterMs2NsRes &roRes, const CUserData &roUserData)
{
}
