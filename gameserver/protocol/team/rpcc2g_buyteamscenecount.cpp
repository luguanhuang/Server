#include "pch.h"
#include "team/rpcc2g_buyteamscenecount.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/teamrequest.h"
#include "gamelogic/teammgr.h"
#include "gamelogic/team.h"
#include "team/ptcg2m_teameventnotify.h"
#include "gamelogic/teamsynextradata.h"

// generate by ProtoGen at date: 2016/5/3 15:18:40

RPC_SERVER_IMPLEMETION(RpcC2G_BuyTeamSceneCount, BuyTeamSceneCountP, BuyTeamSceneCountRet)

void RpcC2G_BuyTeamSceneCount::OnCall(const BuyTeamSceneCountP &roArg, BuyTeamSceneCountRet &roRes)
{
	roRes.set_maxcount(0);
	roRes.set_buycount(0);
	roRes.set_entercount(0);
	roRes.set_errcode(KKSG::ERR_UNKNOWN);

	//if(!GSConfig::Instance()->IsHallGS())
	//{
	//	LogWarn("Receive request not in hall gs");
	//	roRes.set_errcode(KKSG::ERR_INVALID_REQUEST);
	//	return;
	//}

	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	TeamRequest tr(pRole);
	tr.BuyTeamCount(roArg.type(), roRes);

	if(roRes.errcode() == KKSG::ERR_SUCCESS)
	{
		HTeamSynExtraData syn;
		syn.TrySynExtraDataToTs(pRole, roArg.type());
	}

	PtcG2M_TeamEventNotify ptc;
	ptc.m_Data.set_type(KKSG::TEAM_EVENT_BUYCOUNT);
	ptc.m_Data.set_roleid(pRole->GetID());
	pRole->SendToMS(ptc);
}

void RpcC2G_BuyTeamSceneCount::OnDelayReplyRpc(const BuyTeamSceneCountP &roArg, BuyTeamSceneCountRet &roRes, const CUserData &roUserData)
{
}
