#include "pch.h"
#include "qa/rpcg2m_createguildqaroom.h"
#include "qa/qamgr.h"
#include "role/rolemanager.h"
#include "role/role.h"

// generate by ProtoGen at date: 2016/10/8 16:07:14

RPC_SERVER_IMPLEMETION(RpcG2M_CreateGuildQARoom, CreateGuildQARoomReq, CreateGuildQARoomRes)

void RpcG2M_CreateGuildQARoom::OnCall(const CreateGuildQARoomReq &roArg, CreateGuildQARoomRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);	
	if(NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return ;
	}
	KKSG::ErrorCode code;
	QARoom* room = QAMgr::Instance()->CreateGuildQARoom(role, code);
	SSInfo << "Create Guild QA Room, roleId: " << role->GetID() << ", code: " << code << END;
	roRes.set_result(code);
	if (!room)
	{
		return;
	}
	roRes.set_room_id(room->GetID());
}

void RpcG2M_CreateGuildQARoom::OnDelayReplyRpc(const CreateGuildQARoomReq &roArg, CreateGuildQARoomRes &roRes, const CUserData &roUserData)
{
}
