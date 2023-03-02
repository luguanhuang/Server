#include "pch.h"
#include "pay/rpcm2d_savepayaccessdata.h"
#include "pay/paymgr.h"

// generate by ProtoGen at date: 2016/12/21 17:24:44

RPC_CLIENT_IMPLEMETION(RpcM2D_SavePayAccessData, SavePayAccessArg, SavePayAccessDataRes)

void RpcM2D_SavePayAccessData::OnReply(const SavePayAccessArg &roArg, const SavePayAccessDataRes &roRes, const CUserData &roUserData)
{
	if (roArg.op() == KKSG::PayAccess_Consume)
	{
		CPayMgr::Instance()->SaveConsumeDataOnReply(roArg.roleid(), roArg.list(), roRes.result()==KKSG::ERR_SUCCESS?true:false);
	}
}

void RpcM2D_SavePayAccessData::OnTimeout(const SavePayAccessArg &roArg, const CUserData &roUserData)
{
	CPayMgr::Instance()->SaveConsumeDataOnReply(roArg.roleid(), roArg.list(), false);
}