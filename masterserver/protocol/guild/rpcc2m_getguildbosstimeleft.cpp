#include "pch.h"
#include "guild/rpcc2m_getguildbosstimeleft.h"
#include "guild/guildboss.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guildmgr.h"

// generate by ProtoGen at date: 2016/9/15 16:26:29

RPC_SERVER_IMPLEMETION(RpcC2M_getguildbosstimeleft, getguildbosstimeleftArg, getguildbosstimeleftRes)

void RpcC2M_getguildbosstimeleft::OnCall(const getguildbosstimeleftArg &roArg, getguildbosstimeleftRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}

	roRes.set_timeleft(CGuildBossMgr::Instance()->GetEndTimeLeft());
	roRes.set_addattrcount(CGuildBossMgr::Instance()->GetAttrCount(CGuildMgr::Instance()->GetGuildIdByRoleId(role->GetID())));
}

void RpcC2M_getguildbosstimeleft::OnDelayReplyRpc(const getguildbosstimeleftArg &roArg, getguildbosstimeleftRes &roRes, const CUserData &roUserData)
{
}
