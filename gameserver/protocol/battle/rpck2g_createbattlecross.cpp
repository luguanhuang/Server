#include "pch.h"
#include "battle/rpck2g_createbattlecross.h"
#include "scene/scenemanager.h"

// generate by ProtoGen at date: 2016/10/29 14:35:19

RPC_SERVER_IMPLEMETION(RpcK2G_CreateBattleCross, CreateBattleCrossArg, CreateBattleCrossRes)

void RpcK2G_CreateBattleCross::OnCall(const CreateBattleCrossArg &roArg, CreateBattleCrossRes &roRes)
{
	Scene* poScene = SceneManager::Instance()->CreateScene(roArg.map_id(), roArg.scene_id(), roArg.param());
	roRes.set_result(poScene != NULL ? KKSG::ERR_SUCCESS : KKSG::ERR_FAILED);
}

void RpcK2G_CreateBattleCross::OnDelayReplyRpc(const CreateBattleCrossArg &roArg, CreateBattleCrossRes &roRes, const CUserData &roUserData)
{
}
