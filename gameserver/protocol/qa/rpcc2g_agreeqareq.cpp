#include "pch.h"
#include "qa/rpcc2g_agreeqareq.h"
#include "gamelogic/QAMgr.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/QARecord.h"
#include "qa/rpcg2m_enterqaroom.h"
#include "idip/idiprecord.h"


// generate by ProtoGen at date: 2016/6/23 19:46:36

RPC_SERVER_IMPLEMETION(RpcC2G_AgreeQAReq, AgreeQAReq, AgreeQARes)

void RpcC2G_AgreeQAReq::OnCall(const AgreeQAReq &roArg, AgreeQARes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	UINT64 roleID = role->GetID();
	bool agree = roArg.agree();
	UINT32 type = roArg.type();
	SSInfo<< "HandleAgreeReq, RoleID: " << roleID <<", Agree: " << agree << ", Type: " << type << END;

	QARecord* record = role->Get<QARecord>();
	// 拒绝 则不创建房间, 直接返回
	if (!agree)
	{
		record->SetTrigger(type);
		return ;
	}

	if (type == QATYPE_GUILD || type == QATYPE_MULTI)
	{
		CIdipRecord* idip = role->Get<CIdipRecord>();
		if (idip->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
		{
			roRes.set_result(ERR_INVALID_REQUEST);
			return;
		}
	}

	QAMgr* mgr = QAMgr::Instance();

	// 检查
	KKSG::ErrorCode code = mgr->CanRoleEnter(role, type);
	if (code != KKSG::ERR_SUCCESS)
	{
		SSInfo << "Role Enter failed, roleId: " << roleID << ", type: " << type << END;
		roRes.set_result(code);
		return;
	}
	// 公会的需要检查房间
	if (QATYPE_GUILD == type && !mgr->HasGuildRoom(role->getGuildId()))
	{
		SSError << "Role Enter guild QA room failed, no room" << END;
		roRes.set_result(KKSG::ERR_QA_NO_GUILD_ROOM);
		return;
	}

	RpcG2M_EnterQARoom* rpc = RpcG2M_EnterQARoom::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpc_id(DelayRpc());
	rpc->m_oArg.set_type(type);
	role->SendToMS(*rpc);
}

void RpcC2G_AgreeQAReq::OnDelayReplyRpc(const AgreeQAReq &roArg, AgreeQARes &roRes, const CUserData &roUserData)
{
	if(roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
	{
		roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
		return;
	}
	if (!roUserData.m_pUserPtr)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is out, session id:"<<m_sessionID<<END;
		return;
	}

	KKSG::EnterQARoomRes* res = (KKSG::EnterQARoomRes*)(roUserData.m_pUserPtr);
	roRes.set_result(res->result());
	if (KKSG::ERR_SUCCESS != res->result())
	{
		return;
	}

	UINT32 type = roArg.type();
	// 记录
	QARecord* record = role->Get<QARecord>();
	record->SetTrigger(type);
	record->AddQACount(type);
	record->SetCurQA(type, res->end_time());

	role->OnQA(type);

	SSInfo << "Finish HandleAgreeQAReq, RoleID:" << role->GetID() << ", type: " << type << END;
}
