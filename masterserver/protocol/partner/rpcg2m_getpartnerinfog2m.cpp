#include "pch.h"
#include "partner/rpcg2m_getpartnerinfog2m.h"
#include "partner/partnermgr.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2016/12/10 22:00:34

RPC_SERVER_IMPLEMETION(RpcG2M_GetPartnerInfoG2M, GetPartnerInfoG2MArg, GetPartnerInfoG2MRes)

void RpcG2M_GetPartnerInfoG2M::OnCall(const GetPartnerInfoG2MArg &roArg, GetPartnerInfoG2MRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;	
	}
	Partner* p = PartnerMgr::Instance()->GetPartnerByRole(pRole->GetID());
	if (p == NULL)
	{
		roRes.set_partnerid(0);
		return;
	}
	roRes.set_partnerid(p->GetID());
	roRes.set_level(p->GetLevel());
	roRes.set_liveness(p->GetLiveness());
}

void RpcG2M_GetPartnerInfoG2M::OnDelayReplyRpc(const GetPartnerInfoG2MArg &roArg, GetPartnerInfoG2MRes &roRes, const CUserData &roUserData)
{
}
