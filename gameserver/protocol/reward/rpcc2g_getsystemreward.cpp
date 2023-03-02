#include "pch.h"
#include "reward/rpcc2g_getsystemreward.h"
#include "unit/rolemanager.h"
#include "gamelogic/rewardrecord.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/rewardmgr.h"
#include "reward/ptcg2c_rewardchangedntf.h"
#include "table/OpenSystemMgr.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2015/7/9 10:44:22

RPC_SERVER_IMPLEMETION(RpcC2G_GetSystemReward, GetSystemRewardArg, GetSystemRewardRes)

void RpcC2G_GetSystemReward::OnCall(const GetSystemRewardArg &roArg, GetSystemRewardRes &roRes)
{
	///> system open
	//if (OpenSystemMgr::Instance()->IsIdipCloseSystem(SysFatigueId))
	//{
	//	roRes.set_errorcode(ERR_FAILED);
	//	SSInfo<<"system close"<<END;
	//	return;
	//}

	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}	

	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(SysFatigueId))
	{
		SSInfo<<"system close"<<END;
		roRes.set_errorcode(ERR_FAILED);
		return;
	}

	UINT64 id = roArg.rewarduniqueid();
	///> 一次性系统奖励
	//if (CRewardMgr::Instance()->RemoveFromDb(pRole->GetID(), id))
	//{
	//	roRes.set_errorcode(ERR_SUCCESS);
	//	return;
	//}
	if (id > MAXREWARDBIT)
	{
		roRes.set_errorcode(ERR_SUCCESS);
		return;
	}
	///> 重复系统奖励
	CRewardRecord* pRecord = pRole->Get<CRewardRecord>();
	if (NULL == pRecord)
	{
		SSWarn<<"pRecord is NULL, role id:"<<pRole->GetID()<<END;
		return;
	}
	SRewardInfo* pInfo = pRecord->GetRewardInfobyId(id);
	if (NULL == pInfo)
	{
		SSWarn<<"pInfo is NULL, unique id:"<<id<<END;
		roRes.set_errorcode(ERR_REWARD_NOTEXIST);
		return;
	}
	// update
	CRewardMgr::Instance()->PushReward(pRole, pInfo->dwType, true);

	// give
	std::vector<ItemDesc> vecItem = pInfo->vecItem;
	if (!pRecord->Pop(pInfo->qwUniqueId))
	{
		SSWarn<<"pop rewardinfo failed, unique id:"<<id<<END;
		roRes.set_errorcode(ERR_REWARD_LIMIT);
		return;
	}

	BagGiveItemTransition stTransition(pRole);
	stTransition.SetReason(ItemFlow_Reward, ItemFlow_Reward_System);
	for (auto i = vecItem.begin(); i != vecItem.end(); ++i)
	{
		stTransition.GiveItem(*i);
	}
	stTransition.NotifyClient();

	pRole->OnTakenSystemReward(KKSG::SystemRewardType(id));

	roRes.set_errorcode(ERR_SUCCESS);
	return;
}

void RpcC2G_GetSystemReward::OnDelayReplyRpc(const GetSystemRewardArg &roArg, GetSystemRewardRes &roRes, const CUserData &roUserData)
{
}
