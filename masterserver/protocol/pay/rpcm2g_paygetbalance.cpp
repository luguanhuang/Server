#include "pch.h"
#include "pay/rpcm2g_paygetbalance.h"
#include "pay/paymgr.h"

// generate by ProtoGen at date: 2016/12/21 23:38:22

RPC_CLIENT_IMPLEMETION(RpcM2G_PayGetBalance, PayGetBalanceArg, PayGetBalanceRes)

void RpcM2G_PayGetBalance::OnReply(const PayGetBalanceArg &roArg, const PayGetBalanceRes &roRes, const CUserData &roUserData)
{
	if (roRes.result() == KKSG::ERR_SUCCESS)
	{
		for (INT32 i = 0 ; i < roRes.paracode_size(); i ++)
		{
			CPayMgr::Instance()->PayOtherFinsih(roRes.paracode(i), roArg.roleid());
		}
	}
}

void RpcM2G_PayGetBalance::OnTimeout(const PayGetBalanceArg &roArg, const CUserData &roUserData)
{
}
