#include "pch.h"
#include "scene/rpcn2g_enterscene.h"
#include "role/ptcg2c_selectrolentf.h"
#include "unit/rolemanager.h"
#include "unit/roleinfoinit.h"
#include "network/gatelink.h"
#include "config.h"
#include "common/ptcg2t_changegsline.h"
#include "scene/ptcg2m_rolechangescenentf.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "logutil.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildboss.h"
#include "network/mslink.h"
#include "crossscene/ptcg2k_roleentercrossscenentf.h"
#include "network/worldlink.h"
#include "login/ptcg2m_rolelogin.h"
#include "table/OpenSystemMgr.h"
#include "unit/selectrolestatemgr.h"

// generate by ProtoGen at date: 2016/7/10 14:10:02

RPC_SERVER_IMPLEMETION(RpcN2G_EnterScene, EnterSceneArg, EnterSceneRes)

void RemoveRoleOnFailed(const std::vector<Role*>& roRoleList)
{
	if(roRoleList.empty()) return;

	std::stringstream ss;
	for(std::vector<Role*>::size_type i = 0; i < roRoleList.size(); ++i)
	{
		Role* poRole = roRoleList[i];
		ss << poRole->GetID() << " ";

		UINT64 qwSessionID = poRole->GetSessionID();
		RoleManager::Instance()->RemoveBySession(qwSessionID);
		SelectRoleStateMgr::Instance()->RemoveSessionState(qwSessionID);
	}

	LogWarn("Remove role [%s] on enter scene failed", ss.str().c_str());
}

void SetRoleCrossData(Role* poRole, const EnterSceneArg& roArg)
{
	if (NULL == poRole)
	{
		return;
	}
	for (int i = 0; i < roArg.switchdata().crossdata_size(); ++i)
	{
		bool flag = false;
		const auto& crossData = roArg.switchdata().crossdata(i);
		if (crossData.roleid() == poRole->GetID())
		{
			poRole->SetCrossData(crossData);
			flag = true;
			break;
		}
		if (!flag)
		{
			SSError << " have not cross data roleid = " << poRole->GetID() << END;
		}
	}

}

void RpcN2G_EnterScene::OnCall(const EnterSceneArg &roArg, EnterSceneRes &roRes)
{
	if(roArg.roledatalist_size() == 0)
	{
		LogError("empty enter scene role list");
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	if(roArg.roledatalist_size() != roArg.rolesessionlist_size())
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	//check session exist?
	for(INT32 i = 0; i < roArg.rolesessionlist_size(); ++i)
	{
		UINT64 qwSessionID = roArg.rolesessionlist(i);
		Role* role = RoleManager::Instance()->FindBySession(qwSessionID);
		if(role != NULL)
		{
			LogError("Role '%s' with sessioin %llu has been in gs, can't enter scene", role->GetName().c_str(), qwSessionID);
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}
	}

	///> simple construct XComponent
	UINT32 dwSceneTemplateID = roArg.mapid();
	EnterSceneInfo entersceneinfo = SceneConfig::Instance()->GetEnterSceneInfo(roArg.type(), dwSceneTemplateID);

	//Create role
	std::vector<Role*>  oRoleList;
	for(INT32 i = 0; i < roArg.roledatalist_size(); ++i)
	{
		UINT64 qwSessionID = roArg.rolesessionlist(i);
		RoleAllInfo* poInfo = const_cast<EnterSceneArg&>(roArg).mutable_roledatalist(i);
		RoleAllInfo& roRoleInfo = *poInfo;
		/*
		if(roArg.type() == KKSG::ENTER_SCENE_SELECT_ROLE)
		{
			CRoleInfoInit::FirstInitRoleInfo(roRoleInfo);
		}
		*/

		ClientInfo* poCliInfo = (i < roArg.cliinfolist_size() ? const_cast<EnterSceneArg&>(roArg).mutable_cliinfolist(i) : NULL);
		Role* poRole = RoleManager::Instance()->CreateRole(&roRoleInfo, qwSessionID, 0, poCliInfo, entersceneinfo, m_dwConnID);
		if(poRole == NULL)
		{
			LogError("Create role failed");
			roRes.set_result(KKSG::ERR_FAILED);
			RemoveRoleOnFailed(oRoleList);
			return;
		}
		poRole->SetAccountBackFlow(roArg.account_backflow(i));

		if(roArg.type() == KKSG::ENTER_SCENE_SELECT_ROLE)
		{
			poRole->SetFlag(RSF_ISFirstInit, true);
			poRole->OnFirstInit(roArg.rolenum(i));
			poRole->SetFlag(RSF_ISFirstInit, false);
		}

		oRoleList.push_back(poRole);
		if (i < roArg.friendlist_size())
		{
			poRole->FillFriendList(roArg.friendlist(i));
		}
		if(roArg.type() == KKSG::ENTER_SCENE_SELECT_ROLE) dwSceneTemplateID = (UINT32)poRole->GetLoginSceneID(roArg.rolenum(0));

		if (entersceneinfo.isenterhall)
		{
			SSInfo<<"roleid:"<<poRole->GetID()<<" create by hall mode"<<END;
		}
	}

	HEnterSceneType hest = HEST_NONE;
	//Create Scene
	Scene *pScene = NULL;
	if(roArg.destsceneid() != INVALID_SCENE_ID)	//进入已存在的场景
	{
		pScene = SceneManager::Instance()->FindBySceneID(roArg.destsceneid());
		if(pScene == NULL)
		{
			LogWarn("find dest scene %d failed!!!", roArg.destsceneid());
			roRes.set_result(ERR_FAILED);
			RemoveRoleOnFailed(oRoleList);
			return;
		}
		hest = HEST_HAVE_CREATED;
	}
	else
	{
		pScene = SceneManager::Instance()->FindOrCreateScene(dwSceneTemplateID, oRoleList.front(), roArg.switchdata(), roArg.newsceneuniqueid());
		if (pScene == NULL)
		{
			LogWarn("find or create scene %d failed!!!", dwSceneTemplateID);
			roRes.set_result(ERR_FAILED);
			RemoveRoleOnFailed(oRoleList);
			return;
		}
		hest = HEST_JUST_CREATED;
	}

	//Enter Scene
	for(std::vector<Role*>::size_type i = 0; i < oRoleList.size(); ++i)
	{
		Role* poRole = oRoleList[i];
		bool isWatcher = false;
		if (roArg.has_switchdata())
		{
			for(size_t i = 0; i < roArg.switchdata().watchers_size(); ++i)
			{
				if (poRole->GetID() == roArg.switchdata().watchers(i))
				{
					isWatcher = true;
					break;
				}
			}
		}

		if(roArg.type() == KKSG::ENTER_SCENE_SELECT_ROLE)
		{
			PtcG2M_RoleLogin oLoginPtc;
			oLoginPtc.m_Data.set_sessionid(poRole->GetSessionID());
			oLoginPtc.m_Data.mutable_roledata()->set_accountid(poRole->GetAccount());
			oLoginPtc.m_Data.mutable_roledata()->set_name(poRole->GetName());
			oLoginPtc.m_Data.mutable_roledata()->set_roleid(poRole->GetID());
			oLoginPtc.m_Data.mutable_roledata()->set_rolecreatedtime((UINT32)poRole->GetRoleAllInfo().brief().inittime());
			oLoginPtc.m_Data.mutable_roledata()->mutable_idipdata()->CopyFrom(poRole->GetRoleAllInfo().idiprecord());
			oLoginPtc.m_Data.mutable_roledata()->set_clientip(poRole->GetClientConf().ip());
			oLoginPtc.m_Data.mutable_roledata()->set_mapid(INVALID_MAP_ID);
			oLoginPtc.m_Data.mutable_roledata()->set_sceneid(INVALID_SCENE_ID);
			oLoginPtc.m_Data.mutable_roledata()->set_level(poRole->GetLevel());
			oLoginPtc.m_Data.mutable_roledata()->set_lastlogintime(poRole->GetLoginTime());
			oLoginPtc.m_Data.mutable_roledata()->set_token(poRole->GetClientConf().token());
			oLoginPtc.m_Data.mutable_roledata()->set_logintype(poRole->GetClientConf().logintype());
			oLoginPtc.m_Data.mutable_roledata()->set_loginplatid(poRole->GetClientConf().platid());
			oLoginPtc.m_Data.mutable_roledata()->set_account_backflow(poRole->IsAccountBackFlow());
			MSLink::Instance()->SendTo(oLoginPtc);
		}

		PtcG2M_RoleChangeSceneNtf oSceneNtf;
		oSceneNtf.m_Data.set_roleid(poRole->GetID());
		oSceneNtf.m_Data.set_gsline(GSConfig::Instance()->GetLine());
		oSceneNtf.m_Data.set_mapid(pScene->GetSceneTemplateID());
		oSceneNtf.m_Data.set_sceneid(pScene->GetSceneID());
		oSceneNtf.m_Data.set_iscrossgs(GSConfig::Instance()->IsCrossGS());
		poRole->SendToMS(oSceneNtf);

		PtcG2T_ChangeGsLine oPtc;
		oPtc.m_sessionID = poRole->GetSessionID();
		oPtc.m_Data.set_sessionid(poRole->GetSessionID());
		oPtc.m_Data.set_line(GSConfig::Instance()->GetLine());
		oPtc.m_Data.set_is_cross_gs(GSConfig::Instance()->IsCrossGS());
		oPtc.m_Data.set_keepalive(pScene->CanLoginReconnect() && !isWatcher);
		poRole->Send(oPtc);

		if(roArg.type() == KKSG::ENTER_SCENE_SELECT_ROLE)
		{
			if (roArg.isgm())
			{
				poRole->SetGM(true);
				SSWarn<<"GM role:"<<poRole->GetName()<<" Enter Game"<<END;
			}

			//send role info
			PtcG2C_SelectRoleNtf oRoleNtf;
			oRoleNtf.m_sessionID = poRole->GetSessionID();
			poRole->DoAllSave();	// 各模块加载可能会对数据有改变，这里应该转一次pb
			*oRoleNtf.m_Data.mutable_roledata() = poRole->GetRoleAllInfo();
			oRoleNtf.m_Data.set_serverid(poRole->GetServerId());
			oRoleNtf.m_Data.set_backflow_firstenter(poRole->IsAccountBackFlow() && poRole->GetRoleAllInfo().extrainfo().logintimes() == 1 && roArg.rolenum(i) == 1);
			OpenSystemMgr::Instance()->ExceptCloseSystem(poRole, oRoleNtf.m_Data.mutable_roledata());
			poRole->Send(oRoleNtf);

			poRole->OnLogin();
			SceneManager::Instance()->EnterScene(poRole, pScene);
		}
		else
		{
			bool isWatcher = false;
			if (roArg.has_switchdata())
			{
				for(size_t i = 0; i < roArg.switchdata().watchers_size(); ++i)
				{
					if (poRole->GetID() == roArg.switchdata().watchers(i))
					{
						isWatcher = true;
						break;
					}
				}
			}

			if (GSConfig::Instance()->IsCrossGS())
			{
				SetRoleCrossData(poRole, roArg);
			}

			poRole->OnEnterGs();
			SceneManager::Instance()->EnterScene(poRole, pScene, isWatcher);
			pScene->InitRoleSwitchData(roArg.switchdata(), poRole, hest);
		}
	}

	if (GSConfig::Instance()->IsCrossGS())
	{
		PtcG2K_RoleEnterCrossSceneNtf ntf;
		ntf.m_Data.set_map_id(dwSceneTemplateID);
		ntf.m_Data.set_scene_id(roArg.destsceneid());
		ntf.m_Data.set_gsline(GSConfig::Instance()->GetLine());
		for (std::vector<Role*>::size_type i = 0; i < oRoleList.size(); ++i)
		{
			ntf.m_Data.add_rolelist(oRoleList[i]->GetID());
		}
		WorldLink::Instance()->SendTo(ntf);
	}

	LogInfo("Role [%s] enter scene success", RoleInfoList2Str(roArg.roledatalist()).c_str());
	roRes.set_mapid(pScene->GetSceneTemplateID());
	roRes.set_sceneid(pScene->GetSceneID());
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcN2G_EnterScene::OnDelayReplyRpc(const EnterSceneArg &roArg, EnterSceneRes &roRes, const CUserData &roUserData)
{
}
