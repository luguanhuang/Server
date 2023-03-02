#include "pch.h"
#include "guild/rpcc2m_fetchguildhistorynew.h"
#include "guild/guildhistory.h"
#include "guild/guildhistorydef.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"

// generate by ProtoGen at date: 2016/9/20 14:22:23

RPC_SERVER_IMPLEMETION(RpcC2M_FetchGuildHistoryNew, GuildHistoryArg, GuildHistoryRes)


	
static void AppedGuildHistory(GuildLogBase *pLog, void *arg)
{
	GuildHistoryRes &roRes = *(GuildHistoryRes *)arg;
	KKSG::GHisRecord *pRecord = roRes.add_records();
	pRecord->set_roleid(pLog->roleid);
	pRecord->set_rolename(pLog->rolename);
	pRecord->set_position(((GuildMemberLog *)pLog)->position);
	pRecord->set_type(pLog->type);
	pRecord->set_time((UINT32)(time(NULL)-pLog->time));
}


void RpcC2M_FetchGuildHistoryNew::OnCall(const GuildHistoryArg &roArg, GuildHistoryRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	UINT64 gid = CGuildMgr::Instance()->GetRoleGuild(pRole->GetID());
	if (gid == INVALID_GUILDID)
	{
		return;
	}

	Guild *pGuild = CGuildMgr::Instance()->GetGuild(gid);
	if (pGuild == NULL)
	{
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}

	roRes.set_result(KKSG::ERR_SUCCESS);
	pGuild->Get<GuildHistory>()->Foreach(GLog_Member, AppedGuildHistory, &roRes);
}

void RpcC2M_FetchGuildHistoryNew::OnDelayReplyRpc(const GuildHistoryArg &roArg, GuildHistoryRes &roRes, const CUserData &roUserData)
{
}
