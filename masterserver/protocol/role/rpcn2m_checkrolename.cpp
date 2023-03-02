#include "pch.h"
#include "role/rpcn2m_checkrolename.h"
#include "role/rolesummarymgr.h"
#include "tsssdk/MsTssHandler.h"
#include "forbid/forbidwordmgr.h"
#include "util/XCommon.h"
#include "table/globalconfig.h"

// generate by ProtoGen at date: 2016/11/9 0:09:48

RPC_SERVER_IMPLEMETION(RpcN2M_CheckRoleName, CheckRoleNameArg, CheckRoleNameRes)

void RpcN2M_CheckRoleName::OnCall(const CheckRoleNameArg &roArg, CheckRoleNameRes &roRes)
{
	UINT32 errorcode = CRoleSummaryMgr::Instance()->IsNameInvalid(roArg.name());
	roRes.set_result(KKSG::ErrorCode(errorcode));
}

void RpcN2M_CheckRoleName::OnDelayReplyRpc(const CheckRoleNameArg &roArg, CheckRoleNameRes &roRes, const CUserData &roUserData)
{
}
