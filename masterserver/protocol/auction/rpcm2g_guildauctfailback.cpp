#include "pch.h"
#include "auction/rpcm2g_guildauctfailback.h"
#include "guildauct/guildauctrequest.h"

// generate by ProtoGen at date: 2016/11/3 20:15:28

RPC_CLIENT_IMPLEMETION(RpcM2G_GuildAuctFailBack, GuildAuctFailArg, GuildAuctFailRes)

	void RpcM2G_GuildAuctFailBack::OnReply(const GuildAuctFailArg &roArg, const GuildAuctFailRes &roRes, const CUserData &roUserData)
{
	if(ERR_SUCCESS != roRes.errorcode())
	{
		GuildAuctRequest req;
		req.AuctFailMailBack(roArg.roleid(), roArg.itemid(), roArg.price());			
	}
}

void RpcM2G_GuildAuctFailBack::OnTimeout(const GuildAuctFailArg &roArg, const CUserData &roUserData)
{
	GuildAuctRequest req;
	req.AuctFailMailBack(roArg.roleid(), roArg.itemid(), roArg.price());	
}
