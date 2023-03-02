#include "pch.h"
#include "auction/rpcm2g_guildauctreqm2c.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "auction/guildauctrequest.h"

// generate by ProtoGen at date: 2016/11/1 23:24:13

RPC_SERVER_IMPLEMETION(RpcM2G_GuildAuctReqM2C, GuildAuctReqM2GArg, GuildAuctReqM2GRes)

void RpcM2G_GuildAuctReqM2C::OnCall(const GuildAuctReqM2GArg &roArg, GuildAuctReqM2GRes &roRes)
{
	const GuildAuctReqArg& c2mArg = (roArg.c2marg());
	GuildAuctReqRes& c2mRes = (*roRes.mutable_c2mres());

	Role* pRole = RoleManager::Instance()->FindBySession(roArg.sessionid());
	if(NULL == pRole)
	{
		c2mRes.set_errorcode(ERR_FAILED);
		return ;
	}

	GuildAuctRequest req(pRole);

	int ret = ERR_SUCCESS;

	switch(c2mArg.reqtype())
	{
	case GART_BUY_AUCT:
	case GART_BUY_NOW:
		{
			ret = req.HandleBuy(roArg.price());
		}
		break;
	default:
		break;
	}

	c2mRes.set_errorcode(ErrorCode(ret));
}

void RpcM2G_GuildAuctReqM2C::OnDelayReplyRpc(const GuildAuctReqM2GArg &roArg, GuildAuctReqM2GRes &roRes, const CUserData &roUserData)
{
}
