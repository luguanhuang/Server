#include "pch.h"
#include "guild/rpcc2g_guildgoblininfo.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guilddef.h"
#include "scene/scene.h"
#include "gamelogic/stagemgr.h"

// generate by ProtoGen at date: 2015/12/15 10:13:50

#define GUILD_GOBLIN_SCENE 2000
RPC_SERVER_IMPLEMETION(RpcC2G_GuildGoblinInfo, GuildGoblinInfoArg, GuildGoblinInfoRes)

void RpcC2G_GuildGoblinInfo::OnCall(const GuildGoblinInfoArg &roArg, GuildGoblinInfoRes &roRes)
{
	roRes.set_level(0);
	roRes.set_leftentercount(0);
	roRes.set_guildtotalkillcount(0);

	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	UINT64 gid = pRole->getGuildId();
	if (gid == INVALID_GUILDID)
	{
		return;
	}

	//哥布林删掉
}

void RpcC2G_GuildGoblinInfo::OnDelayReplyRpc(const GuildGoblinInfoArg &roArg, GuildGoblinInfoRes &roRes, const CUserData &roUserData)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is out, session id:"<<m_sessionID<<END;
		return;
	}
	UINT32 error = roUserData.m_dwUserData;
	if (error != KKSG::ERR_SUCCESS || !roUserData.m_pUserPtr)
	{
		return;
	}

	roRes.CopyFrom(*(GuildGoblinInfoRes*)roUserData.m_pUserPtr);
	StageMgr* mgr = pRole->Get<StageMgr>();
	if (!mgr)
	{
		return;
	}
	SceneInfo* info = SceneConfig::Instance()->FindSceneInfo(GUILD_GOBLIN_SCENE);
	if (!info)
	{
		return;
	}
	roRes.set_leftentercount(mgr->GetCanPlayCount(info));
}
