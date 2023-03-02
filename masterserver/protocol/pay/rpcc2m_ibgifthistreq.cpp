#include "pch.h"
#include "pay/rpcc2m_ibgifthistreq.h"
#include "pay/ibgiftmgr.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2017/4/17 17:25:57

RPC_SERVER_IMPLEMETION(RpcC2M_IbGiftHistReq, IBGiftHistAllItemArg, IBGiftHistAllItemRes)

void RpcC2M_IbGiftHistReq::OnCall(const IBGiftHistAllItemArg &roArg, IBGiftHistAllItemRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	
	IBGiftMgr::Instance()->GetHistItem(role->GetID(),roArg.type(),roRes.mutable_gift());
}

void RpcC2M_IbGiftHistReq::OnDelayReplyRpc(const IBGiftHistAllItemArg &roArg, IBGiftHistAllItemRes &roRes, const CUserData &roUserData)
{
}
