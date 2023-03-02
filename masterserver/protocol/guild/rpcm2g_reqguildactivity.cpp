#include "pch.h"
#include "guild/rpcm2g_reqguildactivity.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildactivity.h"

// generate by ProtoGen at date: 2016/9/16 16:40:03

RPC_CLIENT_IMPLEMETION(RpcM2G_ReqGuildActivity, ReqGuildActivityAgr, ReqGuildActivityRes)

void RpcM2G_ReqGuildActivity::OnReply(const ReqGuildActivityAgr &roArg, const ReqGuildActivityRes &roRes, const CUserData &roUserData)
{

}

void RpcM2G_ReqGuildActivity::OnTimeout(const ReqGuildActivityAgr &roArg, const CUserData &roUserData)
{
}
