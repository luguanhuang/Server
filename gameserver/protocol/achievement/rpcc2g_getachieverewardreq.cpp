#include "pch.h"
#include "achievement/rpcc2g_getachieverewardreq.h"
#include "unit/rolemanager.h"
#include "gamelogic/achievev2Mgr.h"
#include "gamelogic/achievev2Record.h"
#include "table/AchievementV2Table.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2016/5/30 17:36:55

RPC_SERVER_IMPLEMETION(RpcC2G_GetAchieveRewardReq, GetAchieveRewardReq, GetAchieveRewardRes)

void RpcC2G_GetAchieveRewardReq::OnCall(const GetAchieveRewardReq &roArg, GetAchieveRewardRes &roRes)
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
	UINT32 aid = roArg.achieveid();
	///>SSInfo << "HandleGetAchieveRewardReq, session id:"<<m_sessionID<< ", roleid:" << roleid << ", AchieveID=" << aid << END;

	AchievementV2Table* pTable = AchieveV2Mgr::Instance()->GetTable();
	if( NULL == pTable )
	{
		SSError<<"achievev2 table is null, "<< "roleid=" << roleid << END;
		return;		
	}

	AchievementPointRewardTable* pRewardTable = AchieveV2Mgr::Instance()->GetRewardTable();
	if( NULL == pRewardTable)
	{
		SSError<<"achieve reward table is null, "<< "roleid=" << roleid << END;
		return;
	}

	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	if( NULL == record )
	{
		roRes.set_result(ERR_SUCCESS);	
		return; 
	}

	std::map<UINT32, STC_ACHIEVE_V2_INFO>::iterator itFind = record->m_mapAchieveInfo.find(aid);
	if( itFind == record->m_mapAchieveInfo.end() )
	{
		SSError<<"role have not get this achieve yet, "<< "RoleID="<<roleid << ", achieveid=" << aid << END;
		return; 
	}

	if( itFind->second.dwRewardStatus != REWARD_STATUS_CAN )
	{
		SSError<<"achieve reward status not right, "<< "RoleID="<<roleid << ", achieveid=" << aid << ", rewardstatus=" << itFind->second.dwRewardStatus << END;
		return; 
	}

	AchievementV2Table::RowData* pConf = pTable->GetByID(aid);
	if( NULL == pConf )
	{
		SSError<<"get config failed, "<< "RoleID="<<roleid << ", achieveid=" << aid << END;
		return; 
	}

	///>给予奖励
	BagGiveItemTransition transition(pRole);
	transition.SetReason(ItemFlow_AchieveV2, ItemFlow_AchieveV2_Achieve);
	int rewardcount = pConf->Reward.size();
	for (int i = 0; i < rewardcount; i++ )
	{
		UINT32 itemId = pConf->Reward[i][0];
		INT64 itemCount = (INT64)pConf->Reward[i][1];
		transition.GiveItem(itemId, itemCount, false);
	}
	transition.NotifyClient();

	itFind->second.dwRewardStatus = REWARD_STATUS_GOT;

	UINT32 curAchievePoint = 0; 
	if( pConf->Reward.size() > 0 )
	{
		curAchievePoint = pConf->Reward[0][1];
	}

	UINT32 totalAchievePoint = 0; 
	for( auto i=record->m_mapAchieveInfo.begin(); i!= record->m_mapAchieveInfo.end(); i++)
	{
		if( i->second.dwRewardStatus != REWARD_STATUS_GOT)
		{
			continue; 
		}
		AchievementV2Table::RowData* pInnerConf = pTable->GetByID(i->second.dwAchieveID);
		if( NULL == pInnerConf)
		{
			continue; 
		}
		if( pInnerConf->Reward.size() ==0)
		{
			continue; 
		}
		totalAchievePoint += pInnerConf->Reward[0][1];
	}

	for ( auto i=pRewardTable->Table.begin(); i!= pRewardTable->Table.end(); i++)
	{
		if( totalAchievePoint - curAchievePoint < (UINT32)(*i)->Point && totalAchievePoint >= (UINT32)(*i)->Point)
		{
			STC_ACHIEVE_POINT_REWARD_INFO stc;
			stc.dwRewardID = (*i)->ID;
			stc.dwRewardStatus = REWARD_STATUS_CAN;
			record->m_mapAchievePointRewardInfo.insert( std::make_pair(stc.dwRewardID, stc));
			SSInfo << "Insert Achieve Point Reward To Map, RoleID=" << pRole->GetID() << ", RewardID=" << (*i)->ID << END;
			AchieveV2Mgr::Instance()->HintNotify(pRole);
			SSInfo << "Finish Send Hint Notify For AchievePoint Reward, RoleID=" << pRole->GetID() << ", RewardID=" << stc.dwRewardID << END;
		}
	}

	record->m_ismodify.Set();
	roRes.set_result(ERR_SUCCESS);
	///>SSInfo << "Finish Handle GetAchieveRewardReq, session id:"<< m_sessionID << ", roleid:" << roleid << ", AchieveID=" << aid << END;
}

void RpcC2G_GetAchieveRewardReq::OnDelayReplyRpc(const GetAchieveRewardReq &roArg, GetAchieveRewardRes &roRes, const CUserData &roUserData)
{
}
