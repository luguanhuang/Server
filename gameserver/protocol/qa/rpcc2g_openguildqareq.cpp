#include "pch.h"
#include "qa/rpcc2g_openguildqareq.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/QAMgr.h"
#include "qa/rpcg2m_createguildqaroom.h"
#include "define/qadef.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"

// generate by ProtoGen at date: 2016/7/9 14:31:43

RPC_SERVER_IMPLEMETION(RpcC2G_OpenGuildQAReq, OpenGuildQAReq, OpenGuildQARes)

void RpcC2G_OpenGuildQAReq::OnCall(const OpenGuildQAReq &roArg, OpenGuildQARes &roRes)
{
	roRes.set_result(ERR_UNKNOWN);
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn << "role is NULL, session id:" << m_sessionID <<END;
		return;
	}

	UINT64 roleID = role->GetID();
	UINT32 now = TimeUtil::GetTime();
	UINT32 type = QATYPE_GUILD;
	QAMgr* mgr = QAMgr::Instance();

	KKSG::ErrorCode code = KKSG::ERR_SUCCESS;
	do
	{
		// 开启不判断个人，只判断时间
		if (!mgr->IsTimeOk(type, now))
		{
			SSError << "time not ok" << END;
			code = KKSG::ERR_QA_NOT_IN_TIME;
			break;
		}

		CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildSimple(role->getGuildId());
		if(!guild)
		{
			SSInfo << "Role Enter failed, roleId: " << roleID << ", type: " << type << END;
			code = KKSG::ERR_QA_NO_GUILD;
			break;
		}
		if (mgr->HasGuildRoom(role->getGuildId()))
		{
			LogError("Alreay Have Guild QA Room, RoleID:%llu, GuildID:%llu", role->GetID(), guild->GetGuildId());
			code = KKSG::ERR_UNKNOWN;
			break;
		}

		// 不是会长，也不是副会长，则不能开启公会答题
		if (!guild->IsGuildLeader(roleID) && !guild->IsGuildVicLeader(roleID))
		{
			code = KKSG::ERR_GUILD_NO_PERMISSION;
			break;;
		}
	} while (0);

	if (code != KKSG::ERR_SUCCESS)
	{
		roRes.set_result(code);
		SSError << "Open Guild failed, roleId: " << roleID << ", code: " << code << END;
		return;
	}
	
	RpcG2M_CreateGuildQARoom* rpc = RpcG2M_CreateGuildQARoom::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpc_id(DelayRpc());
	role->SendToMS(*rpc);
}

void RpcC2G_OpenGuildQAReq::OnDelayReplyRpc(const OpenGuildQAReq &roArg, OpenGuildQARes &roRes, const CUserData &roUserData)
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

	KKSG::CreateGuildQARoomRes* res = (KKSG::CreateGuildQARoomRes*)(roUserData.m_pUserPtr);
	roRes.set_result(res->result());
}
