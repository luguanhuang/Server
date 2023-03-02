#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "gamelogic/takecostmgr.h"
#include "worldboss/rpcm2g_worldbossguildaddattrgs.h"

// generate by ProtoGen at date: 2017/4/10 14:52:00

RPC_SERVER_IMPLEMETION(RpcM2G_WorldBossGuildAddAttrGs, WorldBossGuildAddAttrGsArg, WorldBossGuildAddAttrGsRes)

void RpcM2G_WorldBossGuildAddAttrGs::OnCall(const WorldBossGuildAddAttrGsArg &roArg, WorldBossGuildAddAttrGsRes &roRes)
{
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
		roRes.set_errorcode(ERR_FAILED);
		return;
	}
	UINT32 scenetype = scene->GetSceneType();
	if (KKSG::SCENE_WORLDBOSS != scenetype)
	{
		roRes.set_errorcode(ERR_FAILED);
		return;
	}
	WorldBossHandler* handler = scene->GetWorldBossHandler();
	if (NULL != handler)
	{
		///> 世界boss公会鼓舞限制
		if (!TakeCostManager::Instance()->TakeCost("WorldBossGuildConsume", pRole, roArg.count(), ItemFlow_Stage, ItemFlow_WorldBoss_GuildAttr))
		{
			roRes.set_errorcode(ERR_AUCT_DRAGONCOINLESS);
			return;
		}
	}
	roRes.set_errorcode(ERR_SUCCESS);
	return;
}

void RpcM2G_WorldBossGuildAddAttrGs::OnDelayReplyRpc(const WorldBossGuildAddAttrGsArg &roArg, WorldBossGuildAddAttrGsRes &roRes, const CUserData &roUserData)
{
}
