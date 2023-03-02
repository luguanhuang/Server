#include "pch.h"
#include "sceneswitch.h"
#include "role/role.h"
#include "network/gslink.h"
#include "scene/scenemanager.h"
#include "scene/rpcn2g_leavescene.h"
#include "scene/rpcn2g_enterscene.h"
#include "account/accountsessionmgr.h"
#include "network/rslink.h"
#include "logutil.h"


bool CSceneSwitch::EnterScene(CRole* poRole, UINT32 dwSceneInstanceID, const KKSG::SceneSwitchData& roData, UINT32 gsLine, bool isCrossGs)
{
	if(poRole == NULL)
	{
		return false;
	}

	if(poRole->GetState() != ROLE_STATE_IN_GAME)
	{
		LogWarn("Role[%llu] not in state game, can't enter scene", poRole->GetID());
		return false;
	}

	CScene* poScene = NULL;
	// 本服创建的场景SceneManager会管理，跨服区的场景不会
	if (isCrossGs == false)
	{
		poScene = CSceneManager::Instance()->GetScene(dwSceneInstanceID);
		if(poScene == NULL)
		{
			LogWarn("Can't find scene %u, enter scene failed", dwSceneInstanceID);
			return false;
		}
	}
	bool isWatcher = false;
	for (int i = 0; i < roData.watchers_size(); ++i)
	{
		if (roData.watchers(i) == poRole->GetID())
		{
			isWatcher = true;
			break;
		}
	}

	UINT32 destGsLine = poScene ? poScene->GetLine() : gsLine;

	poRole->SetState(ROLE_STATE_LEAVE_SCENE);
	poRole->SetDestGsLine(destGsLine);
	poRole->SetDestMapID(poScene ? poScene->GetMapID() : 0);
	poRole->SetDestSceneID(dwSceneInstanceID);
	poRole->SetDestIsCross(isCrossGs);
	poRole->SetDestIsWatcher(isWatcher);
	poRole->GetSwitchData().CopyFrom(roData);

	RpcN2G_LeaveScene* rpc = RpcN2G_LeaveScene::CreateRpc();
	rpc->m_oArg.set_type(KKSG::LEAVE_SCENE_SWITCH);
	rpc->m_oArg.set_line(poRole->GetGsLine());
	rpc->m_oArg.set_destline(destGsLine);
	rpc->m_oArg.add_rolelist(poRole->GetID());
	poRole->SendMsgToGS(*rpc);

	LogInfo("Role [%llu] begin to change %s_%u to %s_%u", poRole->GetID(), GetGsPrefix(poRole->IsInCrossGs()), poRole->GetGsLine(), GetGsPrefix(isCrossGs), destGsLine);
	return true;
}

bool CSceneSwitch::EnterMainHall(CRole* poRole)
{
	if(poRole == NULL)
	{
		return false;
	}

	if(poRole->GetState() != ROLE_STATE_IN_GAME)
	{
		return false;
	}

	//if(poRole->GetGsLine() == MAIN_HALL_GS_LINE)
	//{
	//	return false;
	//}

	poRole->SetState(ROLE_STATE_LEAVE_SCENE);
	poRole->SetDestGsLine(MAIN_HALL_GS_LINE);
	poRole->SetDestMapID(HALL_MAP_ID);
	poRole->SetDestSceneID(INVALID_SCENE_ID);
	poRole->SetDestIsCross(false);
	poRole->SetDestIsWatcher(false);

	RpcN2G_LeaveScene* rpc = RpcN2G_LeaveScene::CreateRpc();
	rpc->m_oArg.set_type(KKSG::LEAVE_SCENE_SWITCH);
	rpc->m_oArg.set_line(poRole->GetGsLine());
	rpc->m_oArg.set_destline(MAIN_HALL_GS_LINE);
	rpc->m_oArg.add_rolelist(poRole->GetID());
	poRole->SendMsgToGS(*rpc);

	LogInfo("Role [%llu] begin to change gs_%u to gs_%u", poRole->GetID(), poRole->GetGsLine(), MAIN_HALL_GS_LINE);
	return true;
}