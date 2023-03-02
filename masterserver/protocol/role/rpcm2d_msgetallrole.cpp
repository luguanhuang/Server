#include "pch.h"
#include "role/rpcm2d_msgetallrole.h"
#include "idip/idipproc.h"

// generate by ProtoGen at date: 2016/7/21 0:18:30

RPC_CLIENT_IMPLEMETION(RpcM2D_MsGetAllRole, GetAllRoleArg, GetAllRoleRes)

void RpcM2D_MsGetAllRole::OnReply(const GetAllRoleArg &roArg, const GetAllRoleRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.result();
	oUser.m_pUserPtr = (void*)&roRes;
	//KKSG::GetAllRoleRes* pInfo = roRes.mutable_roleallinfolist();
	CIdipProcess::Instance()->OnAsyncReply(roArg.id(), oUser);
}

void RpcM2D_MsGetAllRole::OnTimeout(const GetAllRoleArg &roArg, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)KKSG::ERR_TIMEOUT;
	CIdipProcess::Instance()->OnAsyncReply(roArg.id(), oUser);
}
