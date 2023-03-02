#include "pch.h"
#include "guildcastle/rpcc2m_gcfreadysinforeq.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "guildcastle/guildcastlerequest.h"

// generate by ProtoGen at date: 2016/12/14 23:57:06

RPC_SERVER_IMPLEMETION(RpcC2M_GCFReadysInfoReq, GCFReadyInfoArg, GCFReadyInfoRes)

void RpcC2M_GCFReadysInfoReq::OnCall(const GCFReadyInfoArg &roArg, GCFReadyInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		return ;
	}
	GcfRequest req(pRole);
	req.ReadyInfos(roRes);
}

void RpcC2M_GCFReadysInfoReq::OnDelayReplyRpc(const GCFReadyInfoArg &roArg, GCFReadyInfoRes &roRes, const CUserData &roUserData)
{
}
