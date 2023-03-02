#include "pch.h"
#include "qa/qamgr.h"
#include "qa/rpcg2m_enterqaroom.h"
#include "role/rolemanager.h"
#include "role/role.h"

// generate by ProtoGen at date: 2016/10/8 14:05:24

RPC_SERVER_IMPLEMETION(RpcG2M_EnterQARoom, EnterQARoomReq, EnterQARoomRes)

void RpcG2M_EnterQARoom::OnCall(const EnterQARoomReq &roArg, EnterQARoomRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);	
	if(NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return ;
	}
	KKSG::ErrorCode code;
	QARoom* room = QAMgr::Instance()->EnterQARoom(role, roArg.type(), code);
	SSInfo << "Enter QARoom, RoleId: " << role->GetID() << ", type: " << roArg.type() << ", code: " << code << END;
	if (!room)
	{
		roRes.set_result(code);
		return;
	}
	roRes.set_result(code);
	roRes.set_room_id(room->GetID());
	roRes.set_end_time(room->GetEndTime());
}

void RpcG2M_EnterQARoom::OnDelayReplyRpc(const EnterQARoomReq &roArg, EnterQARoomRes &roRes, const CUserData &roUserData)
{
}
