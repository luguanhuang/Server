#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "role/rpcm2g_changenamegs.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/randomnamemgr.h"
#include "gamelogic/rolesummary.h"
#include "gamelogic/rolesummarymgr.h"

// generate by ProtoGen at date: 2016/10/10 17:40:30

RPC_SERVER_IMPLEMETION(RpcM2G_ChangeNameGs, ChangeNameGsArg, ChangeNameGsRes)

#define CHANGENAME_LEVELLIMIT 5

void RpcM2G_ChangeNameGs::OnCall(const ChangeNameGsArg &roArg, ChangeNameGsRes &roRes)
{
	Role* role = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	// level limit
	if (role->GetLevel() < CHANGENAME_LEVELLIMIT)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	// count limit 
	UINT32 count = role->GetRoleAllInfoPtr()->brief().changenamecount();
	if (0 != count)
	{
		// consume
		BagTakeItemTransition trans(role);
		trans.SetReason(ItemFlow_Rename, ItemFlow_RenameCost);
		if (m_oArg.iscostitem())
		{
			if (!trans.TakeItem(GetGlobalConfig().RenameItemCost[0], GetGlobalConfig().RenameItemCost[1]))
			{
				trans.RollBack();
				roRes.set_errorcode(ERR_ITEM_NOT_ENOUGH);
				return;
			}
		}
		else
		{
			UINT32 cost = RandomNameMgr::Instance()->GetRenameConsume(count);
			if (!trans.TakeItem(DRAGON_COIN, cost))
			{
				trans.RollBack();
				roRes.set_errorcode(ERR_ITEM_NEED_DRAGONCOIN);
				return;
			}
		}
		trans.NotifyClient();
	}
	role->GetRoleAllInfoPtr()->mutable_brief()->set_changenamecount(count + 1);
	role->GetRoleAllInfoPtr()->mutable_brief()->set_name(roArg.name());

	RoleSummary* summary = RoleSummaryMgr::Instance()->GetRoleSummary(role->GetID());
	if (NULL != summary)
	{
		summary->SetName(roArg.name());
		RoleSummaryMgr::Instance()->UpdateRoleSummary(role, SUMMARY_NAME);
	}
	roRes.set_errorcode(ERR_SUCCESS);
}

void RpcM2G_ChangeNameGs::OnDelayReplyRpc(const ChangeNameGsArg &roArg, ChangeNameGsRes &roRes, const CUserData &roUserData)
{
}
