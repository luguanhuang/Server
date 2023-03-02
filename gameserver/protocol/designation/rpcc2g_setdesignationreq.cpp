#include "pch.h"
#include "designation/rpcc2g_setdesignationreq.h"
#include "gamelogic/designationRecord.h"
#include "gamelogic/designationMgr.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolesummarymgr.h"
#include "unit/roleoutlook.h"

// generate by ProtoGen at date: 2016/5/30 12:30:23

RPC_SERVER_IMPLEMETION(RpcC2G_SetDesignationReq, SetDesignationReq, SetDesignationRes)

void RpcC2G_SetDesignationReq::OnCall(const SetDesignationReq &roArg, SetDesignationRes &roRes)
{
	roRes.set_result(ERR_UNKNOWN);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	UINT64 roleID = pRole->GetID();
	///>SSInfo<<"HandleSetDesignationReq, RoleID="<<roleID<<", Type=" << roArg.type() << ", DesignationID=" << roArg.designationid() << ", optype=" << roArg.type() << END;

	DesignationRecord* record = pRole->Get<DesignationRecord>();
	if( NULL == record )
	{
		roRes.set_result(ERR_SUCCESS);
		return; 
	}
	
	//获取称号ID;
	UINT32 did = roArg.designationid();
	UINT32 optype = roArg.type();

	//既不是设置能力称号也不是设置封面称号;
	if( optype != 1 && optype != 2 )
	{
		SSError<<"set designation invalid op type, session id:"<<m_sessionID<<", RoleID:" << roleID << ", optype:" << optype << END;
		return; 
	}

	std::map<UINT32, stDesignationInfo>::iterator itFind = record->m_mapDesignationInfo.find(did);
	if( did > 0 && itFind == record->m_mapDesignationInfo.end())
	{
		SSError<<"cannot find desgiantionid for this role, session id:"<<m_sessionID<<", roleId:" << roleID << "designationid:" << did << END;
		return; 
	}

	//封面称号;
	if(optype == 1)
	{
		std::string szName =  record->SetCoverDesignation(did);
		roRes.set_name(szName);
		SSInfo << "Update Player Cover Designation, RoleID:" << pRole->GetID() << ", CoverID:" << record->GetCoverDesignation() << ",CovewrName" << szName << END;
	}

	///>能力称号: 会修改角色属性: 因为战力会变，所以也要更新summary
	if( optype == 2 )
	{
		record->SetAbilityDesignation(did);
		///>SSWarn << "Update Player Ability Designation, RoleID:" << pRole->GetID() << ", AbilityID:" << record->m_dwAbilityDesignationID << END;
	}

	//获取各大类红点列表;
	std::vector<bool> vecRedPoint;
	vecRedPoint.clear();
	DesignationMgr::Instance()->GetBigTypeRedPoint(pRole, vecRedPoint);
	for( UINT32 i=0; i<vecRedPoint.size(); i++ )
	{
		//roRes.set_datalist(vecRedPoint[i]);
		roRes.add_datalist(vecRedPoint[i]);
		///>SSInfo << "Set Designation Big Type, RoleID=" << pRole->GetID() << ", BigType=" << i+1 << ", RedPoint=" << vecRedPoint[i] << END; 
	}

	record->Change();
	roRes.set_result(ERR_SUCCESS);

	///>SSInfo<<"Finish Handle SetDesignationReq, RoleID="<<roleID<<", Type=" << roArg.type() << ", DesignationID=" << roArg.designationid() << ", optype=" << roArg.type() << END;
}

void RpcC2G_SetDesignationReq::OnDelayReplyRpc(const SetDesignationReq &roArg, SetDesignationRes &roRes, const CUserData &roUserData)
{
}
