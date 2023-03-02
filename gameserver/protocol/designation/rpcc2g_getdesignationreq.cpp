#include "pch.h"
#include "designation/rpcc2g_getdesignationreq.h"
#include "gamelogic/designationRecord.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/designationMgr.h"
#include "table/DesignationTable.h"
#include <map>

// generate by ProtoGen at date: 2016/5/30 12:26:11

RPC_SERVER_IMPLEMETION(RpcC2G_GetDesignationReq, GetDesignationReq, GetDesignationRes)

void RpcC2G_GetDesignationReq::OnCall(const GetDesignationReq &roArg, GetDesignationRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}
	UINT64 roleid = pRole->GetID();
	DesignationRecord* record = pRole->Get<DesignationRecord>();
	DesignationTable* pTable = DesignationMgr::Instance()->GetTable();
	if( NULL == pTable )
	{
		roRes.set_result(ERR_UNKNOWN);
		SSError<<"designation table is null, session id:"<<m_sessionID<< ", roleid:" << roleid << END;
		return; 
	}
	UINT32 maxppt = DesignationMgr::Instance()->GetMaxPPT(pRole);
	roRes.set_abilitydesignationid(record->GetAbilityDesignation());
	roRes.set_coverdesignationid(record->GetCoverDesignation());
	roRes.set_name(record->GetName());
	roRes.set_maxppt(maxppt);
	std::vector<bool> vecBigTypeRedPoint;
	DesignationMgr::Instance()->GetBigTypeRedPoint(pRole, vecBigTypeRedPoint);
	for( UINT32 i=0; i<vecBigTypeRedPoint.size(); i++ )
	{
		roRes.add_datalist(vecBigTypeRedPoint[i]);
	}
	roRes.set_result(ERR_SUCCESS);
}

void RpcC2G_GetDesignationReq::OnDelayReplyRpc(const GetDesignationReq &roArg, GetDesignationRes &roRes, const CUserData &roUserData)
{
}
