#include "pch.h"
#include "login/rpcc2t_reconnect.h"
#include "session/sessionmanager.h"

// generate by ProtoGen at date: 2015/6/4 15:09:56

RPC_SERVER_IMPLEMETION(RpcC2T_Reconnect, ReconnArg, ReconnRes)

void RpcC2T_Reconnect::OnCall(const ReconnArg &roArg, ReconnRes &roRes)
{
	UINT32 delayrpcid = DelayRpc();
	if (!SessionManager::Instance()->ReConnectBySession(m_dwConnID, roArg.session(), roArg.roleid(), roArg.sceneid(), delayrpcid))
	{
		CUserData data;
		data.m_dwUserData = KKSG::ERR_FAILED;
		CRpc::ReplyDelayRpc(delayrpcid, data);
	}
}

void RpcC2T_Reconnect::OnDelayReplyRpc(const ReconnArg &roArg, ReconnRes &roRes, const CUserData &roUserData)
{
	roRes.set_result(KKSG::ErrorCode(roUserData.m_dwUserData));
}
