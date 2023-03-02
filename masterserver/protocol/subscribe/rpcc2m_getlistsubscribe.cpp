#include "pch.h"
#include "subscribe/rpcc2m_getlistsubscribe.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "httptask/getlistsubscribe.h"
#include "httptask/httptaskmgr.h"

// generate by ProtoGen at date: 2017/1/10 14:37:54

RPC_SERVER_IMPLEMETION(RpcC2M_GetListSubscribe, GetListSubscribeArg, GetListSubscribeRes)

void RpcC2M_GetListSubscribe::OnCall(const GetListSubscribeArg &roArg, GetListSubscribeRes &roRes)
{
	
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}
	CGetListSubscribeTask* poTask = new CGetListSubscribeTask();
	poTask->SetRpcId(DelayRpc());
	poTask->SetToken(roArg.token());
	poTask->SetOpenId(role->GetAccount());
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
	
}

void RpcC2M_GetListSubscribe::OnDelayReplyRpc(const GetListSubscribeArg &roArg, GetListSubscribeRes &roRes, const CUserData &roUserData)
{
	roRes.set_result(KKSG::ERR_SUCCESS);
	if (roUserData.m_dwUserData != 0)
	{
		roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
	}
	if (roUserData.m_pUserPtr)
	{
		KKSG::GetListSubscribeRes* pRes = (KKSG::GetListSubscribeRes*)roUserData.m_pUserPtr;
		roRes.CopyFrom(*pRes);
	}
}
