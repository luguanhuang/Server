#include "pch.h"
#include "login/ptct2n_logoutntf.h"
#include "account/accountsessionmgr.h"
#include "scene/rpcn2g_leavescene.h"
#include "network/gslink.h"

// generate by ProtoGen at date: 2016/11/3 14:35:07

void PtcT2N_LogoutNtf::Process(UINT32 dwConnID)
{
	//连接断开超时后Logout
	CAccountSessionMgr::Instance()->RemoveReconnectAccount(m_sessionID);
	CAccountSession* poAccount = CAccountSessionMgr::Instance()->GetBySession(m_sessionID);
	if(poAccount == NULL)
	{
		LogWarn("Can't find account when session %llu terminated", m_sessionID);
		return;
	}

	CRole* poRole = poAccount->GetRole();
	if(poRole == NULL)
	{
		LogInfo("Account %s with session %llu receive logout notify from gate, logout account", poAccount->GetAccountID().c_str(), m_sessionID);
		CAccountSessionMgr::Instance()->LogoutAccount(poAccount->GetAccountID(), KKSG::KICK_NORMAL);
		return;
	}

	if(poRole->GetState() != ROLE_STATE_IN_GAME)
	{
		LogWarn("Account %s role %llu is changing scene, ignore logout notify from gate", poAccount->GetAccountID().c_str(), poRole->GetID());
		return;
	}

	if(poRole->GetGsLine() == INVALID_LINE_ID)
	{
		LogWarn("Account %s role %llu receive logout notify from gate, but gsline is 0", poAccount->GetAccountID().c_str(), poRole->GetID());
		return;
	}

	RpcN2G_LeaveScene* rpc = RpcN2G_LeaveScene::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.add_rolelist(poRole->GetID());
	rpc->m_oArg.set_type(KKSG::LEAVE_SCENE_LOGOUT);
	rpc->m_oArg.set_logouttype(KKSG::LOGOUT_ACCOUNT_NORMAL);
	rpc->m_oArg.set_rpcid(0);
	rpc->m_oArg.set_account(poAccount->GetAccountID());
	rpc->m_oArg.set_line(poRole->GetGsLine());
	poRole->SendMsgToGS(*rpc);
	poRole->SetState(ROLE_STATE_LEAVE_SCENE);
}
