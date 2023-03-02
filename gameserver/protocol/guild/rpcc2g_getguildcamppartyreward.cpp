#include "pch.h"
#include "guild/rpcc2g_getguildcamppartyreward.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildcampparty.h"
// generate by ProtoGen at date: 2017/5/5 11:55:07

RPC_SERVER_IMPLEMETION(RpcC2G_GetGuildCampPartyReward, GetGuildCampPartyRewardArg, GetGuildCampPartyRewardRes)

void RpcC2G_GetGuildCampPartyReward::OnCall(const GetGuildCampPartyRewardArg &roArg, GetGuildCampPartyRewardRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	KKSG::ErrorCode result = CGuildCampPartyMgr::Instance()->CheckPartyConditon(role);
	if ( result != KKSG::ERR_SUCCESS)
	{
		roRes.set_result(result);
		return ;
	}
	

	roRes.set_result(CGuildCampPartyMgr::Instance()->GetReward(role,roArg.reward_id()));
}

void RpcC2G_GetGuildCampPartyReward::OnDelayReplyRpc(const GetGuildCampPartyRewardArg &roArg, GetGuildCampPartyRewardRes &roRes, const CUserData &roUserData)
{
}
