#include "pch.h"
#include "guild/rpcc2m_getguildcheckinrecordsnew.h"

#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildcheckin.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include <time.h>

// generate by ProtoGen at date: 2016/9/16 20:35:10

RPC_SERVER_IMPLEMETION(RpcC2M_GetGuildCheckinRecordsNew, GetGuildCheckinRecordsArg, GetGuildCheckinRecordsRes)

void RpcC2M_GetGuildCheckinRecordsNew::OnCall(const GetGuildCheckinRecordsArg &roArg, GetGuildCheckinRecordsRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (guild == NULL)
	{
		return;
	}

	CGuildCheckin* checkin = guild->Get<CGuildCheckin>();
	if (checkin == NULL)
	{
		return ;
	}

	const std::vector<GuildCheckinRecord>& records = checkin->GetRecords();
	UINT32 nowtime = (UINT32)time(NULL);
	for (auto i = records.begin(); i != records.end(); ++i)
	{
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(i->roleid);
		if (NULL != summary)
		{
			roRes.add_roleid(i->roleid);
			roRes.add_name(summary->GetName());
			roRes.add_type(i->type);
			roRes.add_timestamp((nowtime > i->timestamp) ? (nowtime - i->timestamp) : 0);
		}
	}
}

void RpcC2M_GetGuildCheckinRecordsNew::OnDelayReplyRpc(const GetGuildCheckinRecordsArg &roArg, GetGuildCheckinRecordsRes &roRes, const CUserData &roUserData)
{
}
