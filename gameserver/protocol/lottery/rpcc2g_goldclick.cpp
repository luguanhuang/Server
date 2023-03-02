#include "pch.h"
#include "lottery/rpcc2g_goldclick.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/lotteryrecord.h"
#include "gamelogic/bagtransition.h"
#include "table/OpenSystemMgr.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2016/10/31 20:53:00

RPC_SERVER_IMPLEMETION(RpcC2G_GoldClick, GoldClickArg, GoldClickRes)

static UINT32 GetGoldClick(UINT32 times, std::vector<UINT32>& counts)
{
	UINT32 count = 0;
	for (auto i = GetGlobalConfig().GoldClickCount.begin(); i != GetGlobalConfig().GoldClickCount.end(); ++i)
	{
		count += i->seq[0];	
	}
	UINT32 ret = 0;
	for (UINT32 j = 0; j < times; ++j)
	{
		UINT32 random = XRandom::randInt(1, count + 1);
		UINT32 allrandom = 0;
		for (auto i = GetGlobalConfig().GoldClickCount.begin(); i != GetGlobalConfig().GoldClickCount.end(); ++i)
		{
			allrandom += i->seq[0];
			if (random <= allrandom)
			{
				ret += i->seq[1];
				counts.push_back(i->seq[1]);
				break;
			}
		}
	}
	return ret;
}

void RpcC2G_GoldClick::OnCall(const GoldClickArg &roArg, GoldClickRes &roRes)
{
	///> system open
	//if (OpenSystemMgr::Instance()->IsIdipCloseSystem(SysGoldClick))
	//{
	//	SSInfo<<"system close"<<END;
	//	return;
	//}

	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	if (!role->Get<CRoleSystem>()->IsSystemOpened(SysGoldClick))
	{
		SSInfo<<"system close"<<END;
		roRes.set_errorcode(ERR_FAILED);
		return;
	}

	LotteryRecord* record = role->Get<LotteryRecord>();
	UINT32 nowtime = (UINT32)time(NULL);
	record->UpdateGoldClick(nowtime);

	std::vector<UINT32> counts;
	if (0 == roArg.type())
	{

	}
	// free
	else if (1 == roArg.type())
	{
		if (!record->CountLimit(true) && record->CanFree())
		{
			UINT32 ret = GetGoldClick(1, counts);
			// give 
			BagGiveItemTransition give(role);
			give.SetReason(ItemFlow_GoldClick, ItemFlow_GoldClickGive);
			give.GiveItem(GOLD, ret*GetGlobalConfig().GoldClickBaseCount);
			give.NotifyClient();

			SSInfo<<"give free gold:"<<ret<<END;

			// mark
			record->UpdateGoldClickNextFreeTime();
			record->AddCount(true);

			role->OnBuy(GOLD);
		}
		else
		{
			roRes.set_errorcode(KKSG::ERR_FAILED);
		}
	}
	// cost
	else if (2 == roArg.type())
	{
		UINT32 times = 1 == roArg.count() ? 1 : 10;
		UINT32 temptimes = 0;
		bool iscostlimit = false;
		while (!record->CountLimit(false) && temptimes < times)
		{
			UINT32 ret = GetGoldClick(1, counts);
			// cost limit
			BagTakeItemTransition take(role);
			take.SetReason(ItemFlow_GoldClick, ItemFlow_GoldClickTake);
			if (!take.TakeItem(GetGlobalConfig().GoldClickConsume.seq[0], GetGlobalConfig().GoldClickConsume.seq[1]))
			{
				iscostlimit = true;
				take.RollBack();
				break;
			}
			take.NotifyClient();

			// give 
			BagGiveItemTransition give(role);
			give.SetReason(ItemFlow_GoldClick, ItemFlow_GoldClickGive);
			give.GiveItem(GOLD, ret * GetGlobalConfig().GoldClickBaseCount);
			give.NotifyClient();

			SSInfo<<"give cost gold:"<<ret<<END;

			// mark
			record->AddCount(false);
			++temptimes;

			role->OnBuy(GOLD);
		}
		if (0 == temptimes)
		{
			roRes.set_errorcode(iscostlimit ? KKSG::ERR_DRAGONCOIN_LIMIT : KKSG::ERR_GOLDCLICK_LIMIT);
		}
	}
	else
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	for (auto i = counts.begin(); i != counts.end(); ++i)
	{
		roRes.add_results(*i);
	}

	roRes.set_freetimeleft(record->GetFreeTimeLeft());
	roRes.set_freecount(record->GetFreeCount());
	roRes.set_freeallcount(record->GetAllFreeCount());
	roRes.set_count(record->GetCount());
	roRes.set_allcount(record->GetAllCount());
	
	return;
}

void RpcC2G_GoldClick::OnDelayReplyRpc(const GoldClickArg &roArg, GoldClickRes &roRes, const CUserData &roUserData)
{
}
