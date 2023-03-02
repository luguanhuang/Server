#include "pch.h"
#include "guild/rpcc2m_aceptguildinherit.h"
#include "guild/rpcm2g_tryaddguildinherit.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guildinherit.h"

// generate by ProtoGen at date: 2016/11/17 17:06:08

RPC_SERVER_IMPLEMETION(RpcC2M_AceptGuildInherit, AceptGuildInheritArg, AceptGuildInheritRes)

void RpcC2M_AceptGuildInherit::OnCall(const AceptGuildInheritArg &roArg, AceptGuildInheritRes &roRes)
{

	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}

	if (!CGuildInherit::Instance()->IsMapRight(pRole->GetID(), roArg.roleid()))
	{
		CGuildInherit::Instance()->DelInherit(pRole->GetID(), roArg.roleid());
		roRes.set_errorcode(ERR_GUILD_INHERIT_MAP_WRONG);
		return;
	}

	if (!CGuildInherit::Instance()->IsInherit(pRole->GetID(), roArg.roleid()))
	{
		CGuildInherit::Instance()->DelInherit(pRole->GetID(), roArg.roleid());
		roRes.set_errorcode(ERR_GUILD_INHERIT_CAN_NOT);
		return;
	}

	UINT32 errorCode = CGuildInherit::Instance()->CanInherit(pRole->GetID(), roArg.roleid()); 
	if (errorCode != ERR_SUCCESS)
	{
		CGuildInherit::Instance()->DelInherit(pRole->GetID(), roArg.roleid());
		roRes.set_errorcode((KKSG::ErrorCode)errorCode);
		return;
	}

	RpcM2G_tryAddGuildInherit* pMsg = RpcM2G_tryAddGuildInherit::CreateRpc();
	pMsg->m_oArg.set_roleone(pRole->GetID());
	pMsg->m_oArg.set_roletwo(roArg.roleid());
	pMsg->m_oArg.set_rpcid(DelayRpc());
	pMsg->m_oArg.set_isdo(true);
	pRole->SendMsgToGS(*pMsg);

	CGuildInherit::Instance()->DelInherit(pRole->GetID(), roArg.roleid());
	return ;
}

void RpcC2M_AceptGuildInherit::OnDelayReplyRpc(const AceptGuildInheritArg &roArg, AceptGuildInheritRes &roRes, const CUserData &roUserData)
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

	CGuildInherit::Instance()->AcceptInherit(pRole->GetID(), roArg.roleid());
	roRes.set_errorcode(ERR_SUCCESS);
}
