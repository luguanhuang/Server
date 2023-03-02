#include "pch.h"
#include "dragonguild/rpcm2g_modifydragonguildnamecost.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/bagtransition.h"
// generate by ProtoGen at date: 2017/9/8 10:45:33

RPC_SERVER_IMPLEMETION(RpcM2G_ModifyDragonGuildNameCost, ModifyDragonNameCostArg, ModifyDragonNameCostRes)

void RpcM2G_ModifyDragonGuildNameCost::OnCall(const ModifyDragonNameCostArg &roArg, ModifyDragonNameCostRes &roRes)
{
	Role* role = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	// reason subreason items 需要改成小分队的，先暂时用这个替代
	BagTakeItemTransition trans(role);
	trans.SetReason(ItemFlow_Rename, ItemFlow_RenameCost);
	if (!trans.TakeItem(GetGlobalConfig().ReDragonGuileNameItemCost[0], GetGlobalConfig().ReDragonGuileNameItemCost[1]))
	{
		trans.RollBack();
		roRes.set_result(ERR_ITEM_NOT_ENOUGH);
		return;
	}
	trans.NotifyClient();

	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcM2G_ModifyDragonGuildNameCost::OnDelayReplyRpc(const ModifyDragonNameCostArg &roArg, ModifyDragonNameCostRes &roRes, const CUserData &roUserData)
{
}
