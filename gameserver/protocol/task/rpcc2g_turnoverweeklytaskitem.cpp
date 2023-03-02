#include "pch.h"
#include "task/rpcc2g_turnoverweeklytaskitem.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/taskrecord.h"
#include "gamelogic/bag.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/bagtransition.h"
#include "task/rpcg2m_taskdonateg2m.h"

// generate by ProtoGen at date: 2017/9/7 16:26:06

RPC_SERVER_IMPLEMETION(RpcC2G_TurnOverWeeklyTaskItem, TurnOverWeeklyTaskItemArg, TurnOverWeeklyTaskItemRes)

void RpcC2G_TurnOverWeeklyTaskItem::OnCall(const TurnOverWeeklyTaskItemArg &roArg, TurnOverWeeklyTaskItemRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	TaskRecord* taskRecord = pRole->Get<TaskRecord>();
	WeeklyTaskRecord& wRecord = taskRecord->GetWeeklyRecord();
	WeeklyTaskOne* pSubTask = wRecord.GetSubTaskByIndex<WeeklyTaskOne>(roArg.index());
	if (pSubTask == NULL)
	{
		roRes.set_result(KKSG::ERR_TASK_NOT_FOUND);
		return;
	}
	if (!pSubTask->IsItemTypeTask())
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	if (pSubTask->step >= pSubTask->maxStep)
	{
		roRes.set_result(KKSG::ERR_TASK_ALREADY_FINISH);
		return;
	}
	if (pSubTask->step + roArg.itemuid_size() > pSubTask->maxStep)
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
	for (int i  = 0; i < roArg.itemuid_size(); ++i)
	{
		UINT64 uid = roArg.itemuid(i);
		itemUidCount[uid]++;
	}


	Bag* pBag = pRole->Get<Bag>();
	std::vector<UINT32> condIds;
	UINT32 itemConnId = 0;
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
		if (!pSubTask->IsTaskItem(pItemConf))
		{
			roRes.set_result(KKSG::ERR_TASK_NOT_TASK_ITEM);
			return;
		}
		condIds.push_back(itemConnId);
	}

	pSubTask->AddTaskStep(roArg.itemuid_size());
	if (pSubTask->askHelp)
	{
		if (pSubTask->step >= pSubTask->maxStep)
		{
			RpcG2M_TaskDonateG2M* rpc = RpcG2M_TaskDonateG2M::CreateRpc();
			rpc->m_sessionID = m_sessionID;
			rpc->m_oArg.set_type(TaskDonateOper_Remove);
			rpc->m_oArg.set_roleid(pRole->GetID());
			rpc->m_oArg.set_index(pSubTask->index);
			rpc->m_oArg.set_taskid(pSubTask->id);
			pRole->SendToMS(*rpc);
		}
		else
		{
			RpcG2M_TaskDonateG2M* rpc = RpcG2M_TaskDonateG2M::CreateRpc();
			rpc->m_sessionID = m_sessionID;
			rpc->m_oArg.set_type(TaskDonateOper_Update);
			rpc->m_oArg.set_tasktype(KKSG::PeriodTaskType_Weekly);
			rpc->m_oArg.set_roleid(pRole->GetID());
			rpc->m_oArg.set_index(pSubTask->index);
			rpc->m_oArg.set_itemcount(pSubTask->maxStep - pSubTask->step);
			rpc->m_oArg.set_taskid(pSubTask->id);
			pRole->SendToMS(*rpc);
		}
	}

	BagTakeItemTransition trans(pRole);
	trans.SetReason(ItemFlow_Task, ItemFlow_TaskTurnOver);
	for (auto it = itemUidCount.begin(); it != itemUidCount.end(); ++it)
	{
		trans.TakeUniqueItem(it->first, it->second);
	}
	trans.NotifyClient();

	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2G_TurnOverWeeklyTaskItem::OnDelayReplyRpc(const TurnOverWeeklyTaskItemArg &roArg, TurnOverWeeklyTaskItemRes &roRes, const CUserData &roUserData)
{
}
