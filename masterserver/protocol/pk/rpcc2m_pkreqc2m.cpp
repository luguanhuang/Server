#include "pch.h"
#include "pk/rpcc2m_pkreqc2m.h"
#include "pk/rpcm2g_pkreqm2g.h"
#include "network/gslink.h"
#include "commondef.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2016/8/25 20:14:43

RPC_SERVER_IMPLEMETION(RpcC2M_PkReqC2M, PkReqArg, PkReqRes)

void RpcC2M_PkReqC2M::OnCall(const PkReqArg &roArg, PkReqRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;	
	}

	RpcM2G_PkReqM2G* rpc = RpcM2G_PkReqM2G::CreateRpc();
	UINT32 delayid = DelayRpc();
	rpc->m_oArg.set_delayid(delayid);
	rpc->m_oArg.set_sessionid(m_sessionID);
	rpc->m_oArg.mutable_c2marg()->CopyFrom(roArg);
	pRole->SendMsgToGS(*rpc);
}

void RpcC2M_PkReqC2M::OnDelayReplyRpc(const PkReqArg &roArg, PkReqRes &roRes, const CUserData &roUserData)
{
	if(NULL == roUserData.m_pUserPtr)
	{
		roRes.set_errorcode(ERR_FAILED);
		return ;
	}
	
	PkReqRes* tmp = (PkReqRes*)roUserData.m_pUserPtr;
	roRes.CopyFrom(*tmp);

}
