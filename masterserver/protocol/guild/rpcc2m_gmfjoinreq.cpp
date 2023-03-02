#include "pch.h"
#include "guild/rpcc2m_gmfjoinreq.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "gmf/guildmatchfightmgr.h"

// generate by ProtoGen at date: 2016/10/10 19:14:35

RPC_SERVER_IMPLEMETION(RpcC2M_gmfjoinreq, gmfjoinarg, gmfjoinres)

void RpcC2M_gmfjoinreq::OnCall(const gmfjoinarg &roArg, gmfjoinres &roRes)
{
	CRole *pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	int ret = GuildMatchFightMgr::Instance()->JoinRequest(pRole);
	roRes.set_errorcode((ErrorCode)ret);
}

void RpcC2M_gmfjoinreq::OnDelayReplyRpc(const gmfjoinarg &roArg, gmfjoinres &roRes, const CUserData &roUserData)
{
}
