#include "pch.h"
#include "task/rpcm2g_dailytaskrefreshm2g.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/taskrecord.h"
#include "define/taskdef.h"
#include "table/globalconfig.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2017/9/19 15:53:20

RPC_SERVER_IMPLEMETION(RpcM2G_DailyTaskRefreshM2G, DailyTaskRefreshM2GArg, DailyTaskRefreshM2GRes)

void RpcM2G_DailyTaskRefreshM2G::OnCall(const DailyTaskRefreshM2GArg &roArg, DailyTaskRefreshM2GRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	DailyTaskRecord& record = pRole->Get<TaskRecord>()->GetDailyRecord();
	if (roArg.type() == TaskRefreshType_UpdateLuck)
	{
		record.SetLuck(roArg.luck());
	}
	else if (roArg.type() == TaskRefreshType_BuyRefreshCount)
	{
		BagTakeItemTransition trans(pRole);	
		trans.SetReason(ItemFlow_Task, ItemFlow_BuyDailyTaskRefreshCount);
		UINT32 cost = GetGlobalConfig().GetDailyTaskRefreshCost(roArg.count());
		if (!trans.TakeItem(DRAGON_COIN, cost))
		{
			trans.RollBack();
			roRes.set_result(KKSG::ERR_ITEM_NOT_ENOUGH_COMMON);
			return;
		}
		trans.NotifyClient();
	}
	else
	{
		roRes.set_result(KKSG::ERR_FAILED);
	}

}

void RpcM2G_DailyTaskRefreshM2G::OnDelayReplyRpc(const DailyTaskRefreshM2GArg &roArg, DailyTaskRefreshM2GRes &roRes, const CUserData &roUserData)
{
}
