#include "pch.h"
#include "task/rpcm2g_costdonateitem.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/taskrecord.h"
#include "gamelogic/dailytaskrecord.h"
#include "table/globalconfig.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"
#include "table/TaskMgr.h"
#include "spactivity/spactivityhandler.h"

// generate by ProtoGen at date: 2016/11/16 15:21:25

RPC_SERVER_IMPLEMETION(RpcM2G_CostDonateItem, CostDonateItemArg, CostDonateItemRes)

void RpcM2G_CostDonateItem::OnCall(const CostDonateItemArg &roArg, CostDonateItemRes &roRes)
{
	roRes.set_result(KKSG::ERR_INVALID_REQUEST);
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!pRole)
	{
		return;
	}
	TaskRecord* record = pRole->Get<TaskRecord>();
	DailyTaskRecord& dailyRecord = record->GetDailyRecord();
	UINT32 donateNum = dailyRecord.GetTodayDonateCount();
	UINT32 num = roArg.itemcount() ? roArg.itemcount() : roArg.itemuid_size();
	if (num + donateNum > GetGlobalConfig().DailyTaskDonateNum)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	DailyTask::RowData* pData = TaskMgr::Instance()->GetDailyTask(roArg.taskid());
	if (pData == NULL)
	{
		roRes.set_result(KKSG::ERR_TASK_NOT_FOUND);
		return;
	}

	UINT32 itemQuality = 0;
	if (roArg.itemid())
	{
		if (pData->tasktype != KKSG::TaskConn_ItemID)
		{
			roRes.set_result(KKSG::ERR_TASK_NOT_TASK_ITEM);
			return;
		}
		UINT32 itemId = roArg.itemid();
		if (std::find(pData->conditionId.begin(), pData->conditionId.end(), itemId) == pData->conditionId.end())
		{
			roRes.set_result(KKSG::ERR_TASK_NOT_TASK_ITEM);
			return;
		}
		BagTakeItemTransition trans(pRole);
		trans.SetReason(ItemFlow_Guild, ItemFlow_Donate);
		if (!trans.TakeItem(roArg.itemid(), roArg.itemcount()))
		{
			trans.RollBack();
			roRes.set_result(KKSG::ERR_ITEM_NOT_ENOUGH);
			return;
		}
		trans.NotifyClient();
		roRes.add_itemid(roArg.itemid());
		roRes.add_itemcount(roArg.itemcount());
		itemQuality = ItemConfig::Instance()->GetItemQuailty(itemId);
	}
	else
	{
		if (pData->tasktype != KKSG::TaskConn_ItemTypeQuality)
		{
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}
		if (roArg.itemuid_size() == 0)
		{
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}
		std::map<UINT64, UINT32> itemUidCount;
		Bag* pBag = pRole->Get<Bag>();
		std::vector<UINT32> condIds;
		for (int i  = 0; i < roArg.itemuid_size(); ++i)
		{
			UINT64 itemUid = roArg.itemuid(i);
			itemUidCount[itemUid]++;
		}

		std::map<UINT32, UINT32> itemIdCount;
		for (auto it = itemUidCount.begin(); it != itemUidCount.end(); ++it)
		{
			UINT64 itemUid = it->first;
			UINT32 itemCount = it->second;
			XItem* pXItem = pBag->FindUniqueItemInBag(itemUid);
			if (pXItem == NULL)
			{
				roRes.set_result(KKSG::ERR_ITEM_NOTEXIST);
				return;
			}
			if (pXItem->itemCount < itemCount)
			{
				roRes.set_result(KKSG::ERR_ITEM_NOT_ENOUGH_COMMON);
				return;
			}
			ItemConf* pItemConf = ItemConfig::Instance()->GetItemConf(pXItem->itemID);
			if (pItemConf == NULL)
			{
				roRes.set_result(KKSG::ERR_ITEM_NOTEXIST);
				return;
			}
			UINT32 itemConnId = pItemConf->ItemType * 100 + pItemConf->ItemQuality;
			if (std::find(pData->conditionId.begin(), pData->conditionId.end(), itemConnId) == pData->conditionId.end())
			{
				roRes.set_result(KKSG::ERR_TASK_NOT_TASK_ITEM);
				return;
			}
			itemIdCount[pXItem->itemID] += itemCount;
			itemQuality = pItemConf->ItemQuality;
		}

		BagTakeItemTransition trans(pRole);
		trans.SetReason(ItemFlow_Guild, ItemFlow_Donate);
		for (auto it = itemUidCount.begin(); it != itemUidCount.end(); ++it)
		{
			if (!trans.TakeUniqueItem(it->first, it->second))
			{
				trans.RollBack();
				roRes.set_result(KKSG::ERR_ITEM_NOT_ENOUGH);
				return;
			}
		}
		trans.NotifyClient();

		for (auto it = itemIdCount.begin(); it != itemIdCount.end(); ++it)
		{
			roRes.add_itemid(it->first);
			roRes.add_itemcount(it->second);
		}
	}

	roRes.set_result(KKSG::ERR_SUCCESS);
	dailyRecord.AddDonateCount(num);
	roRes.set_todaydonatecount(dailyRecord.GetTodayDonateCount());
	roRes.set_totaldonatecount(dailyRecord.GetTotalDonateCount());

	pRole->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_Activity, KKSG::TaskActType_DonateItem, num);

	SpActivityHandler::GlobalHandler.OnGuildDonate(pRole, num);

	// 计算公会贡献
	UINT32 contribute = GetGlobalConfig().GetDonateByQuality(itemQuality) * num;
	// GS加贡献
	if (contribute)
	{
		roRes.set_contribute(contribute);
		BagGiveItemTransition giveTrans(pRole);
		giveTrans.SetReason(ItemFlow_Guild, ItemFlow_Donate);
		giveTrans.GiveItem(GUILD_CONTRIBUTE, contribute);
		giveTrans.NotifyClient();
	}
}

void RpcM2G_CostDonateItem::OnDelayReplyRpc(const CostDonateItemArg &roArg, CostDonateItemRes &roRes, const CUserData &roUserData)
{
}
