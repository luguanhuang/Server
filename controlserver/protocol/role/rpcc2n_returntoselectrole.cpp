#include "pch.h"
#include "role/rpcc2n_returntoselectrole.h"
#include "scene/rpcn2g_leavescene.h"
#include "account/accountsessionmgr.h"
#include "role/rolemanager.h"
#include "scene/rpcn2g_leavescene.h"
#include "network/gslink.h"

// generate by ProtoGen at date: 2016/11/3 2:19:50

RPC_SERVER_IMPLEMETION(RpcC2N_ReturnToSelectRole, ReturnToSelectRoleArg, ReturnToSelectRoleRes)

void RpcC2N_ReturnToSelectRole::OnCall(const ReturnToSelectRoleArg &roArg, ReturnToSelectRoleRes &roRes)
{
	CAccountSession* poAccount = CAccountSessionMgr::Instance()->GetBySession(m_sessionID);
	if(poAccount == NULL)
	{
		LogError("Can not find account by session %llu", m_sessionID);
		return;
	}

	CRole* poRole = poAccount->GetRole();
	if(poRole == NULL)
	{
		LogError("Account %s has no role", poAccount->GetAccountID().c_str());
		return;
	}

	if(poRole->GetGsLine() == INVALID_LINE_ID)
	{
		LogError("Account %s , role %llu is not in any gs line", poAccount->GetAccountID().c_str(), poRole->GetID());
		return;
	}

	if(poRole->GetState() != ROLE_STATE_IN_GAME)
	{
		LogError("Role %llu is not in scene, can't return to select role", poRole->GetID());
		return;
	}

	RpcN2G_LeaveScene* rpc = RpcN2G_LeaveScene::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.add_rolelist(poRole->GetID());
	rpc->m_oArg.set_type(KKSG::LEAVE_SCENE_LOGOUT);
	rpc->m_oArg.set_logouttype(KKSG::LOGOUT_RETURN_SELECT_ROLE);
	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_account(poAccount->GetAccountID());
	rpc->m_oArg.set_line(poRole->GetGsLine());
	poRole->SendMsgToGS(*rpc);
	poRole->SetState(ROLE_STATE_LEAVE_SCENE);
}

void RpcC2N_ReturnToSelectRole::OnDelayReplyRpc(const ReturnToSelectRoleArg &roArg, ReturnToSelectRoleRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	KKSG::LoadAccountData* poAccount = (KKSG::LoadAccountData*)roUserData.m_pUserPtr;
	if(nResult == KKSG::ERR_SUCCESS && poAccount != NULL)
	{
		*roRes.mutable_accountdata() = *poAccount;
	}
}
