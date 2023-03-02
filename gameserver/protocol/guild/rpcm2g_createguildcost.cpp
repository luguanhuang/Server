#include "pch.h"
#include "guild/rpcm2g_createguildcost.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/takecostmgr.h"
#include "table/OpenSystemMgr.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2016/8/26 17:30:59

RPC_SERVER_IMPLEMETION(RpcM2G_CreateGuildCost, CreateGuildCostArg, CreateGuildCostRes)

void RpcM2G_CreateGuildCost::OnCall(const CreateGuildCostArg &roArg, CreateGuildCostRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (pRole == NULL)
	{
		SSWarn << "pRole is NULL, session id:" << m_sessionID << END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(GuildSysId))
	{
		roRes.set_errorcode(KKSG::ERR_SYS_NOTOPEN);
		return ;
	}
	/*
	if (pRole->GetLevel() < OpenSystemMgr::Instance()->GetPlayerLevelBySystemID(17))
	{
		roRes.set_errorcode(KKSG::ERR_SYS_NOTOPEN);
		return ;
	}
	*/

	if (TakeCostManager::Instance()->TakeCost("GuildCreateCost", pRole, 0, ItemFlow_Guild, ItemFlow_Guild_Create))
	{
		roRes.set_errorcode(ERR_SUCCESS);
	}
	else
	{
		roRes.set_errorcode(ERR_ITEM_NEED_DRAGONCOIN);
	}
	return ;
}

void RpcM2G_CreateGuildCost::OnDelayReplyRpc(const CreateGuildCostArg &roArg, CreateGuildCostRes &roRes, const CUserData &roUserData)
{
}
