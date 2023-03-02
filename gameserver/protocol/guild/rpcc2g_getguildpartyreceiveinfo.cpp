#include "pch.h"
#include "guild/rpcc2g_getguildpartyreceiveinfo.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildcampparty.h"
#include "guild/guildrecord.h"

// generate by ProtoGen at date: 2017/5/11 10:13:19

RPC_SERVER_IMPLEMETION(RpcC2G_GetGuildPartyReceiveInfo, GetGuildPartyReceiveInfoArg, GetGuildPartyReceiveInfoRes)

void RpcC2G_GetGuildPartyReceiveInfo::OnCall(const GetGuildPartyReceiveInfoArg &roArg, GetGuildPartyReceiveInfoRes &roRes)
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

	std::map<UINT32,UINT32> reward_info;
	role->Get<CGuildRecord>()->GetPartyRewardInfo(reward_info);
	for (auto it = reward_info.begin(); it != reward_info.end(); ++it)
	{
		KKSG::MapIntItem* item = roRes.add_receives();
		item->set_key(it->first);
		item->set_value(it->second);
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2G_GetGuildPartyReceiveInfo::OnDelayReplyRpc(const GetGuildPartyReceiveInfoArg &roArg, GetGuildPartyReceiveInfoRes &roRes, const CUserData &roUserData)
{
}
