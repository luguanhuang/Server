#include "pch.h"
#include "herobattle/rpcc2g_buyheroinherobattle.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/herobattlerecord.h"

// generate by ProtoGen at date: 2016/12/27 23:56:02

RPC_SERVER_IMPLEMETION(RpcC2G_BuyHeroInHeroBattle, BuyHeroInHeroBattleArg, BuyHeroInHeroBattleRes)

void RpcC2G_BuyHeroInHeroBattle::OnCall(const BuyHeroInHeroBattleArg &roArg, BuyHeroInHeroBattleRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	roRes.set_errorcode(pRole->Get<CHeroBattleRecord>()->BuyHero(roArg.heroid()));
}

void RpcC2G_BuyHeroInHeroBattle::OnDelayReplyRpc(const BuyHeroInHeroBattleArg &roArg, BuyHeroInHeroBattleRes &roRes, const CUserData &roUserData)
{
}
