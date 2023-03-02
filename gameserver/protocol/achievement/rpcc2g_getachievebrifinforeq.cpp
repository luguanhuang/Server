#include "pch.h"
#include "achievement/rpcc2g_getachievebrifinforeq.h"
#include "gamelogic/achievev2Mgr.h"
#include "gamelogic/achievev2Record.h"
#include "table/AchievementV2Table.h"
#include "foreach.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2016/5/30 12:44:55

RPC_SERVER_IMPLEMETION(RpcC2G_GetAchieveBrifInfoReq, GetAchieveBrifInfoReq, GetAchieveBrifInfoRes)

void RpcC2G_GetAchieveBrifInfoReq::OnCall(const GetAchieveBrifInfoReq &roArg, GetAchieveBrifInfoRes &roRes)
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
	///>SSDebug<<"HandleGetAchieveBrifInfoReq, session id:"<<m_sessionID<< ", roleid:" << roleid << END;

	AchievementV2Table* pTable = AchieveV2Mgr::Instance()->GetTable();
	if( NULL == pTable )
	{
		SSError<<"achievev2 table is null, "<< "roleid=" << roleid << END;
		return;		
	}

	AchievementPointRewardTable* pRewardTable = AchieveV2Mgr::Instance()->GetRewardTable();
	if( NULL == pRewardTable )
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

	std::map<UINT32, UINT32> mapGet;
	mapGet.clear();
	std::map<UINT32, UINT32> mapTotal;
	mapTotal.clear();
	std::map<UINT32, UINT32> mapCanRewardCount;
	mapCanRewardCount.clear();
	for( int i=ACHIEVE_TYPE_COMMON; i<= ACHIEVE_TYPE_ACTIVITY; i++ )
	{
		mapGet.insert( std::make_pair(i, 0) );
		mapTotal.insert( std::make_pair(i, 0));
		mapCanRewardCount.insert( std::make_pair(i, 0));
	}

	UINT32 roleAchievePoint = 0; 
	for( auto i=record->m_mapAchieveInfo.begin(); i!= record->m_mapAchieveInfo.end(); i++ )
	{
		AchievementV2Table::RowData* cfg = pTable->GetByID(i->first);
		if( NULL == cfg )
		{
			continue; 
		}
		if( cfg->Reward.size() == 0)
		{
			continue; 
		}

		if( i->second.dwRewardStatus == REWARD_STATUS_GOT )
		{
			roleAchievePoint += cfg->Reward[0][1];
		}
	
		std::map<UINT32, UINT32>::iterator itFind = mapGet.find( cfg->Type );
		if( itFind == mapGet.end() )
		{
			continue; 
		}

		if( i->second.dwRewardStatus == REWARD_STATUS_GOT)
		{
			itFind->second += cfg->Reward[0][1];
		}
	
		std::map<UINT32, UINT32>::iterator itRewardFind = mapCanRewardCount.find(cfg->Type);
		if( itRewardFind == mapCanRewardCount.end() )
		{
			continue; 
		}

		if( i->second.dwRewardStatus != REWARD_STATUS_CAN )
		{
			continue; 
		}

		itRewardFind->second += 1; 
	}

	UINT32 configAchievePoint = 0; 
	for( auto it = pRewardTable->Table.begin(); it != pRewardTable->Table.end(); it++ )
	{
		if( (*it)->Point <= (int)roleAchievePoint)
		{
			continue; 
		}

		configAchievePoint = (*it)->Point;
		break; 
	}

	roRes.set_result(ERR_SUCCESS);
	roRes.set_achievepoint(roleAchievePoint);
	roRes.set_maxachievepoint(configAchievePoint);

	for( auto it = pTable->Table.begin(); it != pTable->Table.end(); it++ )
	{
		std::map<UINT32, UINT32>::iterator itFind = mapTotal.find((*it)->Type);
		if( itFind == mapTotal.end() )
		{
			continue; 
		}
		if(  (*it)->Reward.size() == 0 )
		{
			continue; 
		}
		itFind->second += (*it)->Reward[0][1];
	}

	for( int i=ACHIEVE_TYPE_COMMON; i<=ACHIEVE_TYPE_ACTIVITY; i++ )
	{
		AchieveBriefInfo* pInfo = roRes.add_datalist();
		pInfo->set_achieveclassifytype(i);
		pInfo->set_achievepoint(mapGet[i]);
		pInfo->set_maxachievepoint(mapTotal[i]);
		pInfo->set_canrewardcount(mapCanRewardCount[i]);
		///>SSInfo << "Achieve Brief Info, RoleID=" << pRole->GetID() << ", BigType=" << i << ", achievepoint=" << mapGet[i] << ", totalpoint=" << mapTotal[i] << ", canrewardcount=" << mapCanRewardCount[i] << END;
	}

	///>取出第一个没有领取的成就点数奖励
	UINT32 firstNotGetRewardId = 0; 
	auto pRbegin = pRewardTable->Table.rbegin();
	UINT32 maxCfgId = 0; 
	if( pRbegin != pRewardTable->Table.rend() )
	{
		maxCfgId = (*pRbegin)->ID;
	}
	if( record->m_mapAchievePointRewardInfo.size() ==0 )
	{
		firstNotGetRewardId = (*(pRewardTable->Table.begin()))->ID;
	}
	if( record->m_mapAchievePointRewardInfo.size() > 0 )
	{
		for( auto i=record->m_mapAchievePointRewardInfo.begin(); i != record->m_mapAchievePointRewardInfo.end(); i++ )
		{
			if( i->second.dwRewardStatus == REWARD_STATUS_GOT )
			{
				continue; 
			}

			firstNotGetRewardId = i->second.dwRewardID;
			break; 
		}

		///>全部领取过
		if( firstNotGetRewardId == 0 )
		{
			firstNotGetRewardId = record->m_mapAchievePointRewardInfo.rbegin()->second.dwRewardID + 1; 
		}

		if( firstNotGetRewardId > maxCfgId )
		{
			firstNotGetRewardId = 0; 
		}
	}
	roRes.set_rewardid(firstNotGetRewardId);
	///>SSDebug << "Finish Send SC Message, CMD=RpcC2G_GetAchieveBrifInfoReq, RoleID=" << pRole->GetID() << ", ListSize=" << roRes.datalist_size() << ", PointRewardID=" << firstNotGetRewardId <<END;
}

void RpcC2G_GetAchieveBrifInfoReq::OnDelayReplyRpc(const GetAchieveBrifInfoReq &roArg, GetAchieveBrifInfoRes &roRes, const CUserData &roUserData)
{
}
