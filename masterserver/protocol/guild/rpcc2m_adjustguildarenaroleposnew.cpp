#include "pch.h"
#include "guild/rpcc2m_adjustguildarenaroleposnew.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildarena.h"
#include "foreach.h"

// generate by ProtoGen at date: 2016/9/22 15:44:37

RPC_SERVER_IMPLEMETION(RpcC2M_AdjustGuildArenaRolePosNew, AdjustGuildArenaRolePosArg, AdjustGuildArenaRolePosRes)

void RpcC2M_AdjustGuildArenaRolePosNew::OnCall(const AdjustGuildArenaRolePosArg &roArg, AdjustGuildArenaRolePosRes &roRes)
{
	/*
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);

	if (pRole == NULL)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		SSWarn<<"RpcC2G_AdjustGuildArenaRolePos:: pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		SSWarn<<"RpcC2G_AdjustGuildArenaRolePos:: pGuild is NULL"<< END;
		return;
	}

	UINT32 pos = roArg.pos();
	UINT64 roleId = roArg.roleid();
	CGuildWarMgr::Instance()->Deploy(pGuild->GetID(), pRole->GetID(), roleId, pos);

	CGuildArenaTeam* pTeam = CGuildWarMgr::Instance()->GetTeam(pGuild->GetID());
	if (pTeam == NULL)
	{
		SSWarn<<"RpcC2G_AdjustGuildArenaRolePos:: pTeam is NULL"<<END;
		return;
	}

	std::map<UINT32, fightUnit> unitMap = pTeam->GetFigthRoleMap();
	foreach (i in unitMap)
	{
		KKSG::GuildDarenaUnit* pData = roRes.add_fightunits();
		pData->set_roleid(i->second.roleId);
		pData->set_state(i->second.state);
	}
	*/
}

void RpcC2M_AdjustGuildArenaRolePosNew::OnDelayReplyRpc(const AdjustGuildArenaRolePosArg &roArg, AdjustGuildArenaRolePosRes &roRes, const CUserData &roUserData)
{
}
