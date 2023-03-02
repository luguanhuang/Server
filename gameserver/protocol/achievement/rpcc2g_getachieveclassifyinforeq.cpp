#include "pch.h"
#include "achievement/rpcc2g_getachieveclassifyinforeq.h"
#include "unit/rolemanager.h"
#include "gamelogic/achievev2Mgr.h"
#include "gamelogic/achievev2Record.h"
#include "table/AchievementV2Table.h"


// generate by ProtoGen at date: 2016/5/30 12:45:35

RPC_SERVER_IMPLEMETION(RpcC2G_GetAchieveClassifyInfoReq, GetAchieveClassifyInfoReq, GetAchieveClassifyInfoRes)

void RpcC2G_GetAchieveClassifyInfoReq::OnCall(const GetAchieveClassifyInfoReq &roArg, GetAchieveClassifyInfoRes &roRes)
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
	UINT32 type = roArg.type();
	///>SSDebug<<"HandleGetAchieveClassifyInfoReq, session id:"<<m_sessionID<< ", roleid:" << roleid << ", Type=" << type << END;

	AchievementV2Table* pTable = AchieveV2Mgr::Instance()->GetTable();
	if( NULL == pTable )
	{
		SSError<<"achievev2 table is null, "<< "roleid=" << roleid << END;
		return;		
	}

	AchieveV2Record* record = pRole->Get<AchieveV2Record>();
	if( NULL == record )
	{
		roRes.set_result(ERR_SUCCESS);	
		return; 
	}

	for( auto it = record->m_mapAchieveInfo.begin(); it != record->m_mapAchieveInfo.end(); it++ )
	{
		AchievementV2Table::RowData* pData = pTable->GetByID(it->first);
		if( NULL == pData )
		{
			continue; 
		}
		if( pData->Type != (int)type )
		{
			continue;
		}
		StcAchieveInfo* pInfo = roRes.add_datalist();
		pInfo->set_achieveid(it->first);
		pInfo->set_rewardstatus(it->second.dwRewardStatus);
		///>SSInfo<<"Classify Reward Info, BigType="<< type << ", roleid:" << roleid << ", aid=" << pInfo->achieveid() << ", rewardstatus=" << pInfo->rewardstatus() << END;	
	}
	roRes.set_result(ERR_SUCCESS);
	///>SSDebug << "Finish Handle GetAchieveClassifyInfoReq, session id:"<<m_sessionID<< ", roleid:" << roleid << ", Type=" << type << ", ListSize=" << roRes.datalist_size() << END;	
}

void RpcC2G_GetAchieveClassifyInfoReq::OnDelayReplyRpc(const GetAchieveClassifyInfoReq &roArg, GetAchieveClassifyInfoRes &roRes, const CUserData &roUserData)
{
}
