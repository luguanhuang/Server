#include "pch.h"
#include "dragonguild/rpcg2m_getdragonguildinfog2m.h"
#include "dragonguild/dragonguildmgr.h"
#include "role/rolemanager.h"
// generate by ProtoGen at date: 2017/9/6 11:19:23

RPC_SERVER_IMPLEMETION(RpcG2M_GetDragonGuildInfoG2M, GetPartnerInfoG2MArg, GetPartnerInfoG2MRes)

void RpcG2M_GetDragonGuildInfoG2M::OnCall(const GetPartnerInfoG2MArg &roArg, GetPartnerInfoG2MRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;	
	}
	DragonGuild* p = DragonGuildMgr::Instance()->GetDragonGuildByRole(pRole->GetID());
	if (p == NULL)
	{
		roRes.set_partnerid(0);
		return;
	}
	roRes.set_partnerid(p->GetID());
	roRes.set_level(p->GetLevel());
	roRes.set_liveness(p->GetLiveness());
}

void RpcG2M_GetDragonGuildInfoG2M::OnDelayReplyRpc(const GetPartnerInfoG2MArg &roArg, GetPartnerInfoG2MRes &roRes, const CUserData &roUserData)
{
}
