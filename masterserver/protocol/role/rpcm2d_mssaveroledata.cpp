#include "pch.h"
#include "role/rpcm2d_mssaveroledata.h"
#include "idip/idipproc.h"

// generate by ProtoGen at date: 2016/7/18 11:08:11

RPC_CLIENT_IMPLEMETION(RpcM2D_MsSaveRoleData, SaveRoleDataArg, SaveRoleDataRes)

void RpcM2D_MsSaveRoleData::OnReply(const SaveRoleDataArg &roArg, const SaveRoleDataRes &roRes, const CUserData &roUserData)
{
	CUserData data;
	data.m_pUserPtr = (void*)&roRes;
	CIdipProcess::Instance()->OnAsyncReply(roArg.param(), data);
}

void RpcM2D_MsSaveRoleData::OnTimeout(const SaveRoleDataArg &roArg, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)KKSG::ERR_TIMEOUT;
	CIdipProcess::Instance()->OnAsyncReply(roArg.param(), oUser);

}
