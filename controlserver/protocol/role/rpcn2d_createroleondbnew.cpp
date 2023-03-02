#include "pch.h"
#include "role/rpcn2d_createroleondbnew.h"
#include "account/accountsessionmgr.h"
#include "account/logincontrol.h"

// generate by ProtoGen at date: 2016/11/3 1:56:57

RPC_CLIENT_IMPLEMETION(RpcN2D_CreateRoleOnDBNew, CreateRoleOnDBNewArg, CreateRoleOnDBNewRes)

void RpcN2D_CreateRoleOnDBNew::OnReply(const CreateRoleOnDBNewArg &roArg, const CreateRoleOnDBNewRes &roRes, const CUserData &roUserData)
{
	//1. set nick id
	if(roRes.result() == KKSG::ERR_SUCCESS)
	{
		KKSG::RoleAllInfo* poRoleInfo = const_cast<CreateRoleOnDBNewArg&>(roArg).mutable_roledata();
		poRoleInfo->mutable_brief()->set_nickid(roRes.nickid());
	}

	//2. reply to client
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.result();
	oUser.m_pUserPtr   = const_cast<CreateRoleOnDBNewArg&>(roArg).mutable_roledata();
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);

	//3. send new role to ms
	CLoginControl::Instance()->UnlockName(roArg.roledata().brief().name());
	if(roRes.result() == KKSG::ERR_SUCCESS)
	{
		CAccountSessionMgr::Instance()->OnCreateRoleReply(m_sessionID, roRes.result(), roArg.roledata());
	}
}

void RpcN2D_CreateRoleOnDBNew::OnTimeout(const CreateRoleOnDBNewArg &roArg, const CUserData &roUserData)
{
	CUserData oUserData;
	oUserData.m_dwUserData = ERR_TIMEOUT;
	oUserData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUserData);

	//不能UnLockName，防止DB超时，名字会被占用
	CAccountSessionMgr::Instance()->OnCreateRoleTimeout(m_sessionID);
}
