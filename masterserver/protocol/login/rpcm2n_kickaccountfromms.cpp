#include "pch.h"
#include "login/rpcm2n_kickaccountfromms.h"
#include "account/accountkick.h"

// generate by ProtoGen at date: 2016/11/4 17:38:43

RPC_CLIENT_IMPLEMETION(RpcM2N_KickAccountFromMs, KickAccountFromMsArg, KickAccountFromMsRes)

void RpcM2N_KickAccountFromMs::OnReply(const KickAccountFromMsArg &roArg, const KickAccountFromMsRes &roRes, const CUserData &roUserData)
{
	CAccountKicker::Instance()->OnKickReply(roArg.accountid(), roRes.result());
}

void RpcM2N_KickAccountFromMs::OnTimeout(const KickAccountFromMsArg &roArg, const CUserData &roUserData)
{
	CAccountKicker::Instance()->OnKickReply(roArg.accountid(), KKSG::ERR_TIMEOUT);
}
