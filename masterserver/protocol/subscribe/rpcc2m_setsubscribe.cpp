#include "pch.h"
#include "subscribe/rpcc2m_setsubscribe.h"
#include "role/rolemanager.h"
#include "httptask/setsubscribe.h"
#include "httptask/httptaskmgr.h"
#include "push/pushsubscribemgr.h"

// generate by ProtoGen at date: 2017/1/10 14:44:49

RPC_SERVER_IMPLEMETION(RpcC2M_SetSubscribe, SetSubscirbeArg, SetSubscribeRes)

void RpcC2M_SetSubscribe::OnCall(const SetSubscirbeArg &roArg, SetSubscribeRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}
	KKSG::SetSubscirbeArg oData;
	oData.CopyFrom(roArg);
	oData.clear_msgid();
	UINT32 TxMsgId = -1;
	for (INT32 i = 0; i < roArg.msgid_size(); i ++)
	{
		TxMsgId = CPushSubscribeMgr::Instance()->GetTxMsgId(roArg.msgid(i));
		if (TxMsgId == UINT32(-1))
		{
			roRes.set_result(ERR_FAILED);
			return;
		}
		oData.add_msgid(TxMsgId);
	}

	CSetSubscribeTask* poTask = new CSetSubscribeTask();
	poTask->SetRpcId(DelayRpc());
	poTask->SetToken(roArg.token());
	poTask->SetOpenId(role->GetAccount());
	poTask->BuildPostData(oData);
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void RpcC2M_SetSubscribe::OnDelayReplyRpc(const SetSubscirbeArg &roArg, SetSubscribeRes &roRes, const CUserData &roUserData)
{
	roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
}
