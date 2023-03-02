#include "pch.h"
#include "gamelogic/arenamgr.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scene/sceneconfig.h"
#include "scene/scenemanager.h"
#include "gamelogic/stagemgr.h"
#include "table/globalconfig.h"
#include "arena/rpcm2g_startarenags.h"

// generate by ProtoGen at date: 2016/9/11 19:07:13

RPC_SERVER_IMPLEMETION(RpcM2G_StartArenaGs, StartArenaGsArg, StartArenaGsRes)

void RpcM2G_StartArenaGs::OnCall(const StartArenaGsArg &roArg, StartArenaGsRes &roRes)
{
	Role* role = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		SSWarn<<"role is null, roleid:"<<roArg.roleid()<<END;
		return;
	}
	SceneInfo* sceneinfo = SceneConfig::Instance()->FindSceneInfo( GetGlobalConfig().ArenaSceneID);
	if (NULL == sceneinfo)
	{
		//roRes.set_errorcode(KKSG::ERR_DES_HAS_ERROR);
		return;
	}
	///> 判断一下cd
	UINT32 cooldown = role->Get<StageMgr>()->GetSceneCoolDown(sceneinfo);
	if (cooldown > 0)
	{
		roRes.set_cooltime(cooldown);
		roRes.set_errorcode(KKSG::ERR_SCENE_COOLDOWN);
		return;
	}	

	if(!SceneManager::Instance()->ChangeScene(role, (UINT32) GetGlobalConfig().ArenaSceneID))
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
	}
	else
	{
		CArenaMgr::Instance()->PushBattleInfo(role->GetID(), roArg.info());
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
	}
}

void RpcM2G_StartArenaGs::OnDelayReplyRpc(const StartArenaGsArg &roArg, StartArenaGsRes &roRes, const CUserData &roUserData)
{
}
