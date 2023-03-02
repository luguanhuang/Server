#include "pch.h"
#include "pay/rpcc2g_buygoldandfatigue.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/buyexchangemgr.h"
#include "define/itemdef.h"

// generate by ProtoGen at date: 2015/7/20 10:15:31

RPC_SERVER_IMPLEMETION(RpcC2G_BuyGoldAndFatigue, BuyGoldAndFatigueArg, BuyGoldAndFatigueRes)

void RpcC2G_BuyGoldAndFatigue::OnCall(const BuyGoldAndFatigueArg &roArg, BuyGoldAndFatigueRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	roRes.set_result(KKSG::ERR_UNKNOWN);
	if (pRole == NULL)
	{
		return;
	}

	int ret = 0;
	switch(roArg.type())
	{
	case DIAMOND_BUY_FATIGUE:
		{
			//ret = BuyExchangeMgr::Instance()->BuyFatigue(pRole, DIAMOND, roArg.fatigueid());
		}
		break;
	case DIAMONE_BUY_DRAGONCOIN:
		{
			ret = BuyExchangeMgr::Instance()->BuyDragonCoinFunPre(pRole, DIAMOND, (roArg.count() == 0  || roArg.count() == 1) ? 1 : 10);	
		}
		break;
	case DIAMONE_BUY_GOLD:
		{
			//ret = BuyExchangeMgr::Instance()->BuyGold(pRole, DIAMOND);
		}
		break;
	case DRAGON_BUY_FATIGUE:
		{
			ret = BuyExchangeMgr::Instance()->BuyFatigue(pRole, DRAGON_COIN, roArg.fatigueid());
		}
		break;
	case DRAGONCOIN_BUY_GOLD:
		{
			ret = BuyExchangeMgr::Instance()->BuyGold(pRole, DRAGON_COIN);
		}
		break;
	case DRAGONCOIN_BUY_BLUEBIRD:
		{
			ret = BuyExchangeMgr::Instance()->BuyBlueBird(pRole, DRAGON_COIN);
		}
		break;
	case DIAMOND_EXCHANGE_DRAGONCOIN:
		{
			ret = BuyExchangeMgr::Instance()->ExchangeDiamond(pRole, roArg.count());
		}
		break;
	default:
		break;
	}
	roRes.set_result((KKSG::ErrorCode)ret);
	
}

void RpcC2G_BuyGoldAndFatigue::OnDelayReplyRpc(const BuyGoldAndFatigueArg &roArg, BuyGoldAndFatigueRes &roRes, const CUserData &roUserData)
{
}
