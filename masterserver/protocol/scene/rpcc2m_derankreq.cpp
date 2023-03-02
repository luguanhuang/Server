#include "pch.h"
#include "scene/rpcc2m_derankreq.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "table/DragonExpRankMgr.h"

// generate by ProtoGen at date: 2016/9/3 17:33:58

RPC_SERVER_IMPLEMETION(RpcC2M_DERankReq, DERankArg, DERankRes)

void RpcC2M_DERankReq::OnCall(const DERankArg &roArg, DERankRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errcode(ERR_INVALID_REQUEST);
		return ;
	}
	
	roRes.set_errcode(ERR_SUCCESS);
	DragonExpRankMgr::Instance()->FillRankData(pRole->GetID(), roRes);
}

void RpcC2M_DERankReq::OnDelayReplyRpc(const DERankArg &roArg, DERankRes &roRes, const CUserData &roUserData)
{
}
