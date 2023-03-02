#include "pch.h"
#include "team/rpcc2m_invhistoryc2mreq.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "team/teaminvitemgr.h"

// generate by ProtoGen at date: 2016/8/18 14:22:13

RPC_SERVER_IMPLEMETION(RpcC2M_InvHistoryC2MReq, InvHistoryArg, InvHistoryRes)

void RpcC2M_InvHistoryC2MReq::OnCall(const InvHistoryArg &roArg, InvHistoryRes &roRes)
{
	CRole *pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_ret(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	roRes.set_ret(KKSG::ERR_SUCCESS);
	switch(roArg.type())
	{
	case KKSG::INVH_REQ_UNF_LIST:
		{
			TeamInviteMgr::Instance()->FillAllInvToMe(pRole->GetID(), roRes);
		}
		break;
	case KKSG::INVH_UNF_IGNORE_ALL:
		{
			TeamInviteMgr::Instance()->DelInvToMe(pRole->GetID());
		}
		break;
	case KKSG::INVH_REFUSE_FORNOW:
		{
			TeamInviteMgr::Instance()->DelInvToMe(pRole->GetID());
			TeamInviteMgr::Instance()->AddUnfRefuseForNow(pRole->GetID());
		}
		break;
	default:
		break;
	}
}

void RpcC2M_InvHistoryC2MReq::OnDelayReplyRpc(const InvHistoryArg &roArg, InvHistoryRes &roRes, const CUserData &roUserData)
{
}
