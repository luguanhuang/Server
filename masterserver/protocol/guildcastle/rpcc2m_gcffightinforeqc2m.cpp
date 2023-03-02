#include "pch.h"
#include "guildcastle/rpcc2m_gcffightinforeqc2m.h"
#include "role/rolemanager.h"
#include "guildcastle/guildcastlerequest.h"

// generate by ProtoGen at date: 2016/12/20 10:29:26

RPC_SERVER_IMPLEMETION(RpcC2M_GCFFightInfoReqC2M, GCFFightInfoArg, GCFFightInfoRes)

void RpcC2M_GCFFightInfoReqC2M::OnCall(const GCFFightInfoArg &roArg, GCFFightInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		return ;
	}
	GcfRequest req(pRole);
	req.FightInfos(roRes);

}

void RpcC2M_GCFFightInfoReqC2M::OnDelayReplyRpc(const GCFFightInfoArg &roArg, GCFFightInfoRes &roRes, const CUserData &roUserData)
{
}
