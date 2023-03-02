#include "pch.h"
#include "scene/rpcn2g_createbattle.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"

// generate by ProtoGen at date: 2016/11/4 10:52:51

RPC_SERVER_IMPLEMETION(RpcN2G_CreateBattle, CreateBattleArg, CreateBattleRes)

void RpcN2G_CreateBattle::OnCall(const CreateBattleArg &roArg, CreateBattleRes &roRes)
{
	Scene* poScene = SceneManager::Instance()->CreateScene(roArg.mapid(), roArg.sceneid(), roArg.param());
	roRes.set_result(poScene != NULL ? KKSG::ERR_SUCCESS : KKSG::ERR_FAILED);
}

void RpcN2G_CreateBattle::OnDelayReplyRpc(const CreateBattleArg &roArg, CreateBattleRes &roRes, const CUserData &roUserData)
{
}
