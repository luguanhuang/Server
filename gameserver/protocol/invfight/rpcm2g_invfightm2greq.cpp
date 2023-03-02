#include "pch.h"
#include "invfight/rpcm2g_invfightm2greq.h"
#include "unit/rolemanager.h"
#include "unit/role.h"

// generate by ProtoGen at date: 2016/12/15 16:49:02

RPC_SERVER_IMPLEMETION(RpcM2G_InvFightM2GReq, InvFightM2GArg, InvFightM2GRes)

void RpcM2G_InvFightM2GReq::OnCall(const InvFightM2GArg &roArg, InvFightM2GRes &roRes)
{
	ErrorCode ret = ERR_SUCCESS;
	switch(roArg.c2marg().reqtype())
	{
	case IFRT_INV_ONE:
		{
			Role* pFromRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
			if(pFromRole)
			{
				roRes.set_fromlevel(pFromRole->GetLevel());
			}
			else
			{
				ret = ERR_INVFIGHT_ME_SCENE;
				break;
			}

			Role* pToRole = RoleManager::Instance()->FindByRoleID(roArg.c2marg().roleid());
			if(pToRole)
			{
				roRes.set_tolevel(pToRole->GetLevel());
			}
			else
			{
				ret = ERR_INVFIGHT_OTHER_SCENE;
				break;
			}
		}
		break;
	case IFRT_ACCEPT_ONE:
		{
			Role* pFromRole = RoleManager::Instance()->FindByRoleID(roArg.c2marg().roleid());
			if(NULL == pFromRole || NULL == pFromRole->GetCurrScene())
			{
				ret = ERR_INVFIGHT_OTHER_SCENE;
				break;
			}
		}
		break;
	default:
		break;
	}

	roRes.set_errorcode(ret);
}

void RpcM2G_InvFightM2GReq::OnDelayReplyRpc(const InvFightM2GArg &roArg, InvFightM2GRes &roRes, const CUserData &roUserData)
{
}
