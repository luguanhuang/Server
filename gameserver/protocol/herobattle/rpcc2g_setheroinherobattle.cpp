#include "pch.h"
#include "herobattle/rpcc2g_setheroinherobattle.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/scenebase.h"
#include "scene/sceneherobattle.h"
#include "gamelogic/herobattlerecord.h"
#include "scene/scenemorphinfo.h"
#include "scene/scenemobabattle.h"

// generate by ProtoGen at date: 2016/12/21 21:21:01

RPC_SERVER_IMPLEMETION(RpcC2G_SetHeroInHeroBattle, SetHeroInHeroBattleArg, SetHeroInHeroBattleRes)

void RpcC2G_SetHeroInHeroBattle::OnCall(const SetHeroInHeroBattleArg &roArg, SetHeroInHeroBattleRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	if (pRole->GetCurrScene() == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	std::string ss = "Unknown";
	KKSG::ErrorCode code = KKSG::ERR_FAILED;
	if (pRole->GetCurrScene()->GetHeroBattleHandler())
	{
		SceneHeroBattle* battle = pRole->GetCurrScene()->GetHeroBattleHandler();
		code = battle->SetHero(pRole->GetID(), roArg.heroid());
		ss = "HeroBattle";
	}
	else if (pRole->GetCurrScene()->GetMobaBattleHandler())
	{
		SceneMobaBattle* battle = pRole->GetCurrScene()->GetMobaBattleHandler();
		code = battle->SetHero(pRole->GetID(), roArg.heroid());
		ss = "MobaBattle";
	}
	else if (pRole->GetCurrScene()->GetSceneMorphInfo()->CanMorph())
	{
		code = pRole->GetCurrScene()->GetSceneMorphInfo()->SetHero(pRole->GetID(), roArg.heroid());
		ss = "SceneMorph";
	}

	roRes.set_errorcode(code);

	LogInfo("role [%llu] choose hero [%u] in %s, errorcode:%d", pRole->GetID(), roArg.heroid(), ss.c_str(), code);
}

void RpcC2G_SetHeroInHeroBattle::OnDelayReplyRpc(const SetHeroInHeroBattleArg &roArg, SetHeroInHeroBattleRes &roRes, const CUserData &roUserData)
{
}
