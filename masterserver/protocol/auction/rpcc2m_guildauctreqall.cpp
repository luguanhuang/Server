#include "pch.h"
#include "auction/rpcc2m_guildauctreqall.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "guildauct/guildauctrequest.h"
#include "network/dblink.h"
#include "auction/rpcm2g_guildauctreqm2c.h"
#include "network/gslink.h"
#include "guildauct/guildauctmgr.h"
#include "util/gametime.h"
#include "table/globalconfig.h"
#include "define/systemiddef.h"
#include "table/OpenSystemMgr.h"

// generate by ProtoGen at date: 2016/11/1 16:07:35

RPC_SERVER_IMPLEMETION(RpcC2M_GuildAuctReqAll, GuildAuctReqArg, GuildAuctReqRes)

void RpcC2M_GuildAuctReqAll::OnCall(const GuildAuctReqArg &roArg, GuildAuctReqRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return; 
	}

	if (ROLE_STATE_IN_GAME != pRole->GetState())
	{
		roRes.set_errorcode(ERR_FAILED);
		return ;
	}

	if(!DBLink::Instance()->IsConnected())//db err
	{
		SSWarn << " db connect err " << END;
		roRes.set_errorcode(ERR_FAILED);
		return;
	}
	
	// 系统开放
	if (!pRole->IsSystemOpened(SYS_AUCTION) || OpenSystemMgr::Instance()->IsClosedSystem(SYS_AUCTION))
	{	
		roRes.set_errorcode(KKSG::ERR_SYS_NOTOPEN);
		return ;
	}
	
	// 竞价和拍卖前的公示时间检查
	if (GART_BUY_AUCT == roArg.reqtype() || GART_BUY_NOW == roArg.reqtype())
	{
		auto data = GuildAuctMgr::Instance()->GetGuildAuctData(roArg.uid());
		if(NULL == data || data->IsSaled())
		{
			roRes.set_errorcode(KKSG::ERR_AUCT_ITEMSALED);// 已出售
			return ;
		}
		// 工会拍卖行才需要公示时间
		if (data->GetSaleType() == GAST_GUILD_AUCT_ING)
		{		
			time_t now = GameTime::GetTime();
			if (now < GetGlobalConfig().GuildAuctGuildItemPublicityTime + data->GetGwTime())
			{
				roRes.set_errorcode(KKSG::ERR_GUILDAUCT_PUBLIC_TIME);
				return;
			}		
		}
	}	

	GuildAuctRequest req(pRole);

	UINT32 price = 0;
	bool gotogs = false;
	int ret = ERR_SUCCESS;

	switch(roArg.reqtype())
	{
	case GART_ACT_TYPE:
		{
			ret = req.HandleActList(roArg.acttype(), roRes);
		}
		break;
	case GART_ITEM_TYPE:
		{
			ret = req.HandleItemList(roArg.itemtype(), roRes);
		}
		break;
	case GART_BUY_AUCT:
		{
			ret = req.HandleBuyAuctPre(roArg.uid(), roArg.curauctprice(), price, roRes);	
			gotogs = true;
		}
		break;
	case GART_BUY_NOW:
		{
			ret = req.HandleBuyNowPre(roArg.uid(), price);
			gotogs = true;
		}
		break;
	case GART_AUCT_GUILD_HISTORY:
		{
			ret = req.HandleAuctGuildHistory(roRes);
		}
		break;
	case GART_AUCT_WORLD_HISTORY:
		{
			ret = req.HandleAuctWorldHistory(roRes);
		}
		break;
	default:
		break;
	}

	if(false == gotogs || ERR_SUCCESS != ret)
	{
		roRes.set_errorcode(ErrorCode(ret));
		req.HandleCommonData(roArg, roRes);
	}
	else
	{
		UINT32 delayid = DelayRpc();
		RpcM2G_GuildAuctReqM2C* rpc = RpcM2G_GuildAuctReqM2C::CreateRpc();
		rpc->m_oArg.mutable_c2marg()->CopyFrom(roArg);
		rpc->m_oArg.set_delayid(delayid);
		rpc->m_oArg.set_sessionid(m_sessionID);
		rpc->m_oArg.set_price(price);
		GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc);
	}
}

void RpcC2M_GuildAuctReqAll::OnDelayReplyRpc(const GuildAuctReqArg &roArg, GuildAuctReqRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return ;
	}
	if(NULL == roUserData.m_pUserPtr)
	{
		roRes.set_errorcode(ERR_FAILED);
		return ;
	}

	GuildAuctReqM2GRes* m2gRes = (GuildAuctReqM2GRes*)(roUserData.m_pUserPtr);
	roRes.CopyFrom(m2gRes->c2mres());

	GuildAuctRequest req(pRole);

	if(roArg.reqtype() == GART_BUY_AUCT || roArg.reqtype() == GART_BUY_NOW)
	{
		req.ClearLock(roArg.uid());
	}

	if(ERR_SUCCESS != roRes.errorcode())
	{
		req.HandleCommonData(roArg, roRes);
		return;
	}
	int ret = ERR_SUCCESS;

	switch(roArg.reqtype())
	{
	case GART_BUY_AUCT:
		{
			ret = req.HandleBuyAuctAft(roArg.uid(), roRes);
		}
		break;
	case GART_BUY_NOW:
		{
			ret = req.HandleBuyNowAft(roArg.uid());
		}
		break;
	default:
		break;
	}

	roRes.set_errorcode(ErrorCode(ret));
	req.HandleCommonData(roArg, roRes);
}
