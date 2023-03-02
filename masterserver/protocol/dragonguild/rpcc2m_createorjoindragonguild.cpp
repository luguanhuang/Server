#include "pch.h"
#include "dragonguild/rpcc2m_createorjoindragonguild.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "dragonguild/dragonguildmgr.h"
#include "network/gslink.h"
#include "tsssdk/MsTssHandler.h"
#include "dragonguild/rpcm2g_createdragonguildcost.h"
#include "dragonguild/rpcm2g_getrolepower.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2017/9/4 20:43:58

RPC_SERVER_IMPLEMETION(RpcC2M_CreateOrJoinDragonGuild, CreateOrJoinDragonGuildArg, CreateOrJoinDragonGuildRes)

void RpcC2M_CreateOrJoinDragonGuild::OnCall(const CreateOrJoinDragonGuildArg &roArg, CreateOrJoinDragonGuildRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn << "pRole is NULL, session id: " << m_sessionID << END;
		return ;
	}

	if (!pRole->IsSystemOpened(SYS_DRAGONGUILD))
	{
		roRes.set_result(KKSG::ERR_SYS_NOTOPEN);
		return;
	}

	if (roArg.iscreate())
	{
		//请求腾讯sdk验证名字是否合法;
		KKSG::ErrorCode nErrCode = CTssHandler::Instance()->ValidNameCheck(roArg.dgname());
		if(nErrCode != KKSG::ERR_SUCCESS)
		{
			roRes.set_result(nErrCode);
			return;
		}

		UINT32 err = DragonGuildMgr::Instance()->CanCreateDragonGuild(pRole, roArg.dgname());
		if (err != KKSG::ERR_SUCCESS)
		{
			roRes.set_result((KKSG::ErrorCode)err);
			return ;
		}

		RpcM2G_CreateDragonGuildCost* pMSg = RpcM2G_CreateDragonGuildCost::CreateRpc();
		pMSg->m_oArg.set_iscreate(roArg.iscreate());
		pMSg->m_oArg.set_rpcid(DelayRpc());
		pMSg->m_oArg.set_roleid(pRole->GetID());
		GSLink::Instance()->SendToLine(pRole->GetGsLine(), *pMSg);
	}
	else
	{
		if (DragonGuildMgr::Instance()->GetDragonGuildIdByRole(pRole->GetID()) != INVAILD_DRAGONGUILD)
		{
			roRes.set_result(KKSG::ERR_DG_ALREADY_IN_DG);
			return ;
		}

		RpcM2G_GetRolePower* pMsg = RpcM2G_GetRolePower::CreateRpc();
		pMsg->m_oArg.set_roleid(pRole->GetID());
		pMsg->m_oArg.set_rpcid(DelayRpc());
		pMsg->m_oArg.set_sysid((UINT32)SYS_DRAGONGUILD);
		GSLink::Instance()->SendToLine(pRole->GetGsLine(), *pMsg);
		return;
	}
}

void RpcC2M_CreateOrJoinDragonGuild::OnDelayReplyRpc(const CreateOrJoinDragonGuildArg &roArg, CreateOrJoinDragonGuildRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn << "pRole is NULL, session id:" << m_sessionID << END;
		return;
	}

	if (roArg.iscreate())
	{
		if (roUserData.m_dwUserData == KKSG::ERR_SUCCESS)
		{
			// create dragonguild 
			DragonGuildMgr::Instance()->CreateDragonGuild(pRole,roArg.dgname());
		}
		else
		{
			roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
		}
	}
	else
	{
		if (*((UINT32 *)roUserData.m_pUserPtr) == KKSG::ERR_SUCCESS)
		{
			roRes.set_result((KKSG::ErrorCode)DragonGuildMgr::Instance()->JoinDragonGuild(pRole, roUserData.m_dwUserData, roArg.dgid()));

		}
		else
		{
			roRes.set_result((KKSG::ErrorCode)(*(UINT32 *)roUserData.m_pUserPtr));
			roRes.set_cdtime((UINT32)roUserData.m_dwUserData);
		}
	}

	DragonGuild* pDGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(pRole->GetID());
	if (pDGuild != NULL)
	{
		roRes.set_dgid(pDGuild->GetID());
		roRes.set_name(pDGuild->GetName());
	}
	else
	{
		roRes.set_dgid(0);
		roRes.set_name("");
	}
}
