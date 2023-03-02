#include "pch.h"
#include "guild/rpcc2m_createorenterguild.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/rpcm2g_createguildcost.h"
#include "guild/rpcm2g_enterguildpower.h"
#include "network/gslink.h"
#include "tsssdk/MsTssHandler.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2016/8/26 16:31:58

RPC_SERVER_IMPLEMETION(RpcC2M_CreateOrEnterGuild, CreateOrJoinGuild, CreateOrJoinGuildRes)

void RpcC2M_CreateOrEnterGuild::OnCall(const CreateOrJoinGuild &roArg, CreateOrJoinGuildRes &roRes)
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
		//请求腾讯sdk验证名字是否合法;
		KKSG::ErrorCode nErrCode = CTssHandler::Instance()->ValidNameCheck(roArg.gname());
		if(nErrCode != KKSG::ERR_SUCCESS)
		{
			roRes.set_result(nErrCode);
			return;
		}
		/*
		if(bFlag && ForbidWordMgr::Instance()->HaveForbidWord(roArg.gname()))
		{
			bFlag = false;
			roRes.set_result(KKSG::ERR_WORD_FORBID);
		}*/
		/*
		else if(ForbidWordMgr::Instance()->HaveIllegalCode(roArg.gname()))
		{
			roRes.set_result(KKSG::ERR_ILLEGAL_CODE);
		}
		*/
		//if (bFlag)
		//{
		UINT32 err = CGuildMgr::Instance()->CanCreateGuild(pRole, roArg.gname(), roArg.icon()); 
		if (err != KKSG::ERR_SUCCESS)
		{
			roRes.set_result((KKSG::ErrorCode)err);
			return ;
		}
		RpcM2G_CreateGuildCost* pMsg =  RpcM2G_CreateGuildCost::CreateRpc();
		pMsg->m_oArg.set_rpcid(DelayRpc());
		pMsg->m_oArg.set_iscreate(roArg.iscreate());
		pMsg->m_oArg.set_roleid(pRole->GetID());
		GSLink::Instance()->SendToLine(pRole->GetGsLine(), *pMsg);
		return ;
		//	}
	}
	else 
	{
		RpcM2G_EnterGuildPower* pMsg =  RpcM2G_EnterGuildPower::CreateRpc();
		pMsg->m_oArg.set_rpcid(DelayRpc());
		pMsg->m_oArg.set_iscreate(roArg.iscreate());
		pMsg->m_oArg.set_roleid(pRole->GetID());
		GSLink::Instance()->SendToLine(pRole->GetGsLine(), *pMsg);
		return ;
		//roRes.set_result((KKSG::ErrorCode)CGuildMgr::Instance()->JoinGuild(pRole, roArg.gid()));
	}
}

void RpcC2M_CreateOrEnterGuild::OnDelayReplyRpc(const CreateOrJoinGuild &roArg, CreateOrJoinGuildRes &roRes, const CUserData &roUserData)
{

	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn << "pRole is NULL, session id:" << m_sessionID << END;
		return;
	}

	if (roArg.iscreate())  // 创建公会
	{
		if (roUserData.m_dwUserData == KKSG::ERR_SUCCESS)
		{
			roRes.set_result((KKSG::ErrorCode)CGuildMgr::Instance()->CreateGuild(pRole, roArg.gname(), roArg.icon()));
		}
		else
		{
			roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
		}
	}
	else  // 加入
	{
		if (*((UINT32*)roUserData.m_pUserPtr) == KKSG::ERR_SUCCESS)
		{
			roRes.set_result((KKSG::ErrorCode)CGuildMgr::Instance()->JoinGuild(pRole->GetID(), roUserData.m_dwUserData, roArg.gid()));
		}
		else
		{
			roRes.set_result((KKSG::ErrorCode)(*((UINT32*)roUserData.m_pUserPtr)));
		}
	}


	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild != NULL)
	{
		roRes.set_gid(pGuild->GetID());
		roRes.set_name(pGuild->GetName());

		CGuildMgr::Instance()->HintNotify(pRole);		
	}
	else
	{
		roRes.set_gid(0);
		roRes.set_name("");
	}	
}
