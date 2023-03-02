#include "pch.h"
#include "guild/rpcm2g_enterguildpower.h"

// generate by ProtoGen at date: 2016/8/29 17:36:36

RPC_CLIENT_IMPLEMETION(RpcM2G_EnterGuildPower, EnterGuildPowerArg, EnterGuildPowerRes)

void RpcM2G_EnterGuildPower::OnReply(const EnterGuildPowerArg &roArg, const EnterGuildPowerRes &roRes, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.roleattr();
	UINT32 errorcode = roRes.errorcode(); 
	oUser.m_pUserPtr = (void*)&errorcode;
	//oUser.m_dwUserData = roRes.errorcode();
	/*
	bool isCreate = roArg.iscreate(); 
	oUser.m_pUserPtr = (void*)&isCreate;
	oUser.m_dwUserData = roRes.errorcode();
	*/
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcM2G_EnterGuildPower::OnTimeout(const EnterGuildPowerArg &roArg, const CUserData &roUserData)
{
}
