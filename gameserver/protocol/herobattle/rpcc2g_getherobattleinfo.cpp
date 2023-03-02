#include "pch.h"
#include "herobattle/rpcc2g_getherobattleinfo.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/sceneswitch.h"
#include "gamelogic/herobattlerecord.h"

// generate by ProtoGen at date: 2016/12/13 16:36:04

RPC_SERVER_IMPLEMETION(RpcC2G_GetHeroBattleInfo, GetHeroBattleInfoArg, GetHeroBattleInfoRes)

void RpcC2G_GetHeroBattleInfo::OnCall(const GetHeroBattleInfoArg &roArg, GetHeroBattleInfoRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	pRole->Get<CHeroBattleRecord>()->FillHeroBattleInfo(m_oRes);
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcC2G_GetHeroBattleInfo::OnDelayReplyRpc(const GetHeroBattleInfoArg &roArg, GetHeroBattleInfoRes &roRes, const CUserData &roUserData)
{
}
