#include "pch.h"
#include "chat/rpcc2g_queryclientip.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2016/12/21 15:34:48

RPC_SERVER_IMPLEMETION(RpcC2G_QueryClientIp, QueryClientIpArg, QueryClientIpRes)

void RpcC2G_QueryClientIp::OnCall(const QueryClientIpArg &roArg, QueryClientIpRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(!pRole)
	{
		SSError << "Cannot Find Role, SessionID:" << m_sessionID << END; 
		return; 
	}
	roRes.set_ip(pRole->GetClientConf().ip());
}

void RpcC2G_QueryClientIp::OnDelayReplyRpc(const QueryClientIpArg &roArg, QueryClientIpRes &roRes, const CUserData &roUserData)
{
}
