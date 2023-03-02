#include "pch.h"
#include "designation/rpcc2g_getclassifydesignationreq.h"
#include "gamelogic/designationRecord.h"
#include "gamelogic/designationMgr.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "table/DesignationTable.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2016/5/30 12:33:29

RPC_SERVER_IMPLEMETION(RpcC2G_GetClassifyDesignationReq, GetClassifyDesignationReq, GetClassifyDesignationRes)

void RpcC2G_GetClassifyDesignationReq::OnCall(const GetClassifyDesignationReq &roArg, GetClassifyDesignationRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	DesignationTable* pTable = DesignationMgr::Instance()->GetTable();
	UINT32 dtype = roArg.type();
	for( auto iter=record->m_mapDesignationInfo.begin(); iter!= record->m_mapDesignationInfo.end(); iter++ )
	{
		DesignationTable::RowData* pData = pTable->GetByID(iter->second.dwDesignationID);
		if( NULL == pData||pData->Type != dtype )
			continue; 
		StcDesignationInfo* pInfo = roRes.add_datalist();
		pInfo->set_designationid(iter->second.dwDesignationID);
		pInfo->set_isnew(iter->second.bIsNew);
		std::string szName = record->GetName(iter->second.dwDesignationID);
		if (szName!="")
		{
			pInfo->set_name(szName);
		}else
		{
			pInfo->set_name(iter->second.szName);
		}

		UINT32 nReachTime = iter->second.dwReachTimestamp;
		if (pData->Type == DESIGNAION_BIG_TYPE_TIMELIMIT&&nReachTime)
		{
			UINT32 nLeftTime = 0;
			if (nReachTime+pData->Pragmaticality>GameTime::GetTime())
			{
				 nLeftTime = nReachTime+pData->Pragmaticality-GameTime::GetTime();
			}else
			{
				nLeftTime = 0;
			}			
			pInfo->set_reachtimestamp(nLeftTime);
		}
		iter->second.bIsNew = false;
	}
	roRes.set_result(ERR_SUCCESS);
}

void RpcC2G_GetClassifyDesignationReq::OnDelayReplyRpc(const GetClassifyDesignationReq &roArg, GetClassifyDesignationRes &roRes, const CUserData &roUserData)
{
}
