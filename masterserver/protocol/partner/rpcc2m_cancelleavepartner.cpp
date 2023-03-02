#include "pch.h"
#include "partner/rpcc2m_cancelleavepartner.h"
#include "partner/partnermgr.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2016/12/12 15:47:10

RPC_SERVER_IMPLEMETION(RpcC2M_CancelLeavePartner, CancelLeavePartnerArg, CancelLeavePartnerRes)

void RpcC2M_CancelLeavePartner::OnCall(const CancelLeavePartnerArg &roArg, CancelLeavePartnerRes &roRes)
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
	roRes.set_result(partner->CancelLeaveMember(pRole));
}

void RpcC2M_CancelLeavePartner::OnDelayReplyRpc(const CancelLeavePartnerArg &roArg, CancelLeavePartnerRes &roRes, const CUserData &roUserData)
{
}
