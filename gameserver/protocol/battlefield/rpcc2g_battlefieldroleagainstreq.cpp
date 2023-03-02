#include "pch.h"
#include "battlefield/rpcc2g_battlefieldroleagainstreq.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "scene/battlefieldfight.h"




// generate by ProtoGen at date: 2017/9/5 15:12:33

RPC_SERVER_IMPLEMETION(RpcC2G_BattleFieldRoleAgainstReq, BattleFieldRoleAgainstArg, BattleFieldRoleAgainst)

void RpcC2G_BattleFieldRoleAgainstReq::OnCall(const BattleFieldRoleAgainstArg &roArg, BattleFieldRoleAgainst &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
		return;
	Scene *pScene = pRole->GetCurrScene();
	if (pScene == NULL)
		return;
	SceneBFFight*  pSceneBfFight = pScene->GetBFFightHandler();
	if (pSceneBfFight)
	{
		pSceneBfFight->GetAginstInfo(roRes);
	}
}

void RpcC2G_BattleFieldRoleAgainstReq::OnDelayReplyRpc(const BattleFieldRoleAgainstArg &roArg, BattleFieldRoleAgainst &roRes, const CUserData &roUserData)
{
}
