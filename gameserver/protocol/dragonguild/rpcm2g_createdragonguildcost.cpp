#include "pch.h"
#include "dragonguild/rpcm2g_createdragonguildcost.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/takecostmgr.h"
#include "define/systemiddef.h"
#include "table/OpenSystemMgr.h"

// generate by ProtoGen at date: 2017/9/4 21:00:04

RPC_SERVER_IMPLEMETION(RpcM2G_CreateDragonGuildCost, CreateDragonGuildCostArg, CreateDragonGuildCostRes)

void RpcM2G_CreateDragonGuildCost::OnCall(const CreateDragonGuildCostArg &roArg, CreateDragonGuildCostRes &roRes)
{
	// cost items and reply to Ms
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (pRole == NULL)
	{
		SSWarn << "pRole is NULL, session id:" << m_sessionID << END;
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}

	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_DRAGONGUILD))
	{
		roRes.set_result(KKSG::ERR_SYS_NOTOPEN);
		return ;
	}

	if (pRole->GetLevel() < OpenSystemMgr::Instance()->GetPlayerLevelBySystemID(SYS_DRAGONGUILD))
	{
		roRes.set_result(KKSG::ERR_SYS_NOTOPEN);
		return ;
	}

	// 只消耗龙币，别的需要自己写, DragonGuildCreateCost 需要在globalconfig里面有 
	if (TakeCostManager::Instance()->TakeCost("DragonGuildCreateCost", pRole, 0, ItemFlow_DragonGuild, ItemFlow_DragonGuildCreate))
	{
		roRes.set_result(ERR_SUCCESS);
	}
	else
	{
		roRes.set_result(ERR_ITEM_NEED_DRAGONCOIN);
	}

	return ;
}

void RpcM2G_CreateDragonGuildCost::OnDelayReplyRpc(const CreateDragonGuildCostArg &roArg, CreateDragonGuildCostRes &roRes, const CUserData &roUserData)
{
}
