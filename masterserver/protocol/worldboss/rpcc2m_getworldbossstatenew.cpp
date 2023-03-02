#include "pch.h"
#include "worldboss/worldbossmgr.h"
#include "worldboss/rpcc2m_getworldbossstatenew.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildboss.h"
#include "guild/guildmgr.h"


// generate by ProtoGen at date: 2016/8/8 19:24:07

RPC_SERVER_IMPLEMETION(RpcC2M_GetWorldBossStateNew, GetWorldBossStateArg, GetWorldBossStateRes)

void RpcC2M_GetWorldBossStateNew::OnCall(const GetWorldBossStateArg &roArg, GetWorldBossStateRes &roRes)
{
	if (roArg.type() == 0)
	{
		BossInfo* info = WorldBossMgr::Instance()->GetBossInfo();
		roRes.set_bossid(NULL == info ? 0 : info->dwEnemyId);

		bool isdead = WorldBossMgr::Instance()->IsBossDead();
		if (isdead)
		{
			roRes.set_bosshp(0);
			roRes.set_timeleft(0);
			return;
		}
		if (WorldBossMgr::Instance()->IsSceneOpen())
		{
			roRes.set_timeleft(0);
		}
		else
		{
			UINT32 timeleft = WorldBossMgr::Instance()->GetBeginTimeLeft();
			roRes.set_timeleft(timeleft);
		}
		roRes.set_bosshp(1);
	}
	else if (roArg.type() == 1)
	{
		CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
		if (pRole == NULL)
		{
			return ;
		}
		Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
		if (pGuild == NULL || !MultActivityMgr::Instance()->IsGuildBossOpen(pGuild->GetLevel()))
		{
			return ;
		}

		if (CGuildBossMgr::Instance()->IsSceneOpen(CGuildMgr::Instance()->GetGuildIdByRoleId(pRole->GetID())))
		{
			roRes.set_timeleft(0);
		}
		else
		{
			UINT32 timeleft = CGuildBossMgr::Instance()->GetBeginTimeLeft();
			roRes.set_timeleft(timeleft);
		}

		if (pRole != NULL)
		{
			Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
			if (pGuild != NULL)
			{
				if (CGuildBossMgr::Instance()->IsFinish(pGuild->GetID()) && MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_GUILD_BOSS))
				{
					roRes.set_bosshp(0);
				}
				else
				{
					roRes.set_bosshp(1);
				}
			}
			else
			{
				roRes.set_bosshp(1);
			}
		}
		else
		{
			roRes.set_bosshp(1);
		}
	}
}

void RpcC2M_GetWorldBossStateNew::OnDelayReplyRpc(const GetWorldBossStateArg &roArg, GetWorldBossStateRes &roRes, const CUserData &roUserData)
{
}
