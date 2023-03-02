#include "pch.h"
#include "qa/rpcc2m_getqadatareq.h"
#include "qa/qamgr.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "qa/ptcm2g_giveanserqarewardntf.h"
#include "network/gslink.h"

// generate by ProtoGen at date: 2016/10/9 16:27:30

RPC_SERVER_IMPLEMETION(RpcC2M_GetQADataReq, GetQADataReq, GetQADataRes)

void RpcC2M_GetQADataReq::OnCall(const GetQADataReq &roArg, GetQADataRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);	
	if(NULL == role)
	{
		return ;
	}
	QARoom* room = QAMgr::Instance()->GetRoomByRoleId(role->GetID());
	if (!room)
	{
		SSError << "Get Room Failed, RoleID:" << role->GetID() << END;
		roRes.set_result(KKSG::ERR_QA_NOT_IN_TIME);
		return; 
	}
	room->GetQADataReq(role, roRes);
}

void RpcC2M_GetQADataReq::OnDelayReplyRpc(const GetQADataReq &roArg, GetQADataRes &roRes, const CUserData &roUserData)
{
}
