#include "pch.h"
#include "common/rpct2n_registergt2ns.h"
#include "config.h"
#include "network/gatelink.h"

// generate by ProtoGen at date: 2016/11/2 0:58:27

RPC_SERVER_IMPLEMETION(RpcT2N_RegisterGt2Ns, RegisterGt2NsArg, RegisterGt2NsRes)

void RpcT2N_RegisterGt2Ns::OnCall(const RegisterGt2NsArg &roArg, RegisterGt2NsRes &roRes)
{
	KKSG::ErrorCode nErrCode = GateLink::Instance()->AddGateServer(m_dwConnID, roArg.serverid(), roArg.line(), roArg.addr().ip(), roArg.addr().port());
	roRes.set_result(nErrCode);

	/*if(nErrCode == KKSG::ERR_SUCCESS)
	{
		PtcM2T_VersionNotify oPtc;
		oPtc.m_Data.set_type(MSConfig::Instance()->IsDebug() ? Version_None : Version_All);
		oPtc.m_Data.set_ios(MSConfig::Instance()->GetVersion()->ios);
		oPtc.m_Data.set_android(MSConfig::Instance()->GetVersion()->android);
		GateLink::Instance()->SendToLine(roArg.line(), oPtc);
	}*/
}

void RpcT2N_RegisterGt2Ns::OnDelayReplyRpc(const RegisterGt2NsArg &roArg, RegisterGt2NsRes &roRes, const CUserData &roUserData)
{
}
