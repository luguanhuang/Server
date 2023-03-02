#include "pch.h"
#include "role/rpcn2m_checkrolename.h"
#include "role/rpcn2d_createroleondbnew.h"
#include "network/dblink.h"
#include "account/accountsessionmgr.h"
#include "account/logincontrol.h"
#include "table/globalconfig.h"
#include "timeutil.h"

// generate by ProtoGen at date: 2016/11/9 0:09:48

RPC_CLIENT_IMPLEMETION(RpcN2M_CheckRoleName, CheckRoleNameArg, CheckRoleNameRes)

void RpcN2M_CheckRoleName::OnReply(const CheckRoleNameArg &roArg, const CheckRoleNameRes &roRes, const CUserData &roUserData)
{
	CAccountSession* poAccount = CAccountSessionMgr::Instance()->GetBySession(m_sessionID);
	if(poAccount == NULL)
	{
		CLoginControl::Instance()->UnlockName(roArg.name());
		return;
	}

	if(roRes.result() != KKSG::ERR_SUCCESS)
	{
		CLoginControl::Instance()->UnlockName(roArg.name());

		CUserData oUser;
		oUser.m_dwUserData = (UINT32)roRes.result();
		oUser.m_pUserPtr   = NULL;
		CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
	}
	else
	{
		RpcN2D_CreateRoleOnDBNew* rpc = RpcN2D_CreateRoleOnDBNew::CreateRpc();
		rpc->m_sessionID = m_sessionID;
		rpc->m_oArg.set_rpcid(roArg.rpcid());

		UINT64 qwRoleID = CAccountSessionMgr::Instance()->NewRoleID(poAccount->GetServerId());
		RoleAllInfo *poRoleInfo = rpc->m_oArg.mutable_roledata();
		poRoleInfo->mutable_brief()->set_type(roArg.type());
		poRoleInfo->mutable_brief()->set_name(roArg.name());
		poRoleInfo->mutable_brief()->set_accountid(poAccount->GetAccountID());
		poRoleInfo->mutable_brief()->set_roleid(qwRoleID);
		poRoleInfo->mutable_brief()->set_level(1);		// TODO
		poRoleInfo->mutable_brief()->set_exp(0);
		poRoleInfo->mutable_brief()->set_maxexp(0);
		poRoleInfo->mutable_brief()->set_sceneid(GetGlobalConfig().InitialSceneID);
		//poRoleInfo->mutable_brief()->set_sceneid( GetGlobalConfig().InitialReturnSceneID);
		//poRoleInfo->mutable_brief()->set_tutorialbits(0);

		poRoleInfo->mutable_brief()->set_inittime(TimeUtil::GetTime());
		poRoleInfo->mutable_extrainfo()->set_lastleveluptime(TimeUtil::GetTime());

		DBLink::Instance()->SendTo(*rpc);
	}
}

void RpcN2M_CheckRoleName::OnTimeout(const CheckRoleNameArg &roArg, const CUserData &roUserData)
{
	CLoginControl::Instance()->UnlockName(roArg.name());
	//CAccountSessionMgr::Instance()->OnCreateRoleTimeout(m_sessionID);

	CUserData oUser;
	oUser.m_dwUserData = ERR_TIMEOUT;
	oUser.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}
