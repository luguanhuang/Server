#include "pch.h"
#include "guild/rpcc2m_addguildinherit.h"
#include "guild/rpcm2g_tryaddguildinherit.h"
#include "guild/guildinherit.h"
#include "role/role.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2016/11/17 15:39:53

RPC_SERVER_IMPLEMETION(RpcC2M_AddGuildInherit, AddGuildInheritArg, AddGuildInheritRes)

void RpcC2M_AddGuildInherit::OnCall(const AddGuildInheritArg &roArg, AddGuildInheritRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}

	if (!CGuildInherit::Instance()->IsMapRight(pRole->GetID(), roArg.reqroleid()))
	{
		roRes.set_errorcode(ERR_GUILD_INHERIT_MAP_WRONG);
		return;
	}

	UINT64 reqRoleId = roArg.reqroleid();
	if (pRole->GetID() == reqRoleId)
	{
		roRes.set_errorcode(ERR_GUILD_INHERIT_CAN_NOT);
		return ;
	}

	if (CGuildInherit::Instance()->IsInherit(reqRoleId, pRole->GetID()))
	{
		roRes.set_errorcode(ERR_SUCCESS);
		return ;
	}

	UINT32 errorCode = CGuildInherit::Instance()->CanInherit(pRole->GetID(), reqRoleId);
	if (errorCode != ERR_SUCCESS)
	{
		roRes.set_errorcode((ErrorCode)errorCode);
		return ;
	}

	RpcM2G_tryAddGuildInherit* pMsg = RpcM2G_tryAddGuildInherit::CreateRpc();
	pMsg->m_oArg.set_roleone(pRole->GetID());
	pMsg->m_oArg.set_roletwo(reqRoleId);
	pMsg->m_oArg.set_rpcid(DelayRpc());
	pMsg->m_oArg.set_isdo(false);

	pRole->SendMsgToGS(*pMsg);
}

void RpcC2M_AddGuildInherit::OnDelayReplyRpc(const AddGuildInheritArg &roArg, AddGuildInheritRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
	{
		roRes.set_errorcode((KKSG::ErrorCode)roUserData.m_dwUserData);
		return ;
	}

	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}

	CGuildInherit::Instance()->AddInherit(roArg.reqroleid(), pRole->GetID());
	roRes.set_errorcode(ERR_SUCCESS);
}
