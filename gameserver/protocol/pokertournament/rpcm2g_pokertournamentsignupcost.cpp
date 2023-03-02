#include "pch.h"
#include "pokertournament/rpcm2g_pokertournamentsignupcost.h"
#include "unit/rolemanager.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2016/12/12 11:10:27

RPC_SERVER_IMPLEMETION(RpcM2G_PokerTournamentSignUpCost, ConsumeItemsArg, PokerTournamentSignUpCostRes)

	void RpcM2G_PokerTournamentSignUpCost::OnCall(const ConsumeItemsArg &roArg, PokerTournamentSignUpCostRes &roRes)
{
	LogDebug("roArg.role_id() : %llu",roArg.role_id()); 

	roRes.set_role_id(roArg.role_id());

	Role* role = RoleManager::Instance()->FindByRoleID(roArg.role_id());
	if (NULL == role)
	{
		SSWarn<<"ms consume item, m_sessionID:"<<m_sessionID<<" not login"<<END;
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	SSInfo<<"ms consume item, role id :"<<role->GetID()<<END;

	BagTakeItemTransition transition(role);
	transition.SetReason(roArg.reason(),roArg.subreason());
	for (int i = 0; i < roArg.items_size(); ++i)
	{
		if (!transition.TakeItem(roArg.items(i).itemid(),roArg.items(i).itemcount() ))
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

void RpcM2G_PokerTournamentSignUpCost::OnDelayReplyRpc(const ConsumeItemsArg &roArg, PokerTournamentSignUpCostRes &roRes, const CUserData &roUserData)
{
}
