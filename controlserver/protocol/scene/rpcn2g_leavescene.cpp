#include "pch.h"
#include "scene/rpcn2g_leavescene.h"
#include "login/rpcn2d_nsreadaccountdata.h"
#include "account/accountsessionmgr.h"
#include "network/dblink.h"
#include "account/logincontrol.h"
#include "scene/rpcn2g_enterscene.h"
#include "logutil.h"
#include "network/gslink.h"
#include "role/rolemanager.h"
#include "scene/scenemanager.h"
#include "scene/gsmanager.h"
#include "account/accountkick.h"
//#include "role/ptcm2d_delrolentf.h"
#include "util/dbutil.h"
#include "network/rslink.h"
#include "session/ptcn2r_reportsessioninfo.h"
#include "idutil.h"

// generate by ProtoGen at date: 2016/11/3 2:26:05

RPC_CLIENT_IMPLEMETION(RpcN2G_LeaveScene, LeaveSceneArg, LeaveSceneRes)

void RpcN2G_LeaveScene::OnReply(const LeaveSceneArg &roArg, const LeaveSceneRes &roRes, const CUserData &roUserData)
{
	if(roArg.type() == KKSG::LEAVE_SCENE_LOGOUT)
	{
		CRole* poRole = CRoleManager::Instance()->GetBySession(m_sessionID);
		if (!poRole)
		{
			LogError("Role [%s] not exist", Uint64List2Str(roArg.rolelist()).c_str());
			if(roArg.logouttype() == KKSG::LOGOUT_RELOGIN_KICK_ACCOUNT)
			{
				CLoginControl::Instance()->CancelLogin(roArg.account());
			}
			return;
		}
		bool isInCrossGs = poRole->IsInCrossGs();
		UINT64 roleId = poRole->GetID();
		std::string name = poRole->GetName();
		//踢账号下线
		if(roArg.logouttype() == KKSG::LOGOUT_RELOGIN_KICK_ACCOUNT)
		{
			if(roRes.result() != KKSG::ERR_SUCCESS)
			{
				CLoginControl::Instance()->CancelLogin(roArg.account());
				CUserData oUser((UINT32)roRes.result(), NULL);
				CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
				return;
			}
			UINT32 serverId = 0;
			CLoginRequest* pRequest = CLoginControl::Instance()->GetLoginReq(roArg.account());
			if (pRequest)
			{
				serverId = pRequest->m_dwServerId;
			}
			else
			{
				CAccountSession* pAccount = CAccountSessionMgr::Instance()->GetByAccountID(roArg.account());
				if (pAccount == NULL)
				{
					LogInfo("no loginrequest and accountsession when kickout old role, account[%s]", roArg.account().c_str());
					CLoginControl::Instance()->CancelLogin(roArg.account());
					CUserData oUser((UINT32)KKSG::ERR_FAILED, NULL);
					CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
					return;	
				}
				serverId = pAccount->GetServerId();
				LogInfo("no loginrequest when kickout old role, account[%s], use serverid[%u]", roArg.account().c_str(), serverId);
			}

			RpcN2D_NsReadAccountData *rpc = RpcN2D_NsReadAccountData::CreateRpc();
			rpc->m_sessionID = m_sessionID;
			rpc->m_oArg.set_account(roArg.account());
			rpc->m_oArg.set_serverid(serverId);
			rpc->m_oArg.set_rpcid(roArg.rpcid());
			rpc->m_oArg.set_readtype(KKSG::READ_ACCOUNT_DATA_LOGIN);
			DBLink::Instance()->SendTo(*rpc);

			CAccountSessionMgr::Instance()->LogoutAccount(roArg.account(), KKSG::KICK_RELOGIN);
		}
		else if(roArg.logouttype() == KKSG::LOGOUT_RETURN_SELECT_ROLE)	//返回选角
		{
			if(roRes.result() != KKSG::ERR_SUCCESS)
			{
				CUserData oUser((UINT32)roRes.result(), NULL);
				CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
				return;
			}

			RpcN2D_NsReadAccountData *rpc = RpcN2D_NsReadAccountData::CreateRpc();
			rpc->m_sessionID = m_sessionID;
			rpc->m_oArg.set_account(roArg.account());
			rpc->m_oArg.set_serverid(poRole->GetServerID());
			rpc->m_oArg.set_rpcid(roArg.rpcid());
			rpc->m_oArg.set_readtype(KKSG::READ_ACCOUNT_DATA_RETURN_SELECT_ROLE);
			DBLink::Instance()->SendTo(*rpc);

			CAccountSessionMgr::Instance()->LogoutRole(m_sessionID);
		}
		else if(roArg.logouttype() == KKSG::LOGOUT_IDIP_KICK_ACCOUNT || roArg.logouttype() == KKSG::LOGOUT_CHANGEPROFESSION)
		{
			KKSG::KickType nKickType = KKSG::KICK_NORMAL;
			CAccountKicker::Instance()->OnKickReply(roArg.account(), (KKSG::ErrorCode)roRes.result(), nKickType);

			if(roRes.result() != KKSG::ERR_SUCCESS) return;

			CAccountSessionMgr::Instance()->LogoutAccount(roArg.account(), nKickType);
		}
		else
		{
			if(roRes.result() != KKSG::ERR_SUCCESS) return;

			CAccountSessionMgr::Instance()->LogoutAccount(roArg.account(), KKSG::KICK_NORMAL);
		}


		// 之后pRole被释放
		if (isInCrossGs == false)
		{
			CGsManager::Instance()->DecGsLineRoleNum(roArg.line(), roArg.rolelist_size());
		}
		else
		{
			LogWarn("Kick out role[%llu] from cross gs", roleId);
			PtcN2R_ReportSessionInfo ptc;
			KKSG::CrossRoleInfo* info = ptc.m_Data.add_roles();
			info->set_session(m_sessionID);
			info->set_roleid(roleId);
			info->set_name(name);
			ptc.m_Data.set_is_add(false);
			RSLink::Instance()->SendTo(ptc);
		}
	}
	else
	{
		//切换场景
		std::vector<CRole*> oRoleList;
		for(INT32 i = 0; i < roArg.rolelist_size(); ++i)
		{
			UINT64 qwRoleID = roArg.rolelist(i);
			CRole* poRole = CRoleManager::Instance()->GetByRoleID(qwRoleID);
			if(poRole == NULL)
			{
				LogError("Can't find role %llu when n2g leave scene reply", qwRoleID);
				return;
			}
			oRoleList.push_back(poRole);
		}
		if(oRoleList.empty())
		{
			LogError("Error, Can't find roles when n2g leave scene reply");
			return;
		}

		if(roRes.result() != KKSG::ERR_SUCCESS)
		{
			LogError("Role [%s] leave scene failed, error [%d]", Uint64List2Str(roArg.rolelist()).c_str(), roRes.result());
			for(std::vector<CRole*>::size_type i = 0; i < oRoleList.size(); ++i)
			{
				CRole* poRole = oRoleList[i];
				poRole->SetState(ROLE_STATE_IN_GAME);
				poRole->ClearSwitchInfo();
			}
			return;
		}

		CRole* frontRole = oRoleList.front();
		UINT32 dwCurrGsLine = frontRole->GetGsLine();
		UINT64 qwSceneOwner = frontRole->GetDestOwnerID();
		UINT32 dwDestMapID  = frontRole->GetDestMapID();
		UINT32 dwDestSceneID = frontRole->GetDestSceneID();
		bool isFromCrossGs = frontRole->IsInCrossGs();
		bool isToCrossGs = frontRole->GetDestIsCross();

		KKSG::SceneSwitchData& roSwitchData = frontRole->GetSwitchData();
		for(std::vector<CRole*>::size_type i = 0; i < oRoleList.size(); ++i)
		{
			CRole* poRole = oRoleList[i];
			poRole->OnChangeScene();
		}

		LogInfo("Role [%s] leave %s_%u success", Uint64List2Str(roArg.rolelist()).c_str(), GetGsPrefix(isFromCrossGs), dwCurrGsLine);
		// 从本服场景离开
		if (!isFromCrossGs)
		{
			CGsManager::Instance()->DecGsLineRoleNum(dwCurrGsLine, roArg.rolelist_size());
		}

		//enter scene
		RpcN2G_EnterScene* rpc = RpcN2G_EnterScene::CreateRpc();
		rpc->m_sessionID = frontRole->GetSessionID();
		rpc->m_oArg.set_mapid(dwDestMapID);
		rpc->m_oArg.set_destsceneid(dwDestSceneID);
		rpc->m_oArg.set_type(KKSG::ENTER_SCENE_SWITCH);
		rpc->m_oArg.mutable_switchdata()->CopyFrom(roSwitchData);

		UINT32 dwNewSceneUID = (dwDestSceneID == INVALID_SCENE_ID ? CSceneManager::Instance()->NewID() : 0);
		rpc->m_oArg.set_newsceneuniqueid(dwNewSceneUID);
		for(INT32 i = 0; i < roRes.datalist_size(); ++i)
		{
			RoleAllInfo* poData = rpc->m_oArg.add_roledatalist();
			*poData = roRes.datalist(i);
		}
		for(std::vector<CRole*>::size_type i = 0; i < oRoleList.size(); ++i)
		{
			CRole* poRole = oRoleList[i];
			rpc->m_oArg.add_rolesessionlist(poRole->GetSessionID());
			CAccountSession* poAccount = CAccountSessionMgr::Instance()->GetBySession(poRole->GetSessionID());
			if(poAccount)
			{
				KKSG::ClientInfo* poCliInfo = rpc->m_oArg.add_cliinfolist();
				poCliInfo->CopyFrom(poAccount->GetCliConf());
				rpc->m_oArg.add_rolenum(poAccount->GetProfessionCount());
				rpc->m_oArg.add_account_backflow(poAccount->IsAccountBackFlow());
			}
		}

		// 从跨服场景离开
		if (isFromCrossGs)
		{
			PtcN2R_ReportSessionInfo ptc;
			for(std::vector<CRole*>::size_type i = 0; i < oRoleList.size(); ++i)
			{
				CRole* poRole = oRoleList[i];
				KKSG::CrossRoleInfo* info = ptc.m_Data.add_roles();
				info->set_session(poRole->GetSessionID());
				info->set_roleid(poRole->GetID());
				info->set_name(poRole->GetName());
			}
			ptc.m_Data.set_is_add(false);
			RSLink::Instance()->SendTo(ptc);
		}

		bool bRpcSended = false;
		// 切到本服场景
		if (!isToCrossGs)
		{
			bRpcSended = GSLink::Instance()->SendToLine(roArg.destline(), *rpc);
		}
		// 切到跨服场景
		else 
		{
			PtcN2R_ReportSessionInfo ptc;
			for(std::vector<CRole*>::size_type i = 0; i < oRoleList.size(); ++i)
			{
				CRole* poRole = oRoleList[i];
				KKSG::CrossRoleInfo* info = ptc.m_Data.add_roles();
				info->set_session(poRole->GetSessionID());
				info->set_roleid(poRole->GetID());
				info->set_name(poRole->GetName());
			}
			ptc.m_Data.set_cross_gsline(roArg.destline());
			ptc.m_Data.set_is_add(true);
			RSLink::Instance()->SendTo(ptc);
			bRpcSended = RSLink::Instance()->SendTo(*rpc);
		}

		//Rpc发送失败，踢人下线
		if(!bRpcSended)
		{
			LogError("Send rpc to gs failed, logout role [%s]", Uint64List2Str(roArg.rolelist()).c_str());
			for(std::vector<CRole*>::size_type i = 0; i < oRoleList.size(); ++i)
			{
				CRole* poRole = oRoleList[i];
				CAccountSessionMgr::Instance()->LogoutAccount(poRole->GetAccount(), KKSG::KICK_NORMAL);
			}
		}
	}
}

void RpcN2G_LeaveScene::OnTimeout(const LeaveSceneArg &roArg, const CUserData &roUserData)
{
	//先获取RoleList
	std::vector<CRole*> oRoleList;
	for(INT32 i = 0; i < roArg.rolelist_size(); ++i)
	{
		UINT64 qwRoleID = roArg.rolelist(i);
		CRole* poRole = CRoleManager::Instance()->GetByRoleID(qwRoleID);
		if(poRole == NULL)
		{
			LogError("Can't find role %llu when n2g leave scene timeout", qwRoleID);
			return;
		}
		oRoleList.push_back(poRole);
	}
	if(oRoleList.empty())
	{
		LogError("Error, Can't find roles when n2g leave scene timeout");
		return;
	}

	//清理状态
	for(std::vector<CRole*>::size_type i = 0; i < oRoleList.size(); ++i)
	{
		CRole* poRole = oRoleList[i];

		if(roArg.type() == KKSG::LEAVE_SCENE_LOGOUT)
		{
			//踢账号下线
			if(roArg.logouttype() == KKSG::LOGOUT_RELOGIN_KICK_ACCOUNT)
			{
				CLoginControl::Instance()->CancelLogin(roArg.account());
				LogWarn("account=[%s] role=[%llu] session=[%llu] leave scene timeout when relogin, cancel this login request", poRole->GetAccount().c_str(), poRole->GetID(), m_sessionID);
			}
			else if(roArg.logouttype() == KKSG::LOGOUT_IDIP_KICK_ACCOUNT || roArg.logouttype() == KKSG::LOGOUT_CHANGEPROFESSION)
			{
				KKSG::KickType nKickType = KKSG::KICK_NORMAL;
				CAccountKicker::Instance()->OnKickReply(roArg.account(), KKSG::ERR_TIMEOUT, nKickType);
			}

			poRole->SetState(ROLE_STATE_IN_GAME);
		}
		else
		{
			//切场景
			poRole->SetState(ROLE_STATE_IN_GAME);
			poRole->ClearSwitchInfo();
		}
	}
}
