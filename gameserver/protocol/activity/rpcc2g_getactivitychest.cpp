#include "pch.h"
#include "activity/rpcc2g_getactivitychest.h"
#include "gamelogic/activitymgr.h"
#include "gamelogic/activityrecord.h"
#include "unit/rolemanager.h"
#include "gamelogic/item.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2015/6/27 12:23:39

RPC_SERVER_IMPLEMETION(RpcC2G_GetActivityChest, GetActivityChestArg, GetActivityChestRes)

void RpcC2G_GetActivityChest::OnCall(const GetActivityChestArg &roArg, GetActivityChestRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	CActivityRecord* pRecord = pRole->Get<CActivityRecord>();
	
	CActivityMgr::Instance()->UpdateRecord(pRecord, pRole);

	if (roArg.chestindex() >= CActivityMgr::activityChestNum)
	{
		SSWarn<<"chest index out of range:"<<roArg.chestindex()<<END;
		roRes.set_chestgetinfo(pRecord->GetChestGetInfo());
		roRes.set_errorcode(KKSG::ERR_ACTIVITY_NOCHESTINDEX);
		return;
	}

	///> 是否已经领取
	//UINT32 dwChestIndex = CActivityMgr::Instance()->GetChestIndex(pRecord, roArg.chestindex());
	if (pRecord->IsRecived(roArg.chestindex()))
	{
		roRes.set_errorcode(KKSG::ERR_ACTIVITY_HASGETCHEST);
		roRes.set_chestgetinfo(pRecord->GetChestGetInfo());
		return;
	}
	/*///> 特殊宝箱条件判断
	if (eSpecialChestMask == dwChestIndex)
	{
		if (!CActivityMgr::Instance()->IsFinishActivity(pRecord, pRecord->GetDoubleId()))
		{
			roRes.set_errorcode(ERR_ACTIVITY_SPECIALCHESTCOUNTLIMIT);
			roRes.set_chestgetinfo(pRecord->GetChestGetInfo());
			return;
		}
	}*/
	///> 是否达到完成条件
	UINT32 activityValue = pRecord->GetActivityAllValue(); 
	if (roArg.chestindex() >= GetGlobalConfig().ActivityChestExp.size() - GetGlobalConfig().ActivityChestExpWeekly.size())
	{
		activityValue = pRecord->GetWeekValue(); 
	}

	std::vector<ItemDesc> vecItem;
	if (!CActivityMgr::Instance()->GetChestItem(activityValue, pRole->GetLevel(), roArg.chestindex(), vecItem))
	{
		roRes.set_errorcode(KKSG::ERR_ACTIVITY_NOTENOUGHVALUE);
		roRes.set_chestgetinfo(pRecord->GetChestGetInfo());
		return;
	}
	///> 标记已领取
	pRecord->SetChestGetInfo(roArg.chestindex());
	///> 给宝箱
	BagGiveItemTransition stTransition(pRole);
	stTransition.SetReason(ItemFlow_Activity, ItemFlow_Activity_Day);
	for (auto i = vecItem.begin(); i != vecItem.end(); ++i)
	{
		stTransition.GiveItem(*i);
		roRes.add_itemid(i->itemID);
		roRes.add_itemcount(i->itemCount);
	}
	stTransition.NotifyClient();

	roRes.set_chestgetinfo(pRecord->GetChestGetInfo());
	roRes.set_errorcode(KKSG::ERR_SUCCESS);

	pRole->OnGetActivityChest(roArg.chestindex());

	return;
}

void RpcC2G_GetActivityChest::OnDelayReplyRpc(const GetActivityChestArg &roArg, GetActivityChestRes &roRes, const CUserData &roUserData)
{
}
