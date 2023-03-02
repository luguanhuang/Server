#include "pch.h"
#include "guild/rpcc2g_getguildcamppartyexchangeinfo.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildcampparty.h"
#include "gamelogic/MultActivityMgr.h"

// generate by ProtoGen at date: 2017/5/2 14:02:09

RPC_SERVER_IMPLEMETION(RpcC2G_GetGuildCampPartyExchangeInfo, GetGuildCampPartyExchangeInfoArg, GetGuildCampPartyExchangeInfoRes)

void RpcC2G_GetGuildCampPartyExchangeInfo::OnCall(const GetGuildCampPartyExchangeInfoArg &roArg, GetGuildCampPartyExchangeInfoRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	/*if(!MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_GUILDCAMP_PARTY))
	{
		roRes.set_result(KKSG::ERR_GUILDACMPPATY_NOTOPEN);
		return;
	}
*/
	KKSG::ErrorCode result = CGuildCampPartyMgr::Instance()->CheckPartyConditon(role);
	if ( result != KKSG::ERR_SUCCESS)
	{
		roRes.set_result(result);
		if (result == KKSG::ERR_INVFIGHT_OTHER_SCENE)
		{
			//roRes.set_result(KKSG::ERR_GUILDCAMP_SELF_NOTIN);
			roRes.set_result(KKSG::ERR_SUCCESS);
			CGuildCampPartyMgr::Instance()->GetExchangeInfoByTargeId(role->GetRoleID(),roRes);
		}

		return ;
	}
	
	CGuildCampPartyMgr::Instance()->GetExchangeInfoByTargeId(role->GetRoleID(),roRes);
}

void RpcC2G_GetGuildCampPartyExchangeInfo::OnDelayReplyRpc(const GetGuildCampPartyExchangeInfoArg &roArg, GetGuildCampPartyExchangeInfoRes &roRes, const CUserData &roUserData)
{
}
