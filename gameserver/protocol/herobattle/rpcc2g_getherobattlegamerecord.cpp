#include "pch.h"
#include "herobattle/rpcc2g_getherobattlegamerecord.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/herobattlerecord.h"

// generate by ProtoGen at date: 2016/12/27 15:37:44

RPC_SERVER_IMPLEMETION(RpcC2G_GetHeroBattleGameRecord, GetHeroBattleGameRecordArg, GetHeroBattleGameRecordRes)

void RpcC2G_GetHeroBattleGameRecord::OnCall(const GetHeroBattleGameRecordArg &roArg, GetHeroBattleGameRecordRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	pRole->Get<CHeroBattleRecord>()->FillHeroBattleGameRecord(roRes);
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcC2G_GetHeroBattleGameRecord::OnDelayReplyRpc(const GetHeroBattleGameRecordArg &roArg, GetHeroBattleGameRecordRes &roRes, const CUserData &roUserData)
{
}
