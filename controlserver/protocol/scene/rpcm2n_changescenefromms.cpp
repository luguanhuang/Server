#include "pch.h"
#include "scene/rpcm2n_changescenefromms.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "scene/gsmanager.h"
#include "scene/scenemanager.h"
#include "logutil.h"
#include "scene/rpcn2g_leavescene.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "scene/gsmanager.h"
#include "scene/scenemanager.h"
#include "logutil.h"
#include "scene/rpcn2g_leavescene.h"

// generate by ProtoGen at date: 2016/11/4 18:05:42

RPC_SERVER_IMPLEMETION(RpcM2N_ChangeSceneFromMs, ChangeSceneFromMsArg, ChangeSceneFromMsRes)

void RpcM2N_ChangeSceneFromMs::OnCall(const ChangeSceneFromMsArg &roArg, ChangeSceneFromMsRes &roRes)
{
	if(roArg.rolelist_size() == 0)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	UINT32 dwCurrGsLine = INVALID_LINE_ID;
	std::vector<CRole*> oRoleList;
	for(INT32 i = 0; i < roArg.rolelist_size(); ++i)
	{
		UINT64 qwRoleID = roArg.rolelist(i);
		CRole* poRole = CRoleManager::Instance()->GetByRoleID(qwRoleID);
		if(poRole == NULL)
		{
			LogError("Can't find role %llu when g2m change scene verify", qwRoleID);
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}
		if(poRole->GetState() != ROLE_STATE_IN_GAME)
		{
			LogError("Role %llu is not in scene, can't change scene", poRole->GetID());
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}
		if(dwCurrGsLine != INVALID_LINE_ID && poRole->GetGsLine() != dwCurrGsLine)
		{
			LogError("Role %llu is not in same gs, can't change scene together", poRole->GetID());
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}

		dwCurrGsLine = poRole->GetGsLine();
		oRoleList.push_back(poRole);
	}

	//支持单人直接切换到主城
	bool bEnterMailHall = false;
	if(roArg.rolelist_size() == 1 && roArg.mapid() == HALL_MAP_ID && roArg.destsceneid() == INVALID_SCENE_ID)
	{
		bEnterMailHall = true;
	}

	// 本服创建的场景SceneManager会管理，跨服区的场景不会
	CScene* poScene = NULL;
	UINT32 dwDestLine = roArg.gsline();
	if (roArg.istocrossgs() == false)
	{
		poScene = CSceneManager::Instance()->GetScene(roArg.destsceneid());
		if(poScene == NULL)
		{
			LogWarn("Can't find scene %u, enter scene failed", roArg.destsceneid());
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}

		dwDestLine = poScene->GetLine();
	}

	RpcN2G_LeaveScene* rpc = RpcN2G_LeaveScene::CreateRpc();
	rpc->m_oArg.set_type(KKSG::LEAVE_SCENE_SWITCH);
	rpc->m_oArg.set_destline(dwDestLine);
	rpc->m_oArg.set_line(dwCurrGsLine);
	for(INT32 i = 0; i < roArg.rolelist_size(); ++i)
	{
		rpc->m_oArg.add_rolelist(roArg.rolelist(i));
	}
	oRoleList[0]->SendMsgToGS(*rpc);

	LogInfo("Role [%s] begin to change %s_%u to %s_%u", Uint64List2Str(roArg.rolelist()).c_str(),
		GetGsPrefix(oRoleList[0]->IsInCrossGs()), dwCurrGsLine, 
		GetGsPrefix(roArg.istocrossgs()), dwDestLine);

	const KKSG::SceneSwitchData& sdata = roArg.data();
	for(std::vector<CRole*>::size_type i = 0; i < oRoleList.size(); ++i)
	{
		CRole* poRole = oRoleList[i];
		bool isWatcher = false;
		for (int i = 0; i < sdata.watchers_size(); ++i)
		{
			if (sdata.watchers(i) == poRole->GetID())
			{
				isWatcher = true;
				break;
			}
		}

		poRole->SetState(ROLE_STATE_LEAVE_SCENE);
		poRole->SetDestGsLine(dwDestLine);
		poRole->SetDestMapID(roArg.mapid());
		poRole->SetDestSceneID((UINT32)roArg.destsceneid());
		poRole->SetDestIsCross(roArg.istocrossgs());
		poRole->SetDestIsWatcher(isWatcher);
		poRole->GetSwitchData().CopyFrom(roArg.data());
	}

	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcM2N_ChangeSceneFromMs::OnDelayReplyRpc(const ChangeSceneFromMsArg &roArg, ChangeSceneFromMsRes &roRes, const CUserData &roUserData)
{
}
