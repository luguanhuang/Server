#include "pch.h"
#include "common/rpcc2g_dhrreqc2g.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/dragonhelpconfig.h"
#include "gamelogic/teamrecord.h"
#include "define/teamdef.h"

// generate by ProtoGen at date: 2017/6/14 21:17:07

RPC_SERVER_IMPLEMETION(RpcC2G_DHRReqC2G, DHRArg, DHRRes)

void RpcC2G_DHRReqC2G::OnCall(const DHRArg &roArg, DHRRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	if(!pRole->Get<CRoleSystem>()->IsSystemOpened(SysDragonHelpReward))
	{
		roRes.set_errorcode(ERR_FAILED);
		return;
	}
	ErrorCode ret = ERR_SUCCESS;
	switch(roArg.op())
	{
	case DHR_OP_FETCH_REWARD:
		{
			ret = DragonHelpConfig::Instance()->HandleFetchReward(pRole, roArg.id());	
		}
		break;
	case DHR_OP_WANT_BE_HELP:
		{
			pRole->Get<CTeamRecord>()->SetWantDragonHelp(true);
		}
		break;
	case DHR_OP_WANT_NOT_HELP:
		{
			pRole->Get<CTeamRecord>()->SetWantDragonHelp(false);
		}
		break;
	default:
		break;
	}

	DragonHelpConfig::Instance()->FillRewardState(pRole, roRes);
	roRes.set_helpcount(pRole->Get<CTeamRecord>()->GetHelpCount(DRAGON_TEAM_SCENE));
	roRes.set_wanthelp(pRole->Get<CTeamRecord>()->IsWantDragonHelp());

	roRes.set_errorcode(ret);
}

void RpcC2G_DHRReqC2G::OnDelayReplyRpc(const DHRArg &roArg, DHRRes &roRes, const CUserData &roUserData)
{
}
