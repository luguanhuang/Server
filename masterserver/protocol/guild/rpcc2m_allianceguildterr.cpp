#include "pch.h"
#include "guild/rpcc2m_allianceguildterr.h"
#include "common/rpcm2g_msconsumeitems.h"
#include "guild/guildterritorybattle.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "define/tlogenum.h"
#include "network/gslink.h"
#include "table/globalconfig.h"

// generate by ProtoGen at date: 2016/12/18 10:40:00

RPC_SERVER_IMPLEMETION(RpcC2M_AllianceGuildTerr, AllianceGuildTerrArg, AllianceGuildTerrRes)

void RpcC2M_AllianceGuildTerr::OnCall(const AllianceGuildTerrArg &roArg, AllianceGuildTerrRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		roRes.set_errorcod(ERR_UNKNOWN);
		return ;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		SSWarn << "guild is NULL and roleid is :" << pRole->GetID() <<END;
		roRes.set_errorcod(ERR_UNKNOWN);
		return ;
	}

	if (!CGuildTerritoryBattle::Instance()->IsDeclareTime())
	{
		roRes.set_errorcod(ERR_DECLAREWAR_OUT_TIME);
		return ;
	}

	if (!CGuildTerritoryBattle::Instance()->CanDeclareWar(pRole->GetID(), roArg.id()))
	{
		roRes.set_errorcod(ERR_UNKNOWN);
		return ;
	}

	// 去gs扣龙币
	RpcM2G_MSConsumeItems* msg = RpcM2G_MSConsumeItems::CreateRpc();
	msg->m_oArg.set_rpc_id(DelayRpc());
	msg->m_oArg.set_role_id(pRole->GetID());
	msg->m_oArg.set_reason(ItemFlow_GuildTerritory);
	msg->m_oArg.set_subreason(ItemFlow_GuildTerrChall);
	KKSG::ItemBrief* item = msg->m_oArg.add_items();
	item->set_itemid(GetGlobalConfig().GuildTerritoryCost.seq[0]);
	item->set_itemcount(GetGlobalConfig().GuildTerritoryCost.seq[1]);

	GSLink::Instance()->SendToLine(pRole->GetGsLine(), *msg);	
}

void RpcC2M_AllianceGuildTerr::OnDelayReplyRpc(const AllianceGuildTerrArg &roArg, AllianceGuildTerrRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		roRes.set_errorcod(ERR_UNKNOWN);
		return ;
	}

	if (roUserData.m_dwUserData == ERR_SUCCESS)
	{
		CGuildTerritoryBattle::Instance()->DeclareWar(pRole->GetID(), roArg.id());
	}

	roRes.set_errorcod((KKSG::ErrorCode)roUserData.m_dwUserData);
	return ;
}
