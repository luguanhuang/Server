#include "pch.h"
#include "guild/rpcm2g_reqrecvfatigue.h"
#include "guild/guildrecord.h"
#include "unit/role.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2016/9/19 10:23:40

RPC_SERVER_IMPLEMETION(RpcM2G_ReqRecvFatigue, ReqRecvFatigueArg, ReqRecvFatigueRes)

void RpcM2G_ReqRecvFatigue::OnCall(const ReqRecvFatigueArg &roArg, ReqRecvFatigueRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (pRole == NULL)
	{
		return ;
	}

	CGuildRecord* pRecord = pRole->Get<CGuildRecord>();
	if (pRecord == NULL)
	{
		return ;
	}

	roRes.set_recvnum(pRecord->GetRecvFatigue());
}

void RpcM2G_ReqRecvFatigue::OnDelayReplyRpc(const ReqRecvFatigueArg &roArg, ReqRecvFatigueRes &roRes, const CUserData &roUserData)
{
}
