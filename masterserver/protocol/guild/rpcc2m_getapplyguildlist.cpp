#include "pch.h"
#include "guild/rpcc2m_getapplyguildlist.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildarena.h"

#include "foreach.h"

// generate by ProtoGen at date: 2016/11/3 19:39:17

RPC_SERVER_IMPLEMETION(RpcC2M_getapplyguildlist, getapplyguildlistarg, getapplyguildlistres)

void RpcC2M_getapplyguildlist::OnCall(const getapplyguildlistarg &roArg, getapplyguildlistres &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		return ;
	}

	Guild* pGuildSelf = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuildSelf == NULL)
	{
		return ;
	}

	std::set<IntegralUnitData>& applyList = CGuildArenaIntegralMgr::Instance()->GetApplyList();
	foreach (i in applyList)
	{
		Guild* pGuild = CGuildMgr::Instance()->GetGuild(i->guildId);
		if (pGuild != NULL)
		{
			KKSG::Integralunit*pUnit = roRes.add_guildlist();
			pUnit->set_guildid(i->guildId);
			pUnit->set_guildscore(i->guildScore);
			pUnit->set_name(pGuild->GetName());
			pUnit->set_guildicon(pGuild->GetIcon());
		}
		
	}

}

void RpcC2M_getapplyguildlist::OnDelayReplyRpc(const getapplyguildlistarg &roArg, getapplyguildlistres &roRes, const CUserData &roUserData)
{
}
