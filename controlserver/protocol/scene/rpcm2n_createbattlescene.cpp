#include "pch.h"
#include "scene/rpcm2n_createbattlescene.h"
#include "scene/scenemanager.h"
#include "network/gslink.h"
#include "scene/rpcn2g_createbattle.h"
#include "scene/gsmanager.h"

// generate by ProtoGen at date: 2016/11/4 10:52:58

RPC_SERVER_IMPLEMETION(RpcM2N_CreateBattleScene, CreateBattleSceneArg, CreateBattleSceneRes)

void RpcM2N_CreateBattleScene::OnCall(const CreateBattleSceneArg &roArg, CreateBattleSceneRes &roRes)
{
	UINT32 dwDestLine = roArg.destline() == INVALID_LINE_ID ? CGsManager::Instance()->GetGsLine(roArg.createtype()) : roArg.destline();
	if(dwDestLine == INVALID_LINE_ID)
	{
		LogError("Can't find the minimal load gs line");
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	UINT32 dwNewSceneUID = CSceneManager::Instance()->NewID();
	RpcN2G_CreateBattle* rpc = RpcN2G_CreateBattle::CreateRpc();
	rpc->m_oArg.set_uid(0);
	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_mapid(roArg.mapid());
	rpc->m_oArg.set_sceneid(dwNewSceneUID);
	rpc->m_oArg.set_line(dwDestLine);
	rpc->m_oArg.mutable_param()->CopyFrom(roArg.param());
	GSLink::Instance()->SendToLine(dwDestLine, *rpc);
}

void RpcM2N_CreateBattleScene::OnDelayReplyRpc(const CreateBattleSceneArg &roArg, CreateBattleSceneRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);
	roRes.set_sceneinstanceid(0);
	if(nResult == KKSG::ERR_SUCCESS)
	{
		if(roUserData.m_pUserPtr != NULL)
		{
			CreateBattleArg* roBattleArg = (CreateBattleArg*)(roUserData.m_pUserPtr);
			roRes.set_sceneinstanceid(roBattleArg->sceneid());
			roRes.set_sceneline(roBattleArg->line());
		}
	}
}
