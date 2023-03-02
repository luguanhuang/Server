#include "pch.h"
#include "scene/rpcn2g_leavescene.h"
#include "unit/rolemanager.h"
#include "unit/selectrolestatemgr.h"
#include "unit/accountsessionmgr.h"
#include "scene/leavescenecontrol.h"
#include "logutil.h"
#include "apollovoice/ApolloVoiceMgr.h"
#include "config.h"
#include "gamelogic/rolesummarymgr.h"
#include "login/ptcg2m_rolelogout.h"
#include "gamelogic/changeprofession.h"
#include "crossscene/ptcg2k_roleleavecrossscenentf.h"
#include "network/worldlink.h"

// generate by ProtoGen at date: 2016/7/10 16:17:08

RPC_SERVER_IMPLEMETION(RpcN2G_LeaveScene, LeaveSceneArg, LeaveSceneRes)

void RpcN2G_LeaveScene::OnCall(const LeaveSceneArg &roArg, LeaveSceneRes &roRes)
{
	if(roArg.rolelist_size() == 0)
	{
		LogWarn("leave scene role list is empty");
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	UINT32 logoutType = roArg.logouttype();
	UINT32 leaveType = roArg.type();

	std::vector<Role*> oRoleList;
	for(INT32 i = 0; i < roArg.rolelist_size(); ++i)
	{
		UINT64 qwRoleID = roArg.rolelist(i);
		Role *pRole = RoleManager::Instance()->FindByRoleID(qwRoleID);
		if(pRole == NULL)
		{
			LogWarn("Can't find role %llu when n2g_leavescene", qwRoleID);
			continue;
		}

		oRoleList.push_back(pRole);
		if(leaveType == KKSG::LEAVE_SCENE_LOGOUT || leaveType == KKSG::LEAVE_SCENE_SWITCH)
		{
			ApolloVoiceMgr::Instance()->HandleLeaveScene(qwRoleID);
		}
	}
	if(oRoleList.empty())
	{
		LogWarn("Can't find role [%s] when n2g_leavescene, return with success", Uint64List2Str(roArg.rolelist()).c_str());
		roRes.set_result(KKSG::ERR_SUCCESS);
		return;
	}

	LogInfo("RpcN2G_LeaveScene Role [%s] begin leave scene, LeaveType: %u, LogoutType: %u", Uint64List2Str(roArg.rolelist()).c_str(), leaveType, logoutType);

	UINT32 dwSerialID = 0;
	UINT32 dwRpcID = DelayRpc();
	dwSerialID = CLeaveSceneControl::Instance()->BeginLeave(dwRpcID, oRoleList);
	if(dwSerialID == 0)
	{
		LogWarn("Begin leave scene error");
		roRes.set_result(KKSG::ERR_STATE_ERROR);

		CUserData oUser;
		CRpc::ReplyDelayRpc(dwRpcID, oUser);
		return;
	}

	for(std::vector<Role*>::size_type i = 0; i < oRoleList.size(); ++i)
	{
		Role* pRole = oRoleList[i];
		UINT64 qwRoleID = pRole->GetID();
		UINT64 qwRoleSessionID = pRole->GetSessionID();

		if(roArg.type() == KKSG::LEAVE_SCENE_LOGOUT)
		{
			///> 换职业踢人处理
			if (logoutType == (UINT32)KKSG::KICK_CHANGE_PROFESSION)
			{
				ChangeProfessionMgr::Instance()->DoChange(pRole);
			}
			pRole->OnLogout("normal", dwSerialID);

			PtcG2M_RoleLogout oPtc;
			oPtc.m_Data.set_level(pRole->GetLevel());
			oPtc.m_Data.set_sessionid(qwRoleSessionID);
			pRole->SendToMS(oPtc);
		}
		else
		{
			pRole->OnLeaveGs(dwSerialID);
		}

		KKSG::RoleAllInfo* poRoleInfo = roRes.add_datalist();
		poRoleInfo->CopyFrom(pRole->GetRoleAllInfo());

		// 跨服GS，玩家离开场景rolesummary就删掉了
		if (GSConfig::Instance()->IsCrossGS())
		{
			RoleSummaryMgr::Instance()->RemoveRoleSummary(qwRoleID);
		}
		// 非主城GS，玩家离开场景同步一次rolesummary给MS
		else if (!GSConfig::Instance()->IsHallGS())
		{
			RoleSummaryMgr::Instance()->SendSingleRoleSummary(qwRoleID);
			RoleSummaryMgr::Instance()->RemoveRoleSummary(qwRoleID);
		}

		RoleManager::Instance()->RemoveBySession(qwRoleSessionID);
		SelectRoleStateMgr::Instance()->RemoveSessionState(qwRoleSessionID);
	}

	if (GSConfig::Instance()->IsCrossGS())
	{
		PtcG2K_RoleLeaveCrossSceneNtf ntf;
		ntf.m_Data.set_gsline(GSConfig::Instance()->GetLine());
		for (std::vector<Role*>::size_type i = 0; i < oRoleList.size(); ++i)
		{
			ntf.m_Data.add_rolelist(oRoleList[i]->GetID());
		}
		WorldLink::Instance()->SendTo(ntf);
	}

	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcN2G_LeaveScene::OnDelayReplyRpc(const LeaveSceneArg &roArg, LeaveSceneRes &roRes, const CUserData &roUserData)
{
	roRes.set_result(KKSG::ErrorCode(roUserData.m_dwUserData));
	if(roRes.result() == KKSG::ERR_SUCCESS && roArg.type() == KKSG::LEAVE_SCENE_SWITCH && roUserData.m_pUserPtr != NULL)
	{
		/*CLeaveSceneReq* poReq = (CLeaveSceneReq*)roUserData.m_pUserPtr;

		for(INT32 i = 0; i < roArg.rolelist_size(); ++i)
		{
			UINT64 qwRoleID = roArg.rolelist(i);
		}*/
	}
}
