#include "pch.h"
#include "guild/rpcc2m_getintegralbattleinfo.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildarena.h"
#include "foreach.h"

// generate by ProtoGen at date: 2016/11/3 20:52:29

RPC_SERVER_IMPLEMETION(RpcC2M_getintegralbattleInfo, getintegralbattleInfoarg, getintegralbattleInfores)

void RpcC2M_getintegralbattleInfo::OnCall(const getintegralbattleInfoarg &roArg, getintegralbattleInfores &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		return ;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		return ;
	}

	std::map<UINT32, std::vector<IntegralBattleData> >& turnData = CGuildArenaIntegralMgr::Instance()->GetTrunData();
	foreach (i in turnData)
	{
		foreach (j in i->second)
		{
			if (j->guildOne == pGuild->GetID() || j->guildTwo == pGuild->GetID())
			{
				KKSG::IntegralBattle* pBattle = roRes.add_battleinfo();
				pBattle->set_guildone(j->guildOne);
				pBattle->set_guildonescore(j->guildOneScore);
				pBattle->set_guildtwo(j->guildTwo);
				pBattle->set_guildtwoscore(j->guildTwoScore);
				pBattle->set_isdo(j->isDo);
				pBattle->set_nameone(j->nameOne);
				pBattle->set_nametwo(j->nameTwo);
				pBattle->set_iconone(j->iconOne);
				pBattle->set_icontwo(j->iconTwo);
				pBattle->set_wartime(j->warTime);
				pBattle->set_state(j->state);
			}
		}
	}

	std::set<UINT32> timeSet;
	CGuildArenaIntegralMgr::Instance()->GetAllStartTime(timeSet);
	foreach (k in timeSet)
	{
		roRes.add_battletime(*k);
	}

}

void RpcC2M_getintegralbattleInfo::OnDelayReplyRpc(const getintegralbattleInfoarg &roArg, getintegralbattleInfores &roRes, const CUserData &roUserData)
{
}
