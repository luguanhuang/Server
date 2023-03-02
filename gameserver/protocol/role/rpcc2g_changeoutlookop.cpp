#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "unit/roleoutlook.h"
#include "role/rpcc2g_changeoutlookop.h"

// generate by ProtoGen at date: 2016/8/30 12:07:27

RPC_SERVER_IMPLEMETION(RpcC2G_ChangeOutLookOp, ChangeOutLookOpArg, ChangeOutLookOpRes)

void RpcC2G_ChangeOutLookOp::OnCall(const ChangeOutLookOpArg &roArg, ChangeOutLookOpRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	if (!roArg.has_op())
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}
	*role->GetRoleAllInfoPtr()->mutable_brief()->mutable_op() = roArg.op();

	RoleOutLook outlook(role);
	outlook.Fill(SUMMARY_OP, NULL);
	outlook.BroadCast();

	RoleSummaryMgr::Instance()->UpdateRoleSummary(role, SUMMARY_OP);

	roRes.set_errorcode(ERR_SUCCESS);
}

void RpcC2G_ChangeOutLookOp::OnDelayReplyRpc(const ChangeOutLookOpArg &roArg, ChangeOutLookOpRes &roRes, const CUserData &roUserData)
{
}
