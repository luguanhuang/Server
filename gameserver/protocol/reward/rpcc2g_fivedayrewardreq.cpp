#include "pch.h"
#include "reward/rpcc2g_fivedayrewardreq.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/loginrecord.h"
#include "gamelogic/bagtransition.h"
#include "table/globalconfig.h"

// generate by ProtoGen at date: 2016/6/11 10:40:39

RPC_SERVER_IMPLEMETION(RpcC2G_FiveDayRewardReq, FiveRewardRes, FiveRewardRet)

void RpcC2G_FiveDayRewardReq::OnCall(const FiveRewardRes &roArg, FiveRewardRet &roRes)
{
	/*
	roRes.set_ret(KKSG::ERR_FAILED);

	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(NULL == pRole)
	{
		return ;
	}
	MyLoginRecord* mlr = pRole->Get<MyLoginRecord>();
	if(NULL == mlr)
	{
		return ;
	}
	UINT32 logindaycount = mlr->GetRealLoginDayCount();
	if(roArg.fiveday())
	{
		if(mlr->GetFiveDayRS() == (KKSG::LOGINRS_HAVEHOT))
		{
			BagGiveItemTransition give(pRole);
			give.SetReason(ItemFlow_Reward, ItemFlow_Reward_Days);
			for(size_t i = 0; i <  GetGlobalConfig().FiveDayRewards.size(); ++i)
			{
				Sequence<UINT32,2> seq =  GetGlobalConfig().FiveDayRewards[i];	
				give.GiveItem(seq[0], seq[1], false);
			}
			give.NotifyClient();

			mlr->SetFiveDayRS(KKSG::LOGINRS_HAVE);
			roRes.set_ret(KKSG::ERR_SUCCESS);
		}
	}else if(roArg.twoday())
	{
		if(mlr->GetTwoDayRS() == (KKSG::LOGINRS_HAVEHOT))
		{
			BagGiveItemTransition give(pRole);
			give.SetReason(ItemFlow_Reward, ItemFlow_Reward_Days);
			for(size_t i = 0; i <  GetGlobalConfig().TwoDayRewards.size(); ++i)
			{
				Sequence<UINT32,2> seq =  GetGlobalConfig().TwoDayRewards[i];	
				give.GiveItem(seq[0], seq[1], false);
			}
			give.NotifyClient();

			mlr->SetTwoDayRS(KKSG::LOGINRS_HAVE);
			roRes.set_ret(KKSG::ERR_SUCCESS);
		}
	}
	else if(roArg.threeday())
	{
		if(mlr->GetThreeDayRS() == (KKSG::LOGINRS_HAVEHOT))
		{
			BagGiveItemTransition give(pRole);
			give.SetReason(ItemFlow_Reward, ItemFlow_Reward_Days);
			for(size_t i = 0; i <  GetGlobalConfig().ThreeDayRewards.size(); ++i)
			{
				Sequence<UINT32,2> seq =  GetGlobalConfig().ThreeDayRewards[i];	
				give.GiveItem(seq[0], seq[1], false);
			}
			give.NotifyClient();

			mlr->SetThreeDayRS(KKSG::LOGINRS_HAVE);
			roRes.set_ret(KKSG::ERR_SUCCESS);
		}
	}
	else if(roArg.sevenday())
	{
		if(mlr->GetSevenDayRS() == (KKSG::LOGINRS_HAVEHOT))
		{
			BagGiveItemTransition give(pRole);
			give.SetReason(ItemFlow_Reward, ItemFlow_Reward_Days);
			for(size_t i = 0; i <  GetGlobalConfig().SevenDayRewards.size(); ++i)
			{
				Sequence<UINT32,2> seq =  GetGlobalConfig().SevenDayRewards[i];	
				give.GiveItem(seq[0], seq[1], false);
			}
			give.NotifyClient();

			mlr->SetSevenDayRS(KKSG::LOGINRS_HAVE);
			roRes.set_ret(KKSG::ERR_SUCCESS);
		}
	}

	mlr->FiveDayRSNotify(pRole);
	*/
}

void RpcC2G_FiveDayRewardReq::OnDelayReplyRpc(const FiveRewardRes &roArg, FiveRewardRet &roRes, const CUserData &roUserData)
{
}
