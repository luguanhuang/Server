#include "pch.h"
#include "partner/rpcc2m_leavepartner.h"
#include "partner/partnermgr.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2016/12/12 11:09:40

RPC_SERVER_IMPLEMETION(RpcC2M_LeavePartner, LeavePartnerArg, LeavePartnerRes)

void RpcC2M_LeavePartner::OnCall(const LeavePartnerArg &roArg, LeavePartnerRes &roRes)
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
	roRes.set_result(partner->LeaveMember(pRole));
}

void RpcC2M_LeavePartner::OnDelayReplyRpc(const LeavePartnerArg &roArg, LeavePartnerRes &roRes, const CUserData &roUserData)
{
}
