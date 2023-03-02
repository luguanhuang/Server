#include "pch.h"
#include "wedding/rpcc2m_weddinginviteoperator.h"
#include "marriage/weddingmgr.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2017/7/24 20:20:30

RPC_SERVER_IMPLEMETION(RpcC2M_WeddingInviteOperator, WeddingInviteOperatorArg, WeddingInviteOperatorRes)

void RpcC2M_WeddingInviteOperator::OnCall(const WeddingInviteOperatorArg &roArg, WeddingInviteOperatorRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Wedding* pWed = NULL;
	if (roArg.type() == KKSG::Wedding_Apply)
	{
		pWed = WeddingMgr::Instance()->GetWedding(roArg.weddingid());
	}
	else
	{
		pWed = WeddingMgr::Instance()->GetWeddingByRoleId(pRole->GetID());
	}
	if (pWed == NULL)
	{
		roRes.set_result(KKSG::ERR_WEDDING_NOT_EXISTS);	
		return;
	}

	KKSG::ErrorCode result = KKSG::ERR_SUCCESS;
	switch (roArg.type())
	{
	case KKSG::Wedding_Apply:
		result = pWed->DoApply(pRole->GetID());
		break;
	case KKSG::Wedding_Invite:
		result = pWed->DoInvite(pRole->GetID(), roArg.roleid());
		break;
	case KKSG::Wedding_AgreeApply:
		result = pWed->DoAgreeApply(pRole->GetID(), roArg.roleid());
		break;
	case KKSG::Wedding_DisagreeApply:
		result = pWed->DoDisAgreeApply(pRole->GetID(), roArg.roleid());
		break;
	case KKSG::Wedding_PermitStranger:
		pWed->SetPermitStranger(true);
		break;
	case KKSG::Wedding_ForbidStranger:
		pWed->SetPermitStranger(false);
		break;
	default:
		roRes.set_result(KKSG::ERR_FAILED);
		break;
	}

	roRes.set_result(result);
}

void RpcC2M_WeddingInviteOperator::OnDelayReplyRpc(const WeddingInviteOperatorArg &roArg, WeddingInviteOperatorRes &roRes, const CUserData &roUserData)
{
}
