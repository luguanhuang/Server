#include "pch.h"
#include "guild/rpcm2g_reqguildcheckin.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildrecord.h"

// generate by ProtoGen at date: 2016/9/18 10:29:18

RPC_SERVER_IMPLEMETION(RpcM2G_ReqGuildCheckin, ReqGuildCheckinArg, ReqGuildCheckinRes)

void RpcM2G_ReqGuildCheckin::OnCall(const ReqGuildCheckinArg &roArg, ReqGuildCheckinRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (pRole == NULL)
	{
		return;
	}

	CGuildRecord* pRecord = pRole->Get<CGuildRecord>();
	if (pRecord == NULL)
	{
		return ;
	}

	pRecord->Update();
	roRes.set_checkin(pRecord->GetCheckin());
	roRes.set_boxmask(pRecord->GetBoxMask());
}

void RpcM2G_ReqGuildCheckin::OnDelayReplyRpc(const ReqGuildCheckinArg &roArg, ReqGuildCheckinRes &roRes, const CUserData &roUserData)
{
}
