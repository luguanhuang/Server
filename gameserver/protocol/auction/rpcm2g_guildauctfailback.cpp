#include "pch.h"
#include "auction/rpcm2g_guildauctfailback.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "auction/guildauctrequest.h"

// generate by ProtoGen at date: 2016/11/3 20:15:28

RPC_SERVER_IMPLEMETION(RpcM2G_GuildAuctFailBack, GuildAuctFailArg, GuildAuctFailRes)

void RpcM2G_GuildAuctFailBack::OnCall(const GuildAuctFailArg &roArg, GuildAuctFailRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if(NULL == pRole)
	{
		roRes.set_errorcode(ERR_FAILED);
		return ;
	}
	GuildAuctRequest req(pRole);
	int ret = req.AuctFailBack(roArg.price());
	roRes.set_errorcode(ErrorCode(ret));
}

void RpcM2G_GuildAuctFailBack::OnDelayReplyRpc(const GuildAuctFailArg &roArg, GuildAuctFailRes &roRes, const CUserData &roUserData)
{
}
