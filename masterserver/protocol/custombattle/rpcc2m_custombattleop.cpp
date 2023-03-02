#include "pch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "network/worldlink.h"
#include "table/globalconfig.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattlemgr.h"
#include "custombattle/rpcc2m_custombattleop.h"

// generate by ProtoGen at date: 2017/4/7 21:30:43

RPC_SERVER_IMPLEMETION(RpcC2M_CustomBattleOp, CustomBattleOpArg, CustomBattleOpRes)

void RpcC2M_CustomBattleOp::OnCall(const CustomBattleOpArg &roArg, CustomBattleOpRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	if (!GetGlobalConfig().CustomBattleOpen)
	{
		SSWarn<<"system is not open"<<END;
		roRes.set_errorcode(KKSG::ERR_CUSTOM_NOTOPEN);
		return;
	}

	UINT32 delayid = DelayRpc();

	SSInfo<<"roleid:"<<role->GetID()<<" sessionid:"<<m_sessionID<<" op:"<<roArg.op()<<" uid:"<<roArg.uid()<<" configid:"<<roArg.config().configid()<<" delayid:"<<delayid<<END;

	CustomBattleReq req;
	req.op = roArg.op();
	req.uid = roArg.uid();
	req.conf = roArg.config();
	req.roleid = role->GetID();
	req.delayid = delayid; 
	req.iscross = roArg.querycross();
	req.battlename = roArg.name();
	req.token = roArg.token();
	req.password = roArg.password();
	if (!CustomBattleMgr::Instance()->IsOpen(role, req))
	{
		CUserData data;
		data.m_dwUserData = KKSG::ERR_SYS_NOTOPEN;
		data.m_pUserPtr = NULL;
		ReplyDelayRpc(delayid, data);
		return;
	}

	// cd
	if (!role->IsCustomBattleTimeout(req.op))
	{
		CUserData data;
		data.m_dwUserData = KKSG::ERR_ACCOUNT_ROLE_FULL;
		data.m_pUserPtr = NULL;
		ReplyDelayRpc(delayid, data);
		return;
	}
	role->SetCustomBattleReq(req.op);

	if (!CustomBattleMgr::Instance()->HandleClientReq(req))
	{
		CUserData data;
		data.m_dwUserData = KKSG::ERR_CUSTOM_NOTOPEN;
		data.m_pUserPtr = NULL;
		ReplyDelayRpc(delayid, data);
	}
}

void RpcC2M_CustomBattleOp::OnDelayReplyRpc(const CustomBattleOpArg &roArg, CustomBattleOpRes &roRes, const CUserData &roUserData)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	role->ClearCustomBattleReq(roArg.op());

	SSInfo<<"delay reply roleid:"<<role->GetID()<<" sessionid:"<<m_sessionID<<" op:"<<roArg.op()<<" uid:"<<roArg.uid()<<" configid:"<<roArg.config().configid()<<" delayid:"<<m_dwDelayID<<END;

	KKSG::ErrorCode errorcode = (KKSG::ErrorCode)roUserData.m_dwUserData;
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		SSWarn<<"delay reply errorcode:"<<errorcode<<END;
	}
	else
	{
		KKSG::CustomBattleClientInfo* clientinfo = (KKSG::CustomBattleClientInfo*)roUserData.m_pUserPtr;
		if (NULL != clientinfo)
		{
			roRes.mutable_info()->CopyFrom(*clientinfo);
		}
		else
		{
			SSInfo<<"client info is NULL, roleid:"<<role->GetID()<<" op:"<<roArg.op()<<" uid:"<<roArg.uid()<<" configid:"<<roArg.config().configid()<<END;
		}
	}
	roRes.set_errorcode(errorcode);
}
