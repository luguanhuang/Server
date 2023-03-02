#include "pch.h"
#include "worldboss/rpcm2g_worldbossguildaddattrgs.h"

// generate by ProtoGen at date: 2017/4/10 14:52:00

RPC_CLIENT_IMPLEMETION(RpcM2G_WorldBossGuildAddAttrGs, WorldBossGuildAddAttrGsArg, WorldBossGuildAddAttrGsRes)

void RpcM2G_WorldBossGuildAddAttrGs::OnReply(const WorldBossGuildAddAttrGsArg &roArg, const WorldBossGuildAddAttrGsRes &roRes, const CUserData &roUserData)
{
	CUserData data;
	data.m_pUserPtr = (void*)&roArg;
	data.m_dwUserData = roRes.errorcode();
	CRpc::ReplyDelayRpc(roArg.delayid(), data);	
}

void RpcM2G_WorldBossGuildAddAttrGs::OnTimeout(const WorldBossGuildAddAttrGsArg &roArg, const CUserData &roUserData)
{
	CUserData data;
	data.m_pUserPtr = (void*)&roArg;
	data.m_dwUserData = KKSG::ERR_TIMEOUT;
	CRpc::ReplyDelayRpc(roArg.delayid(), data);	
}
