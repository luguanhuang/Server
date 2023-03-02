#include "pch.h"
#include "common/rpct2m_registergt2ms.h"
#include "network/gatelink.h"
#include "common/ptcm2t_versionnotify.h"
#include "config.h"
#include "network/rslink.h"

// generate by ProtoGen at date: 2016/7/5 21:11:37

RPC_SERVER_IMPLEMETION(RpcT2M_RegisterGt2Ms, RegisterGt2MsArg, RegisterGt2MsRes)

void RpcT2M_RegisterGt2Ms::OnCall(const RegisterGt2MsArg &roArg, RegisterGt2MsRes &roRes)
{
	KKSG::ErrorCode nErrCode = GateLink::Instance()->AddGateServer(m_dwConnID, roArg.serverid(), roArg.line());
	roRes.set_errorcode(nErrCode);

	if(nErrCode == KKSG::ERR_SUCCESS)
	{
		//MSConfig::SyncVersion2Control();
		RSLink::Instance()->NotifyRouter2Gate(roArg.line());
	}
}

void RpcT2M_RegisterGt2Ms::OnDelayReplyRpc(const RegisterGt2MsArg &roArg, RegisterGt2MsRes &roRes, const CUserData &roUserData)
{
}
