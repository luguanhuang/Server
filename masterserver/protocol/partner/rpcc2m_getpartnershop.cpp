#include "pch.h"
#include "partner/rpcc2m_getpartnershop.h"
#include "partner/partnermgr.h"
#include "role/rolemanager.h"
#include "partner/rpcm2g_getpartnerrecordm2g.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2016/12/15 11:36:44

RPC_SERVER_IMPLEMETION(RpcC2M_GetPartnerShop, GetPartnerShopArg, GetPartnerShopRes)

void RpcC2M_GetPartnerShop::OnCall(const GetPartnerShopArg &roArg, GetPartnerShopRes &roRes)
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
	roRes.set_result(KKSG::ERR_SUCCESS);
	PartnerShop& shop = partner->GetPartnerShop();
	shop.FillShopItems(pRole->GetID(), roRes);
	PartnerMember* m = partner->GetMember(pRole->GetID());
	if (m)
	{
		m->openShopTime = GameTime::GetTime();
	}
}

void RpcC2M_GetPartnerShop::OnDelayReplyRpc(const GetPartnerShopArg &roArg, GetPartnerShopRes &roRes, const CUserData &roUserData)
{
}
