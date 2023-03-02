#include "pch.h"
#include "common/rpcc2g_addtempattr.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "table/globalconfig.h"
#include "table/PowerPointMgr.h"
#include "gamelogic/takecostmgr.h"
#include "guild/guild.h"
#include "guild/guildboss.h"
#include "guild/guildmgr.h"
#include "scene/sceneworldboss.h"
#include "guild/ptcg2c_notifyguildbossaddattr.h"
#include "gamelogic/noticemgr.h"
#include "guild/ptcg2m_synguildbossattr.h"
#include "mslink.h"

// generate by ProtoGen at date: 2015/7/21 16:18:27

RPC_SERVER_IMPLEMETION(RpcC2G_AddTempAttr, AddTempAttrArg, AddTempAttrRes)

void RpcC2G_AddTempAttr::OnCall(const AddTempAttrArg &roArg, AddTempAttrRes &roRes)
{
	roRes.set_errorcode(ERR_UNKNOWN);

	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	Scene* scene = pRole->GetCurrScene();
	if (NULL == scene)
	{
		return;
	}
	UINT32 scenetype = scene->GetSceneType();
	if (0 == roArg.type() && KKSG::SCENE_WORLDBOSS == scenetype)
	{
		WorldBossHandler* handler = scene->GetWorldBossHandler();
		if (NULL != handler)
		{
			///> 世界boss鼓舞限制
			UINT32 dwCount = handler->GetAddAttrCount(pRole->GetID());
			if (dwCount >= GetGlobalConfig().WorldBossAttrCount)
			{
				roRes.set_errorcode(ERR_TEAMBUY_COUNT_MAX);
				return;
			}
			if (!TakeCostManager::Instance()->TakeCost("WorldBossConsume", pRole, dwCount, ItemFlow_Stage, ItemFlow_Stage_WorldBoss))
			{
				roRes.set_errorcode(ERR_AUCT_DRAGONCOINLESS);
				return;
			}
		
			dwCount = handler->PushAddAttrCount(pRole);
			roRes.set_count(dwCount);
		
			for (auto i =  GetGlobalConfig().WorldBossAddAttr.begin(); i !=  GetGlobalConfig().WorldBossAddAttr.end(); ++i)
			{
				//float coef = dwCount >= GetGlobalConfig().WorldBossAttrCount ? 5.5f : 3.0f;
				float coef = 1.0f;
				PowerPointMgr::Instance()->AdjustOneAttributeByCoeffcient(pRole, i->seq[0], (float)(i->seq[1]*coef)/100.0f);
			}
		}
		roRes.set_errorcode(ERR_SUCCESS);
		return;
	}
	else if (1 == roArg.type() && KKSG::SCENE_GUILD_BOSS == scenetype)
	{
		Scene* scene = pRole->GetCurrScene();
		if (NULL == scene)
		{
			return;
		}

		UINT32 cdError = GuildBossMgr::Instance()->IsInCDtime(pRole->getGuildId());
		if (cdError != ERR_SUCCESS)
		{
			roRes.set_errorcode((KKSG::ErrorCode)cdError);
			return;
		}


		UINT32 dwCount = GuildBossMgr::Instance()->GetAttrCount(pRole->getGuildId());
		if (!TakeCostManager::Instance()->TakeCost("GuildBossConsume", pRole, dwCount, ItemFlow_Stage, ItemFlow_Stage_GuildBoss))
		{
			roRes.set_errorcode(ERR_AUCT_DRAGONCOINLESS);
			return;
		}
	
		dwCount = GuildBossMgr::Instance()->AddAttrCount(pRole->getGuildId());
		roRes.set_count(dwCount);

		Notice notice(GuildBossInspire);
		notice.Add(pRole);

		const std::list<Role*>& roles = scene->GetAllRoles();
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			Role* role = *i;
			notice.SendSelf(role);
		}	
		
		roRes.set_errorcode(ERR_SUCCESS);
		PtcG2M_SynGuildBossAttr msg;
		msg.m_Data.set_guildid(pRole->getGuildId());
		msg.m_Data.set_sceneid(scene->GetSceneID());
		MSLink::Instance()->Send(MSLink::Instance()->GetConnID(), msg);
		return;
	}
	roRes.set_errorcode(ERR_UNKNOWN);
}

void RpcC2G_AddTempAttr::OnDelayReplyRpc(const AddTempAttrArg &roArg, AddTempAttrRes &roRes, const CUserData &roUserData)
{
}
