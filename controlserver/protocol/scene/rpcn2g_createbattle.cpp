#include "pch.h"
#include "scene/rpcn2g_createbattle.h"
#include "scene/scenemanager.h"

// generate by ProtoGen at date: 2016/11/4 10:52:51

RPC_CLIENT_IMPLEMETION(RpcN2G_CreateBattle, CreateBattleArg, CreateBattleRes)

void RpcN2G_CreateBattle::OnReply(const CreateBattleArg &roArg, const CreateBattleRes &roRes, const CUserData &roUserData)
{
	CScene* scene = NULL;
	if(roRes.result() == KKSG::ERR_SUCCESS)
	{
		scene = CSceneManager::Instance()->CreateScene(roArg.mapid(), roArg.sceneid(), roArg.line());
	}

	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.result();
	oUser.m_pUserPtr = (void*)&roArg;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcN2G_CreateBattle::OnTimeout(const CreateBattleArg &roArg, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)KKSG::ERR_TIMEOUT;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}
