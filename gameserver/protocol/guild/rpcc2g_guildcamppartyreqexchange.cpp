#include "pch.h"
#include "guild/rpcc2g_guildcamppartyreqexchange.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/MultActivityMgr.h"
#include "guild/guildcampparty.h"
#include "scene/scene.h"
// generate by ProtoGen at date: 2017/4/26 19:08:18

RPC_SERVER_IMPLEMETION(RpcC2G_GuildCampPartyReqExchange, GuildCampPartyReqExchangeReq, GuildCampPartyReqExchangeRes)

	void RpcC2G_GuildCampPartyReqExchange::OnCall(const GuildCampPartyReqExchangeReq &roArg, GuildCampPartyReqExchangeRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);	
	if (NULL == role  )
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;	
		return;
	}

	Role* target_role = RoleManager::Instance()->FindByRoleID(roArg.role_id());
	if( NULL == target_role)
	{
		SSWarn<<"pRole is NULL, role id:"<<roArg.role_id()<<END;
		roRes.set_result(KKSG::ERR_ROLE_NOT_ONLINE);
		return;
	}
	
	KKSG::ErrorCode result = CGuildCampPartyMgr::Instance()->CheckPartyConditon(role);
	if ( result != KKSG::ERR_SUCCESS)
	{
		roRes.set_result(result);
		return ;
	}
	result = CGuildCampPartyMgr::Instance()->CheckPartyConditon(target_role);
	if ( result != KKSG::ERR_SUCCESS)
	{
		roRes.set_result(result);
		return ;
	}	

	if (role->getGuildId() != target_role->getGuildId())
	{
		SSWarn<<"not the same guild"<<END;
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);		
		return;
	}

	CGuildCampPartyMgr::Instance()->ExchangeItemReq(role,target_role);
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2G_GuildCampPartyReqExchange::OnDelayReplyRpc(const GuildCampPartyReqExchangeReq &roArg, GuildCampPartyReqExchangeRes &roRes, const CUserData &roUserData)
{
}
