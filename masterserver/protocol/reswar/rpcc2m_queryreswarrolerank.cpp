#include "pch.h"
#include "reswar/rpcc2m_queryreswarrolerank.h"
#include "reswar/reswarmgr.h"
#include "role/rolemanager.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
// generate by ProtoGen at date: 2016/11/7 20:10:17

RPC_SERVER_IMPLEMETION(RpcC2M_QueryResWarRoleRank, ResWarRoleRankArg, ResWarRoleRankRes)

void RpcC2M_QueryResWarRoleRank::OnCall(const ResWarRoleRankArg &roArg, ResWarRoleRankRes &roRes)
{
	CRole *pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!pRole)
	{
		return;
	}
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (!pGuild)
	{
		return;
	}
	UINT64 nGuildID = pGuild->GetID();
	GuildTeam*pGuildTeam = ResWarMgr::Instance()->GetGuildTeam(nGuildID);
	if (!pGuildTeam || !pGuildTeam->pGroupGuildInfo) 
		return;
	UINT32 nGroupID = pGuildTeam->pGroupGuildInfo->nGroupID;
	ResWarMgr::Instance()->SortFightRoleLess(nGroupID);
	ResWarMgr::Instance()->QueryFightRank(nGroupID,roRes);

}

void RpcC2M_QueryResWarRoleRank::OnDelayReplyRpc(const ResWarRoleRankArg &roArg, ResWarRoleRankRes &roRes, const CUserData &roUserData)
{
}
