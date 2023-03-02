#include "pch.h"
#include "partner/rpcc2m_getpartnershoprecord.h"
#include "partner/partnermgr.h"
#include "role/rolemanager.h"
#include "partner/rpcm2g_getpartnerrecordm2g.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2016/12/17 12:14:19

RPC_SERVER_IMPLEMETION(RpcC2M_GetPartnerShopRecord, GetPartnerShopRecordArg, GetPartnerShopRecordRes)

void RpcC2M_GetPartnerShopRecord::OnCall(const GetPartnerShopRecordArg &roArg, GetPartnerShopRecordRes &roRes)
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
	shop.FillShopRecords(roRes);
}

void RpcC2M_GetPartnerShopRecord::OnDelayReplyRpc(const GetPartnerShopRecordArg &roArg, GetPartnerShopRecordRes &roRes, const CUserData &roUserData)
{
}
