#include "pch.h"
#include "role/rpcc2n_createrolenew.h"
#include "account/accountsessionmgr.h"
#include "role/rpcn2m_checkrolename.h"
#include "network/mslink.h"
#include "account/logincontrol.h"

// generate by ProtoGen at date: 2016/11/3 2:20:02

RPC_SERVER_IMPLEMETION(RpcC2N_CreateRoleNew, CreateRoleNewArg, CreateRoleNewRes)

void RpcC2N_CreateRoleNew::OnCall(const CreateRoleNewArg &roArg, CreateRoleNewRes &roRes)
{
	KKSG::ErrorCode nErrCode = CAccountSessionMgr::Instance()->BeginCreateRole(m_sessionID, roArg.type(), roArg.name());
	if(nErrCode != KKSG::ERR_SUCCESS)
	{
		roRes.set_result(nErrCode);
		return;
	}

	CAccountSession* poAccount = CAccountSessionMgr::Instance()->GetBySession(m_sessionID);
	if(poAccount == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	RpcN2M_CheckRoleName* rpc = RpcN2M_CheckRoleName::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_name(roArg.name());
	rpc->m_oArg.set_type(roArg.type());
	MSLink::Instance()->SendTo(*rpc);

	CLoginControl::Instance()->LockName(roArg.name());
}

void RpcC2N_CreateRoleNew::OnDelayReplyRpc(const CreateRoleNewArg &roArg, CreateRoleNewRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	KKSG::RoleAllInfo* poRoleInfo = (KKSG::RoleAllInfo*)roUserData.m_pUserPtr;
	roRes.set_result(nResult);
	if(nResult == KKSG::ERR_SUCCESS && poRoleInfo != NULL)
	{
		*roRes.mutable_roledata() = *poRoleInfo;
	}
}
