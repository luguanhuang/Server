#include "pch.h"
#include "guild/rpcm2g_queryrolecardmatch.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "guild/guildrecord.h"

// generate by ProtoGen at date: 2016/10/26 21:19:07

RPC_SERVER_IMPLEMETION(RpcM2G_QueryRoleCardMatch, QueryRoleCardMatchArg, QueryRoleCardMatchRes)

void RpcM2G_QueryRoleCardMatch::OnCall(const QueryRoleCardMatchArg &roArg, QueryRoleCardMatchRes &roRes)
{
	Role* role = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	CGuildRecord* record = role->Get<CGuildRecord>();
	if (record->IsNoCardMatch())
	{
		record->SetCardMatch(roArg.guildid());
	}
	else
	{
		if (roArg.guildid() == record->GetCardMatch())
		{
			roRes.set_errorcode(KKSG::ERR_SUCCESS);
		}
		else
		{
			roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		}
	}
}

void RpcM2G_QueryRoleCardMatch::OnDelayReplyRpc(const QueryRoleCardMatchArg &roArg, QueryRoleCardMatchRes &roRes, const CUserData &roUserData)
{
}
