#include "pch.h"
#include "partner/rpcc2m_getpartnerdetailinfo.h"
#include "partner/partnermgr.h"
#include "role/rolemanager.h"
#include "partner/rpcm2g_getpartnerrecordm2g.h"

// generate by ProtoGen at date: 2016/12/12 0:15:52

RPC_SERVER_IMPLEMETION(RpcC2M_GetPartnerDetailInfo, GetPartnerDetailInfoArg, GetPartnerDetailInfoRes)

void RpcC2M_GetPartnerDetailInfo::OnCall(const GetPartnerDetailInfoArg &roArg, GetPartnerDetailInfoRes &roRes)
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
	PartnerMember* m = partner->GetMember(pRole->GetID());
	if (m)
	{
		rpc->m_oArg.set_apply_leave_time(m->applyLeaveTime);
	}
	pRole->SendMsgToGS(*rpc);
}

void RpcC2M_GetPartnerDetailInfo::OnDelayReplyRpc(const GetPartnerDetailInfoArg &roArg, GetPartnerDetailInfoRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("can't find role by session[%llu]", m_sessionID);
		return ;
	}
	Partner* partner = PartnerMgr::Instance()->GetPartnerByRole(pRole->GetID());
	if (partner == NULL)
	{
		roRes.set_result(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	roRes.set_partnerid(partner->GetID());

	KKSG::GetPartnerRecordM2GRes* res = NULL;
	if (roUserData.m_pUserPtr)
	{
		res = (KKSG::GetPartnerRecordM2GRes*)(roUserData.m_pUserPtr);
		roRes.set_liveness_redpoint(res->liveness_redpoint());
	}
	else
	{
		roRes.set_liveness_redpoint(false);
	}

	roRes.set_level(partner->GetLevel());
	roRes.set_degree(partner->GetDegree());
	partner->FillAllMemberDetail(roRes);

	// 商店红点
	PartnerShop& shop = partner->GetPartnerShop();
	roRes.set_shop_redpoint(shop.HasRedPoint(pRole->GetID()));
}
