#include "pch.h"
#include "guild/rpcm2g_modifygsguildname.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2017/4/11 22:31:33

RPC_SERVER_IMPLEMETION(RpcM2G_ModifyGsGuildName, ModifyFwArg, ModifyFwRes)

void RpcM2G_ModifyGsGuildName::OnCall(const ModifyFwArg &roArg, ModifyFwRes &roRes)
{
	Role* role = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (NULL == role)
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	BagTakeItemTransition trans(role);
	trans.SetReason(ItemFlow_Rename, ItemFlow_RenameCost);
	if (!trans.TakeItem(GetGlobalConfig().ReGuildNameItemCost[0], GetGlobalConfig().ReGuildNameItemCost[1]))
	{
		trans.RollBack();
		roRes.set_error(ERR_ITEM_NOT_ENOUGH);
		return;
	}
	trans.NotifyClient();

	roRes.set_error(ERR_SUCCESS);
}

void RpcM2G_ModifyGsGuildName::OnDelayReplyRpc(const ModifyFwArg &roArg, ModifyFwRes &roRes, const CUserData &roUserData)
{
}
