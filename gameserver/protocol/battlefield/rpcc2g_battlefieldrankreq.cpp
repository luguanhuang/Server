#include "pch.h"
#include "battlefield/rpcc2g_battlefieldrankreq.h"
#include "scene/battlefieldfight.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "scene/battlefieldfight.h"

// generate by ProtoGen at date: 2017/9/4 15:27:16

RPC_SERVER_IMPLEMETION(RpcC2G_BattleFieldRankReq, BattleFieldRankArg, BattleFieldRankRes)

void RpcC2G_BattleFieldRankReq::OnCall(const BattleFieldRankArg &roArg, BattleFieldRankRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
		return;
	Scene *pScene = pRole->GetCurrScene();
	if (pScene == NULL)
	{
		LogDebug("battle field  pScene is  null");
		return;
	}
	SceneBFFight*  pSceneBfFight = pScene->GetBFFightHandler();
	if (pSceneBfFight)
	{
		pSceneBfFight->GetRank(roRes);
	}else
	{
		LogDebug("battle field  pSceneBfFight is  null");
	}

}

void RpcC2G_BattleFieldRankReq::OnDelayReplyRpc(const BattleFieldRankArg &roArg, BattleFieldRankRes &roRes, const CUserData &roUserData)
{
}
