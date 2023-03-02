#include "pch.h"
#include "pay/rpcg2m_paydegreereq.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "httptask/paynotify.h"
#include "pay/paymgr.h"
#include "httptask/httptaskmgr.h"

// generate by ProtoGen at date: 2017/4/24 11:09:30

RPC_SERVER_IMPLEMETION(RpcG2M_PayDegreeReq, PayDegreeArg, PayDegreeRes)

void RpcG2M_PayDegreeReq::OnCall(const PayDegreeArg &roArg, PayDegreeRes &roRes)
{
	roRes.set_error(KKSG::ERR_SUCCESS);
	CRole* role = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
	if (NULL == role)
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		SSWarn << "role is NULL and roleid is :" << roArg.roleid() <<END;
		return;
	}
	CPayMgr::Instance()->PayNotify(role, KKSG::PAY_PARAM_LIST, "");
}

void RpcG2M_PayDegreeReq::OnDelayReplyRpc(const PayDegreeArg &roArg, PayDegreeRes &roRes, const CUserData &roUserData)
{
	roRes.set_error(KKSG::ERR_FAILED);
	if (roUserData.m_dwUserData == 0)
	{
		KKSG::PayDegreeRes* pData = (KKSG::PayDegreeRes*)roUserData.m_pUserPtr;
		roRes.CopyFrom(*pData);
	}
}
