#include "pch.h"
#include "role/rpcg2d_readhiddenroles.h"

// generate by ProtoGen at date: 2017/1/12 17:26:14

RPC_CLIENT_IMPLEMETION(RpcG2D_ReadHiddenRoles, ReadHiddenRolesArg, ReadHiddenRolesRes)

void RpcG2D_ReadHiddenRoles::OnReply(const ReadHiddenRolesArg &roArg, const ReadHiddenRolesRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)KKSG::ERR_SUCCESS;
	oUser.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcG2D_ReadHiddenRoles::OnTimeout(const ReadHiddenRolesArg &roArg, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)KKSG::ERR_TIMEOUT;
	oUser.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}
