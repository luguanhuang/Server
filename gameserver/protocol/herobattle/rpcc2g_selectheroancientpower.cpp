#include "pch.h"
#include "herobattle/rpcc2g_selectheroancientpower.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "scene/sceneherobattle.h"

// generate by ProtoGen at date: 2017/7/15 14:57:38

RPC_SERVER_IMPLEMETION(RpcC2G_SelectHeroAncientPower, SelectHeroAncientPowerArg, SelectHeroAncientPowerRes)

void RpcC2G_SelectHeroAncientPower::OnCall(const SelectHeroAncientPowerArg &roArg, SelectHeroAncientPowerRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	if (pRole->GetCurrScene() == NULL || pRole->GetCurrScene()->GetHeroBattleHandler() == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	SceneHeroBattle* battle = pRole->GetCurrScene()->GetHeroBattleHandler();
	roRes.set_errorcode(battle->SelectHeroAncientPower(pRole, roArg.selectpower()));
}

void RpcC2G_SelectHeroAncientPower::OnDelayReplyRpc(const SelectHeroAncientPowerArg &roArg, SelectHeroAncientPowerRes &roRes, const CUserData &roUserData)
{
}
