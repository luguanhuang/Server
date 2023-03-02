#include "pch.h"
#include "achievement/rpcc2g_getachievepointrewardreq.h"
#include "unit/rolemanager.h"
#include "gamelogic/achievev2Mgr.h"
#include "gamelogic/achievev2Record.h"
#include "table/AchievementV2Table.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2016/6/4 11:52:47

RPC_SERVER_IMPLEMETION(RpcC2G_GetAchievePointRewardReq, GetAchievePointRewardReq, GetAchievePointRewardRes)

void RpcC2G_GetAchievePointRewardReq::OnCall(const GetAchievePointRewardReq &roArg, GetAchievePointRewardRes &roRes)
{
	roRes.set_result(ERR_UNKNOWN);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	UINT64 roleid = pRole->GetID();
	UINT32 rid = roArg.rewardid();
	///>SSInfo << "HandleGetAchievePointRewardReq, session id:"<<m_sessionID<< ", roleid:" << roleid << ", RewardID=" << rid << END;

	AchievementPointRewardTable* pTable = AchieveV2Mgr::Instance()->GetRewardTable();
	if( NULL == pTable )
	{
		SSError<<"achieve point reward table is null, "<< "roleid=" << roleid << END;
		return;		
	}

	AchievementPointRewardTable::RowData* pConf = pTable->GetByID(rid);
	if( NULL == pConf )
	{
		SSError<<"achieve point reward config not exist, "<< "roleid=" << roleid << ", rewardid=" << rid << END;
		return; 
	}

	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	if( NULL == record )
	{
		roRes.set_result(ERR_SUCCESS);	
		return; 
	}

	std::map<UINT32, STC_ACHIEVE_POINT_REWARD_INFO>::iterator itFind = record->m_mapAchievePointRewardInfo.find(rid);
	if( itFind == record->m_mapAchievePointRewardInfo.end() )
	{
		SSError << "RewardID not Exist IN Role AchievePoint Reward List, RoleID=" << pRole->GetID() << ", RewardID=" << rid << END;
		return; 
	}

	if( itFind->second.dwRewardStatus != REWARD_STATUS_CAN)
	{
		SSError << "Reward Status Not Right, RoleID=" << pRole->GetID() << ", RewardID=" << rid << ", RewardStatus=" << itFind->second.dwRewardStatus << END;
		return; 
	}

	///>给予奖励
	BagGiveItemTransition transition(pRole);
	transition.SetReason(ItemFlow_AchieveV2, ItemFlow_AchieveV2_AchievePoint);
	int rewardcount = pConf->Reward.size();
	for (int i = 0; i < rewardcount; i++ )
	{
		UINT32 itemId = pConf->Reward[i][0];
		INT64 itemCount = (INT64)pConf->Reward[i][1];
		transition.GiveItem(itemId, itemCount, false);
	}
	transition.NotifyClient();

	itFind->second.dwRewardStatus = REWARD_STATUS_GOT;
	record->m_ismodify.Set();
	roRes.set_result(ERR_SUCCESS);
	///>SSInfo << "Finish Send SC Message CMD=GetAchievePointRewardReq, session id:"<<m_sessionID<< ", roleid:" << roleid << ", RewardID=" << rid << END;
}

void RpcC2G_GetAchievePointRewardReq::OnDelayReplyRpc(const GetAchievePointRewardReq &roArg, GetAchievePointRewardRes &roRes, const CUserData &roUserData)
{
}
