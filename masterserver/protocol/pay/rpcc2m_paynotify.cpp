#include "pch.h"
#include "pay/rpcc2m_paynotify.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "httptask/httptaskmgr.h"
#include "httptask/paynotify.h"
#include "pay/paymgr.h"
#include "pay/ptcm2g_buypayaileenntf.h"

// generate by ProtoGen at date: 2016/10/24 11:50:20

RPC_SERVER_IMPLEMETION(RpcC2M_PayNotify, PayNotifyArg, PayNotifyRes)

void RpcC2M_PayNotify::OnCall(const PayNotifyArg &roArg, PayNotifyRes &roRes)
{
	roRes.set_errcode(KKSG::ERR_SUCCESS);
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errcode(KKSG::ERR_INVALID_REQUEST);
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}
	LogInfo("pay notify roleid=%llu,paramtype=%d,paramid=%s,amount=%d,count=%d", role->GetID(),roArg.type(), roArg.paramid().c_str(), roArg.amount(), roArg.count());
	role->SetPayParameterInfo(roArg.data());
	role->SetIsPayFirstGetBalance(true);

	if (roArg.type() == KKSG::PAY_PARAM_AILEEN)
	{
		PtcM2G_BuyPayAileenNtf ptc;
		ptc.m_Data.set_roleid(role->GetID());
		ptc.m_Data.set_paramid(roArg.paramid());
		role->SendMsgToGS(ptc);
	}
	if (roArg.count() > 1)
	{
		CPayMgr::Instance()->PayNotify(role, KKSG::PAY_PARAM_LIST, "");//当特殊情况下 只做拉余额操作，不放入轮询队列;
		return;
	}
	
	PayNotifyTask* pTask = new PayNotifyTask();
	pTask->SetRoleId(role->GetID());
	pTask->SetRpcId(DelayRpc());
	pTask->SetParamType(roArg.type());
	pTask->SetParamId(roArg.paramid());
	pTask->SetParamData(roArg.data());
	CPayMgr::Instance()->BuildHttpParam(role, pTask->m_oparams);
	CPayMgr::Instance()->BuildSign(role, pTask->m_oparams, GetBalanceUrlPath);
	CPayMgr::Instance()->PayBegin(roArg.type(), roArg.paramid(), roArg.amount(), role->GetID());
	HttpTaskMgr::Instance()->AddTask(pTask);
}

void RpcC2M_PayNotify::OnDelayReplyRpc(const PayNotifyArg &roArg, PayNotifyRes &roRes, const CUserData &roUserData)
{
	roRes.set_errcode(KKSG::ERR_SUCCESS);
	if (roUserData.m_dwUserData != 0)
	{
		roRes.set_errcode((KKSG::ErrorCode)roUserData.m_dwUserData);
		return;
	}
}
