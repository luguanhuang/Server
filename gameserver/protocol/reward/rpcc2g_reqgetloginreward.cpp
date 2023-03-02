#include "pch.h"
#include "reward/rpcc2g_reqgetloginreward.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/loginrecord.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/item.h"
#include "gamelogic/spriteconfig.h"
#include "gamelogic/spritetransition.h"

// generate by ProtoGen at date: 2016/6/4 16:46:47

RPC_SERVER_IMPLEMETION(RpcC2G_ReqGetLoginReward, LoginRewardGetReq, LoginRewardGetRet)

void RpcC2G_ReqGetLoginReward::OnCall(const LoginRewardGetReq &roArg, LoginRewardGetRet &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(NULL == pRole)
	{
		return ;
	}
	roRes.set_ret(KKSG::ERR_FAILED);
	MyLoginRecord* mlr = pRole->Get<MyLoginRecord>();
	if(NULL == mlr)
	{
		return ;
	}
	if(KKSG::LOGINRS_HAVEHOT == mlr->GetLoginRewardState(roArg.day()))
	{
		std::vector<ItemDesc> items;
		mlr->GetLoginRewardItem(roArg.day(), items);
		BagGiveItemTransition give(pRole);
		give.SetReason(ItemFlow_Reward, ItemFlow_Reward_Login);
		for(size_t i = 0; i < items.size(); ++i)
		{
			give.GiveItem(items[i]);
		}
		give.NotifyClient();

		mlr->SetLoginRewardState(roArg.day(), KKSG::LOGINRS_HAVE);

		roRes.set_ret(KKSG::ERR_SUCCESS);

		mlr->UpdateLoginRewardOpenState();
	}
}

void RpcC2G_ReqGetLoginReward::OnDelayReplyRpc(const LoginRewardGetReq &roArg, LoginRewardGetRet &roRes, const CUserData &roUserData)
{
}
