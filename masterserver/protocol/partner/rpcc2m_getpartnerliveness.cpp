#include "pch.h"
#include "partner/rpcc2m_getpartnerliveness.h"
#include "partner/partnermgr.h"
#include "role/rolemanager.h"
#include "partner/rpcm2g_getpartnerrecordm2g.h"

// generate by ProtoGen at date: 2016/12/12 10:12:18

RPC_SERVER_IMPLEMETION(RpcC2M_GetPartnerLiveness, GetPartnerLivenessArg, GetPartnerLivenessRes)

void RpcC2M_GetPartnerLiveness::OnCall(const GetPartnerLivenessArg &roArg, GetPartnerLivenessRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("can't find role by session[%llu]", m_sessionID);
		roRes.set_result(KKSG::ERR_FAILED);
		return ;
	}

	Partner* partner = PartnerMgr::Instance()->GetPartnerByRole(pRole->GetID());
	if (partner == NULL)
	{
		roRes.set_result(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}
	RpcM2G_GetPartnerRecordM2G* rpc = RpcM2G_GetPartnerRecordM2G::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_partnerid(partner->GetID());
	rpc->m_oArg.set_level(partner->GetLevel());
	rpc->m_oArg.set_liveness(partner->GetLiveness());
	rpc->m_oArg.set_shop_refresh_time(partner->GetPartnerShop().GetLastRefreshTime());
	rpc->m_oArg.set_rpcid(DelayRpc());
	pRole->SendMsgToGS(*rpc);
}

void RpcC2M_GetPartnerLiveness::OnDelayReplyRpc(const GetPartnerLivenessArg &roArg, GetPartnerLivenessRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("can't find role by session[%llu]", m_sessionID);
		roRes.set_result(KKSG::ERR_FAILED);
		return ;
	}
	Partner* partner = PartnerMgr::Instance()->GetPartnerByRole(pRole->GetID());
	if (partner == NULL)
	{
		roRes.set_result(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}
	if (roUserData.m_pUserPtr == NULL)
	{
		roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
		return;	
	}

	roRes.set_result(KKSG::ERR_SUCCESS);
	KKSG::GetPartnerRecordM2GRes* res = (KKSG::GetPartnerRecordM2GRes*)(roUserData.m_pUserPtr);
	roRes.set_takedchest(res->takedchest());
	PartnerLiveness& ln = partner->GetPartnerLiveness();
	roRes.set_liveness(ln.GetLiveness());
	ln.FillLivenessRecord(roRes);
}
