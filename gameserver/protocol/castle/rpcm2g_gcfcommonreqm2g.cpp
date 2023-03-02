#include "pch.h"
#include "castle/rpcm2g_gcfcommonreqm2g.h"
#include "unit/rolemanager.h"
#include "unit/role.h"

// generate by ProtoGen at date: 2016/12/31 22:32:09

RPC_SERVER_IMPLEMETION(RpcM2G_GCFCommonReqM2G, GCFCommonArgM2G, GCFCommonResM2G)

void RpcM2G_GCFCommonReqM2G::OnCall(const GCFCommonArgM2G &roArg, GCFCommonResM2G &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(roArg.sessionid());
	if(NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return ;
	}
	ErrorCode ret = ERR_SUCCESS;
	switch(roArg.c2marg().reqtype())
	{
	case GCF_JOIN_READY_SCENE:
		{
			if(pRole->GetLevel() < GetGlobalConfig().GuildTerritoryRoleLvl)
			{
				ret = ERR_GCASTLE_ROLE_LEVEL;
			}
		}
		break;
	default:
		break;
	}
	roRes.set_errorcode(ret);
}

void RpcM2G_GCFCommonReqM2G::OnDelayReplyRpc(const GCFCommonArgM2G &roArg, GCFCommonResM2G &roRes, const CUserData &roUserData)
{
}
