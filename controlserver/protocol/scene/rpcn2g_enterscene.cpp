#include "pch.h"
#include "scene/rpcn2g_enterscene.h"
#include "account/accountsessionmgr.h"
#include "role/rolemanager.h"
#include "logutil.h"
#include "scene/scenemanager.h"
#include "scene/gsmanager.h"

// generate by ProtoGen at date: 2016/11/3 2:26:09

RPC_CLIENT_IMPLEMETION(RpcN2G_EnterScene, EnterSceneArg, EnterSceneRes)

void RpcN2G_EnterScene::OnReply(const EnterSceneArg &roArg, const EnterSceneRes &roRes, const CUserData &roUserData)
{
	//先获取RoleList
	std::vector<CRole*> oRoleList;
	for(INT32 i = 0; i < roArg.roledatalist_size(); ++i)
	{
		const KKSG::RoleAllInfo& roInfo = roArg.roledatalist(i);
		CRole* poRole = CRoleManager::Instance()->GetByRoleID(roInfo.brief().roleid());
		if(poRole == NULL)
		{
			LogFatal("Can't find role %llu when n2g enter scene reply", roInfo.brief().roleid());
			return;
		}
		oRoleList.push_back(poRole);
	}
	if(oRoleList.empty())
	{
		LogFatal("Error, Can't find any roles when n2g enter scene reply");
		return;
	}

	if(roArg.type() == KKSG::ENTER_SCENE_SELECT_ROLE)
	{
		//成功设置上line和State
		if(roRes.result() == KKSG::ERR_SUCCESS)
		{
			for(std::vector<CRole*>::size_type i = 0; i < oRoleList.size(); ++i)
			{
				CRole* poRole = oRoleList[i];
				poRole->SetState(ROLE_STATE_IN_GAME);
				poRole->SetSceneID(roRes.sceneid());
			}
			CGsManager::Instance()->IncGsLineRoleNum(MAIN_HALL_GS_LINE, roArg.roledatalist_size());
		}
		else
		{
			//进入失败，清除Role对象
			std::stringstream ss;
			for(std::vector<CRole*>::size_type i = 0; i < oRoleList.size(); ++i)
			{
				CRole* poRole = oRoleList[i];
				ss << "[" << poRole->GetAccount() << "|" << poRole->GetID() << "] ";
				CAccountSessionMgr::Instance()->LogoutRole(poRole->GetSessionID());
			}
			LogError("Role %s enter main hall scene failed when select role, Logout the role", ss.str().c_str());
		}

		CUserData oUser;
		oUser.m_dwUserData = (UINT32)roRes.result();
		CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
	}
	else
	{
		//切场景
		UINT32 dwDestGsLine = oRoleList.front()->GetDestGsLine();
		bool isCrossGs = oRoleList.front()->GetDestIsCross();

		//enter scene 失败，中断流程，踢下线
		if(roRes.result() != KKSG::ERR_SUCCESS)
		{
			LogError("Role [%s] enter %s_%u failed, error [%d], Kickout", RoleInfoList2Str(roArg.roledatalist()).c_str(), GetGsPrefix(isCrossGs), dwDestGsLine, roRes.result());
			for(std::vector<CRole*>::size_type i = 0; i < oRoleList.size(); ++i)
			{
				CRole* poRole = oRoleList[i];
				poRole->SetState(ROLE_STATE_INVALID);
				CAccountSessionMgr::Instance()->LogoutAccount(poRole->GetAccount(), KKSG::KICK_NORMAL);
			}
			return;
		}

		for(std::vector<CRole*>::size_type i = 0; i < oRoleList.size(); ++i)
		{
			CRole* poRole = oRoleList[i];
			poRole->SetGsLine(dwDestGsLine);
			poRole->SetInCrossGs(isCrossGs);
			poRole->SetMapID(roRes.mapid());
			poRole->SetSceneID(roRes.sceneid());
			poRole->SetState(ROLE_STATE_IN_GAME);
			poRole->SetIsWatcher(poRole->GetDestIsWatcher());
			poRole->ClearSwitchInfo();
		}

		LogInfo("Role [%s] enter %s_%u success, map %u, scene %u", RoleInfoList2Str(roArg.roledatalist()).c_str(), GetGsPrefix(isCrossGs), dwDestGsLine, roArg.mapid(),
			roArg.destsceneid() == INVALID_SCENE_ID ? roArg.newsceneuniqueid() : roArg.destsceneid());

		if (!isCrossGs)
		{
			//动态创建的
			if(roArg.destsceneid() == INVALID_SCENE_ID && CSceneManager::IsDynamicScene(roArg.mapid()))
			{
				CSceneManager::Instance()->CreateScene(roArg.mapid(), roArg.newsceneuniqueid(), dwDestGsLine);
			}
			CGsManager::Instance()->IncGsLineRoleNum(dwDestGsLine, roArg.roledatalist_size());
		}
	}
}

void RpcN2G_EnterScene::OnTimeout(const EnterSceneArg &roArg, const CUserData &roUserData)
{
	//先获取RoleList
	std::vector<CRole*> oRoleList;
	for(INT32 i = 0; i < roArg.roledatalist_size(); ++i)
	{
		const KKSG::RoleAllInfo& roInfo = roArg.roledatalist(i);
		CRole* poRole = CRoleManager::Instance()->GetByRoleID(roInfo.brief().roleid());
		if(poRole == NULL)
		{
			LogError("Can't find role %llu when n2g enter scene timeout", roInfo.brief().roleid());
			return;
		}
		oRoleList.push_back(poRole);
	}
	if(oRoleList.empty())
	{
		LogError("Error, Can't find any roles when n2g enter scene timeout");
		return;
	}


	if(roArg.type() == KKSG::ENTER_SCENE_SELECT_ROLE)
	{
		//成功设置上line和State
		for(std::vector<CRole*>::size_type i = 0; i < oRoleList.size(); ++i)
		{
			CRole* poRole = oRoleList[i];
			poRole->SetState(ROLE_STATE_IN_GAME);
		}
		CGsManager::Instance()->IncGsLineRoleNum(MAIN_HALL_GS_LINE, roArg.roledatalist_size());

		CUserData oUser;
		oUser.m_dwUserData = (UINT32)KKSG::ERR_TIMEOUT;
		CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
	}
	else
	{
		//切场景
		UINT32 dwDestGsLine = oRoleList.front()->GetDestGsLine();
		bool isCrossGs = oRoleList.front()->GetDestIsCross();;

		LogWarn("Role [%s] enter %s_%u timeout", RoleInfoList2Str(roArg.roledatalist()).c_str(), GetGsPrefix(isCrossGs), dwDestGsLine);
		for(std::vector<CRole*>::size_type i = 0; i < oRoleList.size(); ++i)
		{
			CRole* poRole = oRoleList[i];
			poRole->SetGsLine(dwDestGsLine);
			poRole->SetInCrossGs(isCrossGs);
			poRole->SetMapID(roArg.mapid());
			poRole->SetIsWatcher(poRole->GetDestIsWatcher());
			if (roArg.destsceneid() == INVALID_SCENE_ID)
			{
				poRole->SetSceneID(roArg.newsceneuniqueid());
			}
			else
			{
				poRole->SetSceneID(roArg.destsceneid());
			}

			poRole->SetState(ROLE_STATE_IN_GAME);
			poRole->ClearSwitchInfo();
		}

		if (!isCrossGs)
		{
			CGsManager::Instance()->IncGsLineRoleNum(dwDestGsLine, roArg.roledatalist_size());
		}
	}
}
