#include "pch.h"
#include "partner/rpcc2m_getpartnerinfo.h"
#include "partner/partnermgr.h"
#include "role/rolemanager.h"
#include "partner/rpcm2g_getpartnerrecordm2g.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2016/12/9 23:13:51

RPC_SERVER_IMPLEMETION(RpcC2M_GetPartnerInfo, GetPartnerInfoArg, GetPartnerInfoRes)

void RpcC2M_GetPartnerInfo::OnCall(const GetPartnerInfoArg &roArg, GetPartnerInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("can't find role by session[%llu]", m_sessionID);
		return ;
	}

	RpcM2G_GetPartnerRecordM2G* rpc = RpcM2G_GetPartnerRecordM2G::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	Partner* partner = PartnerMgr::Instance()->GetPartnerByRole(pRole->GetID());
	if (partner)
	{
		rpc->m_oArg.set_partnerid(partner->GetID());
		rpc->m_oArg.set_level(partner->GetLevel());
		rpc->m_oArg.set_liveness(partner->GetLiveness());
		rpc->m_oArg.set_shop_refresh_time(partner->GetPartnerShop().GetLastRefreshTime());
		PartnerMember* m = partner->GetMember(pRole->GetID());
		if (m)
		{
			rpc->m_oArg.set_apply_leave_time(m->applyLeaveTime);
		}
	}
	else
	{
		rpc->m_oArg.set_partnerid(0);
	}
	rpc->m_oArg.set_rpcid(DelayRpc());
	pRole->SendMsgToGS(*rpc);
}

void RpcC2M_GetPartnerInfo::OnDelayReplyRpc(const GetPartnerInfoArg &roArg, GetPartnerInfoRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("can't find role by session[%llu]", m_sessionID);
		return ;
	}
	KKSG::GetPartnerRecordM2GRes* res = NULL;
	if (roUserData.m_pUserPtr)
	{
		res = (KKSG::GetPartnerRecordM2GRes*)(roUserData.m_pUserPtr);
		roRes.set_last_leave_time(res->last_leave_time());
		roRes.set_liveness_redpoint(res->liveness_redpoint());
	}
	else
	{
		roRes.set_last_leave_time(0);
		roRes.set_liveness_redpoint(false);
	}


	Partner* partner = PartnerMgr::Instance()->GetPartnerByRole(pRole->GetID());
	if (partner == NULL)
	{
		roRes.set_id(0);
		return;
	}

	roRes.set_id(partner->GetID());
	roRes.set_degree(partner->GetDegree());
	roRes.set_level(partner->GetLevel());

	// 成员
	std::vector<UINT64> roleIds;
	partner->GetPartnerMemberIds(roleIds);
	for (auto it = roleIds.begin(); it != roleIds.end(); ++it)
	{
		roRes.add_memberids(*it);
	}

	// 商店红点
	PartnerShop& shop = partner->GetPartnerShop();
	roRes.set_shop_redpoint(shop.HasRedPoint(pRole->GetID()));
}
