#include "pch.h"
#include "scene/rpcg2n_changesceneverify.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "scene/gsmanager.h"
#include "scene/scenemanager.h"
#include "logutil.h"
#include "scene/rpcn2g_leavescene.h"

// generate by ProtoGen at date: 2016/11/3 11:17:51

RPC_SERVER_IMPLEMETION(RpcG2N_ChangeSceneVerify, ChangeSceneVerifyArg, ChangeSceneVerifyRes)

void RpcG2N_ChangeSceneVerify::OnCall(const ChangeSceneVerifyArg &roArg, ChangeSceneVerifyRes &roRes)
{
	if(roArg.rolelist_size() == 0)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	std::vector<CRole*> oRoleList;
	for(INT32 i = 0; i < roArg.rolelist_size(); ++i)
	{
		UINT64 qwRoleID = roArg.rolelist(i);
		CRole* poRole = CRoleManager::Instance()->GetByRoleID(qwRoleID);
		if(poRole == NULL)
		{
			LogError("Can't find role %llu when g2n change scene verify", qwRoleID);
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}
		if(poRole->GetState() != ROLE_STATE_IN_GAME)
		{
			LogError("Role %llu is not in scene, can't change scene", poRole->GetID());
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}

		oRoleList.push_back(poRole);
	}


	UINT32 dwDestLine = INVALID_LINE_ID;

	if (!roArg.isdestcross())
	{
		if(roArg.destsceneid() == INVALID_SCENE_ID)
		{
			if(CSceneManager::IsDynamicScene(roArg.mapid()))
			{
				dwDestLine = CGsManager::Instance()->GetMinimalLoadGsLine();
				if(dwDestLine == INVALID_LINE_ID)
				{
					LogError("Can't find the minimal load gs line");
					roRes.set_result(KKSG::ERR_FAILED);
					return;
				}
			}
			else
			{
				dwDestLine = MAIN_HALL_GS_LINE;
			}
		}
		else
		{
			CScene* poScene = CSceneManager::Instance()->GetScene((UINT32)roArg.destsceneid());
			if(poScene == NULL)
			{
				LogError("Can't find dest scene %u", roArg.destsceneid());
				roRes.set_result(KKSG::ERR_FAILED);
				return;
			}
			dwDestLine = poScene->GetLine();
		}

		if(CGsManager::Instance()->GetGsInfo(dwDestLine) == NULL)
		{
			roRes.set_result(KKSG::ERR_FAILED);
			LogError("Role [%s] begin to change gs_%u to gs_%u failed, target gs was closed", Uint64List2Str(roArg.rolelist()).c_str(), roArg.line(), dwDestLine);
			return;
		}
	}
	else
		dwDestLine = roArg.destline();

	roRes.set_destline(dwDestLine);
	roRes.set_result(KKSG::ERR_SUCCESS);

	{
		RpcN2G_LeaveScene* rpc = RpcN2G_LeaveScene::CreateRpc();
		rpc->m_oArg.set_type(KKSG::LEAVE_SCENE_SWITCH);
		rpc->m_oArg.set_destline(dwDestLine);
		rpc->m_oArg.set_line(roArg.line());
		for(INT32 i = 0; i < roArg.rolelist_size(); ++i)
		{
			rpc->m_oArg.add_rolelist(roArg.rolelist(i));
		}
		oRoleList[0]->SendMsgToGS(*rpc);

		if (oRoleList[0]->IsInCrossGs())
		{
			LogInfo("Role [%s] begin to change cross_gs_%u to gs_%u", Uint64List2Str(roArg.rolelist()).c_str(), roArg.line(), dwDestLine);
		}
		else
		{
			LogInfo("Role [%s] begin to change gs_%u to gs_%u", Uint64List2Str(roArg.rolelist()).c_str(), roArg.line(), dwDestLine);
		}	
	}
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
		//poRole->SetDestIsCross(false);
		poRole->SetDestIsCross(roArg.isdestcross());
		poRole->SetDestIsWatcher(isWatcher);
		poRole->GetSwitchData().CopyFrom(roArg.data());
	}
}

void RpcG2N_ChangeSceneVerify::OnDelayReplyRpc(const ChangeSceneVerifyArg &roArg, ChangeSceneVerifyRes &roRes, const CUserData &roUserData)
{
}
