#include "pch.h"
#include "team/rpcm2g_teamchangeexpcheck.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "team/teamrequest.h"

// generate by ProtoGen at date: 2016/8/24 19:58:05

RPC_CLIENT_IMPLEMETION(RpcM2G_TeamChangeExpCheck, TeamChangeExpArg, TeamChangeExpRes)

void RpcM2G_TeamChangeExpCheck::OnReply(const TeamChangeExpArg &roArg, const TeamChangeExpRes &roRes, const CUserData &roUserData)
{
	UINT64 leader = roArg.teamalldata().leaderid();
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(leader);
	if(NULL == pRole)
	{
		return ;
	}
	if(ERR_SUCCESS == roRes.ret() && 0 != roRes.expid())
	{
		TeamRequest req(pRole);
		KKSG::TeamOPRes tmp;//no use
		req.ChangeExpID(roRes.expid(), tmp, roRes.teamnothelper());
	}
}

void RpcM2G_TeamChangeExpCheck::OnTimeout(const TeamChangeExpArg &roArg, const CUserData &roUserData)
{
}
