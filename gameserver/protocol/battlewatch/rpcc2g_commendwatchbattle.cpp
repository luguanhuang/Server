#include "pch.h"
#include "battlewatch/rpcc2g_commendwatchbattle.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "live/livemanager.h"

// generate by ProtoGen at date: 2016/7/11 16:35:56

RPC_SERVER_IMPLEMETION(RpcC2G_CommendWatchBattle, CommendWatchBattleArg, CommendWatchBattleRes)

void RpcC2G_CommendWatchBattle::OnCall(const CommendWatchBattleArg &roArg, CommendWatchBattleRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role || !role->GetCurrScene())
	{
		roRes.set_error(ERR_INVALID_REQUEST);
		return;
	}

	UINT32 error = LiveManager::Instance()->Commend(role);

	roRes.set_error((ErrorCode)error);
}

void RpcC2G_CommendWatchBattle::OnDelayReplyRpc(const CommendWatchBattleArg &roArg, CommendWatchBattleRes &roRes, const CUserData &roUserData)
{
}
