#include "pch.h"
#include "common/rpcm2n_registerms2ns.h"
#include "config.h"

// generate by ProtoGen at date: 2017/6/9 17:30:23

RPC_CLIENT_IMPLEMETION(RpcM2N_registerms2ns, RegisterMs2NsArg, RegisterMs2NsRes)

void RpcM2N_registerms2ns::OnReply(const RegisterMs2NsArg &roArg, const RegisterMs2NsRes &roRes, const CUserData &roUserData)
{
	//MSConfig::Instance()->SetBackFlowLevel(roRes.backflowlevel());
	MSConfig::Instance()->SyncVersion2Control();
}

void RpcM2N_registerms2ns::OnTimeout(const RegisterMs2NsArg &roArg, const CUserData &roUserData)
{
}
