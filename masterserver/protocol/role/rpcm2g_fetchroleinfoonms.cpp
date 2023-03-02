#include "pch.h"
#include "role/rpcm2g_fetchroleinfoonms.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2016/11/23 15:33:01

RPC_CLIENT_IMPLEMETION(RpcM2G_FetchRoleInfoOnMs, FetchRoleInfoOnMsArg, FetchRoleInfoOnMsRes)

void RpcM2G_FetchRoleInfoOnMs::OnReply(const FetchRoleInfoOnMsArg &roArg, const FetchRoleInfoOnMsRes &roRes, const CUserData &roUserData)
{
	if(roRes.result() != KKSG::ERR_SUCCESS)
	{
		LogError("Fetch role %llu from gs_%u failed", roArg.roleid(), roArg.gsline());
		return;
	}

	const KKSG::RoleInfoOnMs& roInfo = roRes.roleinfo();
	CRole* poFinded = CRoleManager::Instance()->GetByRoleID(roInfo.roleid());
	if(poFinded != NULL)
	{
		poFinded->OnEnterScene();
		return;
	}

	CRole* poRole = CRoleManager::Instance()->RecoverRole(&roInfo, roRes.sessionid());
	if(poRole != NULL)
	{
		poRole->SetState(ROLE_STATE_IN_GAME);
		poRole->SetGsLine(roArg.gsline());
		poRole->SetInCrossGs(roRes.iscrossgs());
		poRole->OnEnterScene();
	}
}

void RpcM2G_FetchRoleInfoOnMs::OnTimeout(const FetchRoleInfoOnMsArg &roArg, const CUserData &roUserData)
{
}
