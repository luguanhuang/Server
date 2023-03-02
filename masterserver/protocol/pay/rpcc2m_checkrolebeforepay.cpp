#include "pch.h"
#include "pay/rpcc2m_checkrolebeforepay.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "pay/paymgr.h"

// generate by ProtoGen at date: 2017/7/28 10:11:38

RPC_SERVER_IMPLEMETION(RpcC2M_CheckRoleBeforePay, CheckRoleBeforePayArg, CheckRoleBeforePayRes)

void RpcC2M_CheckRoleBeforePay::OnCall(const CheckRoleBeforePayArg &roArg, CheckRoleBeforePayRes &roRes)
{
	roRes.set_result(KKSG::ERR_SUCCESS);
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}
	if (CPayMgr::Instance()->IsGMPayCheckRole() || role->GetID() != roArg.roleid() || role->GetServerId() != roArg.serverid() || role->GetAccount() != roArg.openid())
	{
		roRes.set_result(KKSG::ERR_INVALID_PAYROLE);
		LogWarn("invalid, pay role, client request roleid=%llu,serverid=%u,account=%s,server roleid=%llu,serverid=%u,account=%s", roArg.roleid(), roArg.serverid(), roArg.openid().c_str(), 
			role->GetID(), role->GetServerId(), role->GetAccount().c_str());
		return;
	}
}

void RpcC2M_CheckRoleBeforePay::OnDelayReplyRpc(const CheckRoleBeforePayArg &roArg, CheckRoleBeforePayRes &roRes, const CUserData &roUserData)
{
}
