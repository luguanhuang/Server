#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/midas.h"
#include "custombattle/rpcm2g_custombattlegsop.h"

// generate by ProtoGen at date: 2017/5/2 17:00:34

RPC_SERVER_IMPLEMETION(RpcM2G_CustomBattleGsOp, CustomBattleGsOpArg, CustomBattleGsOpRes)

void RpcM2G_CustomBattleGsOp::OnCall(const CustomBattleGsOpArg &roArg, CustomBattleGsOpRes &roRes)
{
	Role* role = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
	    return;
	}
	int type = 0;
	int subreason = ItemFlow_CustomBattle_Invalid;
	switch(roArg.op())
	{
	case KKSG::CustomBattle_DoCreate:
		{
			type = 1;
			subreason = ItemFlow_CustomBattle_Create;
			UINT32 diamondcount = 0;
			for (int i = 0; i < roArg.itemcost_size(); ++i)
			{
				if (roArg.itemcost(i).itemid() == DIAMOND)
				{
					diamondcount = roArg.itemcost(i).itemcount();
					break;
				}
			}
			if (0 != diamondcount)
			{
				// delay to midas
				UINT32 delayid = DelayRpc();

				KKSG::PayConsumeBase info;
				info.set_type((int)MidasConsumeType_CustomBattle);
				info.set_amt(diamondcount);
				info.set_subreason(subreason);
				info.set_reason(ItemFlow_CustomBattle);
				info.set_index(delayid);
				info.set_count(0);

				std::vector<ItemDesc> vecItem;
				if(!role->ConsumeDiamond(info, vecItem))
				{
					CUserData data;
					data.m_dwUserData = KKSG::ERR_ITEM_NOT_ENOUGH;
					ReplyDelayRpc(delayid, data);
				}
				return;
			}
			break;
		}
	case KKSG::CustomBattle_DoJoin:
		{
			type = 1;
			subreason = ItemFlow_CustomBattle_Join;
			break;
		}
	case KKSG::CustomBattle_ClearCD:
		{
			type = 1;
			subreason = ItemFlow_CustomBattle_ClearCD;
			break;
	    }
	case KKSG::CustomBattle_Reward:
		{
			type = 2;
			subreason = roArg.issystem() ? ItemFlow_CustomBattle_RewardSystem : ItemFlow_CustomBattle_RewardCustom;
			break;
		}
	default:
	    break;
	}
	std::stringstream ss;
	if (1 == type)
	{
		BagTakeItemTransition take(role);
		take.SetReason(ItemFlow_CustomBattle, subreason);
		for (int i = 0; i < roArg.itemcost_size(); ++i)
		{
		    if (!take.TakeItem(roArg.itemcost(i).itemid(), roArg.itemcost(i).itemcount()))
		    {
		        take.RollBack();
				roRes.set_errorcode(KKSG::ERR_ITEM_NOT_ENOUGH);
		        return;
		    }
			ss<<roArg.itemcost(i).itemid()<<":"<<roArg.itemcost(i).itemcount()<<",";
		}
		take.NotifyClient();
		roRes.set_errorcode(KKSG::ERR_SUCCESS);

		LogInfo("roleid:%llu op:%d uid:%llu take item %s", role->GetID(), roArg.op(), roArg.uid(), ss.str().c_str());
	}
	else if (2 == type)
	{
		BagGiveItemTransition give(role);
		give.SetReason(ItemFlow_CustomBattle, subreason);
	    for (int i = 0; i < roArg.itemcost_size(); ++i)
	    {
	        give.GiveItem(roArg.itemcost(i).itemid(), roArg.itemcost(i).itemcount());

			ss<<roArg.itemcost(i).itemid()<<":"<<roArg.itemcost(i).itemcount()<<",";
	    }
	    give.NotifyClient();
		roRes.set_errorcode(KKSG::ERR_SUCCESS);

		LogInfo("roleid:%llu op:%d uid:%llu give item %s", role->GetID(), roArg.op(), roArg.uid(), ss.str().c_str());
	}
	else
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
	}
}

void RpcM2G_CustomBattleGsOp::OnDelayReplyRpc(const CustomBattleGsOpArg &roArg, CustomBattleGsOpRes &roRes, const CUserData &roUserData)
{
	roRes.set_errorcode((KKSG::ErrorCode)(roUserData.m_dwUserData));
}
