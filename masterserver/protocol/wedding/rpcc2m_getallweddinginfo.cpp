#include "pch.h"
#include "wedding/rpcc2m_getallweddinginfo.h"
#include "marriage/weddingmgr.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2017/7/17 17:58:46

RPC_SERVER_IMPLEMETION(RpcC2M_GetAllWeddingInfo, GetAllWeddingInfoArg, GetAllWeddingInfoRes)

void RpcC2M_GetAllWeddingInfo::OnCall(const GetAllWeddingInfoArg &roArg, GetAllWeddingInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	WeddingMgr::Instance()->FillAllWeddingInfo(pRole->GetID(), roRes);
}

void RpcC2M_GetAllWeddingInfo::OnDelayReplyRpc(const GetAllWeddingInfoArg &roArg, GetAllWeddingInfoRes &roRes, const CUserData &roUserData)
{
}
