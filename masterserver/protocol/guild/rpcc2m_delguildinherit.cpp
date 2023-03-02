#include "pch.h"
#include "guild/rpcc2m_delguildinherit.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guildinherit.h"

// generate by ProtoGen at date: 2016/11/18 16:31:32

RPC_SERVER_IMPLEMETION(RpcC2M_DelGuildInherit, DelGuildInheritArg, DelGuildInheritRes)

void RpcC2M_DelGuildInherit::OnCall(const DelGuildInheritArg &roArg, DelGuildInheritRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}

	CGuildInherit::Instance()->DelAll(pRole->GetID());
	roRes.set_errorcode(ERR_SUCCESS);
}

void RpcC2M_DelGuildInherit::OnDelayReplyRpc(const DelGuildInheritArg &roArg, DelGuildInheritRes &roRes, const CUserData &roUserData)
{
}
