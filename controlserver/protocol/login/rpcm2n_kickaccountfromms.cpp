#include "pch.h"
#include "login/rpcm2n_kickaccountfromms.h"
#include "account/accountkick.h"

// generate by ProtoGen at date: 2016/11/4 17:38:43

RPC_SERVER_IMPLEMETION(RpcM2N_KickAccountFromMs, KickAccountFromMsArg, KickAccountFromMsRes)

void RpcM2N_KickAccountFromMs::OnCall(const KickAccountFromMsArg &roArg, KickAccountFromMsRes &roRes)
{
	AccountKickInfo* poKickInfo = NULL;
	ErrorCode nErrCode = CAccountKicker::Instance()->KickAccount(roArg.roleid(), roArg.kicktype(), &poKickInfo);
	if(nErrCode != KKSG::ERR_SUCCESS)
	{
		roRes.set_result(nErrCode);
		return;
	}

	if(poKickInfo == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	poKickInfo->m_dwRpcDelayed = DelayRpc();
}

void RpcM2N_KickAccountFromMs::OnDelayReplyRpc(const KickAccountFromMsArg &roArg, KickAccountFromMsRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);
}
