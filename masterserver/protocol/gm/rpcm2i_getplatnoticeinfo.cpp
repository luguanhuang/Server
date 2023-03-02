#include "pch.h"
#include "gm/rpcm2i_getplatnoticeinfo.h"
#include "idip/idipproc.h"

// generate by ProtoGen at date: 2016/11/19 14:54:48

RPC_CLIENT_IMPLEMETION(RpcM2I_GetPlatNoticeInfo, GetPlatNoticeArg, GetPlatNoticeRes)

void RpcM2I_GetPlatNoticeInfo::OnReply(const GetPlatNoticeArg &roArg, const GetPlatNoticeRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.result();
	oUser.m_pUserPtr = (void*)&roRes;
	CIdipProcess::Instance()->OnAsyncReply(roArg.id(), oUser);
}

void RpcM2I_GetPlatNoticeInfo::OnTimeout(const GetPlatNoticeArg &roArg, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)KKSG::ERR_TIMEOUT;
	CIdipProcess::Instance()->OnAsyncReply(roArg.id(), oUser);
}
