#include "pch.h"
#include "npcfeeling/rpcm2g_npcflreqm2g.h"

// generate by ProtoGen at date: 2017/9/11 10:37:15

RPC_CLIENT_IMPLEMETION(RpcM2G_NpcFlReqM2G, NpcFlArgM2G, NpcFlResM2G)

void RpcM2G_NpcFlReqM2G::OnReply(const NpcFlArgM2G &roArg, const NpcFlResM2G &roRes, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_pUserPtr = (void*)(const_cast<NpcFlResM2G*>(&roRes));

	ReplyDelayRpc(roArg.delayid(), userData);		
}

void RpcM2G_NpcFlReqM2G::OnTimeout(const NpcFlArgM2G &roArg, const CUserData &roUserData)
{
}
