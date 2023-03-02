#include "pch.h"
#include "guild/rpcc2m_fetchguildapp.h"
#include "guild/guildapps.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "table/guildConfig.h"
#include <time.h>


// generate by ProtoGen at date: 2016/9/15 21:01:52

RPC_SERVER_IMPLEMETION(RpcC2M_FetchGuildApp, FetchGAPPArg, FetchGAPPRes)

static void AppendApplication(GuildApplicationInfo *pInfo, void *arg)
{
	FetchGAPPRes &roRes = *(FetchGAPPRes *)arg;

	if (CGuildMgr::Instance()->GetRoleGuild(pInfo->qwRoleID) != INVALID_GUILDID)
	{
		pInfo->deleted = true;
		return;
	}

	CRoleSummary *pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pInfo->qwRoleID);
	if (pRoleSum != NULL)
	{
		roRes.add_roleid(pInfo->qwRoleID);
		roRes.add_time((UINT32)(time(NULL) - pInfo->appTime));
		roRes.add_rolename(pRoleSum->GetName());
		roRes.add_level(pRoleSum->GetLevel());
		roRes.add_ppt(pRoleSum->GetPowerPoint());
		roRes.add_profession((KKSG::RoleType)pRoleSum->GetProfession());
	}
	else
	{
		pInfo->deleted = true;
	}
}

void RpcC2M_FetchGuildApp::OnCall(const FetchGAPPArg &roArg, FetchGAPPRes &roRes)
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

	GuildMember *pMember = pGuild->GetMember(pRole->GetID());
	if (!CGuildConfig::Instance()->HasPermission(pMember->position, GPEM_APPROVAL))
	{
		roRes.set_result(KKSG::ERR_GUILD_NO_PERMISSION);
		return;
	}

	pGuild->Get<GuildApplication>()->Foreach(AppendApplication, &roRes);
	pGuild->Get<GuildApplication>()->ClearDeleted();
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2M_FetchGuildApp::OnDelayReplyRpc(const FetchGAPPArg &roArg, FetchGAPPRes &roRes, const CUserData &roUserData)
{
}
