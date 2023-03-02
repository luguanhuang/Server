#include "pch.h"
#include "invfight/rpcs2m_invfightcross2msreq.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "invfight/invfightcrossrequest.h"
#include "invfight/invfightcrossmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"

// generate by ProtoGen at date: 2017/4/13 16:01:33

RPC_SERVER_IMPLEMETION(RpcS2M_InvFightCross2MSReq, InvFightCross2MSReqArg, InvFightCross2MSReqRes)

void RpcS2M_InvFightCross2MSReq::OnCall(const InvFightCross2MSReqArg &roArg, InvFightCross2MSReqRes &roRes)
{
	roRes.set_error(KKSG::ERR_FAILED);

	CRole* role = NULL;

	switch(roArg.reqtype())
	{
	case KKSG::IFRT_INV_ONE:
		{
			role = CRoleManager::Instance()->GetByRoleID(roArg.invmsg().toid());
			if (!role)
			{
				return;
			}
			InvFightCrossRequest req(role);
			UINT32 error = req.InvToOne(roArg.invmsg().invid(), roArg.invmsg().fromid());
			if (error == KKSG::ERR_SUCCESS)
			{
				InvFightCrossMgr::Instance()->AddToCrossFight(roArg.invmsg());
			}
			roRes.set_error((KKSG::ErrorCode)error);
			roRes.mutable_invmsg()->CopyFrom(roArg.invmsg());
		}
		break;
	case KKSG::IFRT_REFUSH_ONE:
		{
			role = CRoleManager::Instance()->GetByRoleID(roArg.invmsg().fromid());
			if (!role)
			{
				return;
			}
			InvFightCrossRequest req(role);
			UINT32 error = req.RefushOneFrom(roArg.invmsg().invid());
			roRes.set_error((KKSG::ErrorCode)error);
		}
		break;
	case KKSG::IFRT_ACCEPT_ONE:
		{
			role = CRoleManager::Instance()->GetByRoleID(roArg.invmsg().fromid());
			if (!role)
			{
				roRes.set_error(KKSG::ERR_ROLE_NOT_ONLINE);
				return;
			}
			InvFightCrossRequest req(role);
			UINT32 error = req.AcceptOneFrom(roArg.invmsg().invid());
			roRes.set_error((KKSG::ErrorCode)error);
			roRes.mutable_invmsg()->CopyFrom(roArg.invmsg());
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(role->GetID());
			if (summary)
			{
				summary->FillSmallInfo(*roRes.mutable_invmsg()->mutable_smallinfo()->add_roles());
			}
		}
		break;
	default:
		break;
	}
}

void RpcS2M_InvFightCross2MSReq::OnDelayReplyRpc(const InvFightCross2MSReqArg &roArg, InvFightCross2MSReqRes &roRes, const CUserData &roUserData)
{
}
