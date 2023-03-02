#include "pch.h"
#include "network/gslink.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "worldboss/worldbossmgr.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/worldbossranklist.h"
#include "ranklist/worldbossguildranklist.h"
#include "worldboss/ptcg2m_worldbosssyncms.h"
#include "worldboss/ptcm2g_worldbosssyncgs.h"

// generate by ProtoGen at date: 2016/8/8 11:43:47

void PtcG2M_WorldBossSyncMS::Process(UINT32 dwConnID)
{
	if (m_Data.has_timenode())
	{
		WorldBossMgr::Instance()->ResetAll(m_Data.mutable_timenode()->beginhour(), m_Data.mutable_timenode()->beginmin()
										, m_Data.mutable_timenode()->endhour(), m_Data.mutable_timenode()->endmin());
	}
	if (m_Data.has_isbossdead())
	{
		WorldBossMgr::Instance()->OnDied();
	}
	if (m_Data.has_damage())
	{
		if (!WorldBossMgr::Instance()->IsSceneOpen())
		{
			SSWarn<<"world boss end"<<END;
			return;
		}
		BossDamageRankList* ranklist = (BossDamageRankList*)CRankListMgr::Instance()->GetRankList(KKSG::WorldBossDamageRank);
		WorldBossGuildRankList* guildranklist = (WorldBossGuildRankList*)CRankListMgr::Instance()->GetRankList(KKSG::WorldBossGuildRank);
		if (NULL == ranklist || NULL == guildranklist)
		{
			SSError<<"rank list NULL"<<END;
			return;
		}
		KKSG::WorldBossRoleDamage* damages = m_Data.mutable_damage();
		// rank
		float subhp = 0.0f;
		for (int i = 0; i < damages->roleid_size(); ++i)
		{
			//SSDebug<<"roleid:"<<damages->roleid(i)<<" damage:"<<damages->damage(i)<<END;

			float temphp = damages->damage(i);
			subhp += temphp;
			ranklist->AddDamage(damages->roleid(i), temphp);
			Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(damages->roleid(i));
			if (NULL != guild)
			{
				guildranklist->AddDamage(guild->GetID(), damages->roleid(i), temphp);	
			}
		}

		WorldBossMgr::Instance()->SubHp(subhp);
		// sync hp
		PtcM2G_WorldBossSyncGS sync;
		sync.m_Data.mutable_scenedamage()->set_sceneid(m_Data.sceneid());
		sync.m_Data.mutable_scenedamage()->set_damage(subhp);
		GSLink::Instance()->SendToAllLine(sync);
	}
}
