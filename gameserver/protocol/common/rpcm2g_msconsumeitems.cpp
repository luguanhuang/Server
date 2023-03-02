#include "pch.h"
#include "common/rpcm2g_msconsumeitems.h"
#include "unit/rolemanager.h"
#include "gamelogic/bagtransition.h"
// generate by ProtoGen at date: 2016/10/4 16:03:56

RPC_SERVER_IMPLEMETION(RpcM2G_MSConsumeItems, ConsumeItemsArg, ConsumeItemsRes)

	void RpcM2G_MSConsumeItems::OnCall(const ConsumeItemsArg &roArg, ConsumeItemsRes &roRes)
{
	Role* role = RoleManager::Instance()->FindByRoleID(roArg.role_id());
	if (NULL == role)
	{
		SSWarn<<"ms consume item, m_sessionID:"<<m_sessionID<<" not login"<<END;
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	if (role->GetLevel() < GetGlobalConfig().GuildTerritoryRoleLvl)
	{
		roRes.set_result(KKSG::ERR_QA_LEVEL_NOT_ENOUGH);
		return;
	}

	SSInfo<<"ms consume item, role id :"<<role->GetID()<<END;

	BagTakeItemTransition transition(role);
	transition.SetReason(roArg.reason(),roArg.subreason());
	for (int i = 0; i < roArg.items_size(); ++i)
	{
		if (!transition.TakeItem(roArg.items(i).itemid(),roArg.items(i).itemcount()))
		{
			transition.RollBack();	
			roRes.set_result(KKSG::ERR_ITEM_NOT_ENOUGH);
			return;
		}

		SSInfo<<"itemid:"<<roArg.items(i).itemid()<<" count:"<<roArg.items(i).itemcount()<<END;
	}
	transition.NotifyClient();	

	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcM2G_MSConsumeItems::OnDelayReplyRpc(const ConsumeItemsArg &roArg, ConsumeItemsRes &roRes, const CUserData &roUserData)
{
}
