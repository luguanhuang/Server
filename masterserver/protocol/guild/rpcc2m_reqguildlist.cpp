#include "pch.h"
#include "guild/rpcc2m_reqguildlist.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
//#include "guild/guildapps.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "account/punishmgr.h"

// generate by ProtoGen at date: 2016/9/1 11:04:39

RPC_SERVER_IMPLEMETION(RpcC2M_ReqGuildList, FetchGuildListArg, FetchGuildListRes)

static void SetGuildInfo(Guild *pGuild, KKSG::GuildInfo *pInfo, UINT64 qwRoleID)
{
	UINT32 prestige = 0;
	KKSG::PunishData punishData;
	if (CPunishMgr::Instance()->GetPunishInfo(pGuild->GetID(), PUNISH_USER_GUILD_RANK, punishData))
	{
		KKSG::IdipGuildInfo rankdata;
		if (punishData.has_rankdata() && rankdata.ParseFromString(punishData.rankdata()))
		{
			prestige = rankdata.prestige();
		}
	}

	pInfo->set_id(pGuild->GetID());
	pInfo->set_name(pGuild->GetName());
	pInfo->set_leaderid(pGuild->GetLeaderID());
	CRoleSummary* pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pGuild->GetLeaderID());
	if (pRoleSum != NULL)
	{
		pInfo->set_leadername(pRoleSum->GetName());
	}
	else
	{
		pInfo->set_leadername(pGuild->GetLeaderName());
	}
	//pInfo->set_leadername(pGuild->GetLeaderName());
	pInfo->set_level(pGuild->GetLevel());
	pInfo->set_ppt(pGuild->GetRecuritPPT());
	pInfo->set_needapproval(pGuild->GetNeedApproval());
	pInfo->set_membercount(pGuild->GetMemberCount());
	pInfo->set_issendapplication(false);
	//pInfo->set_issendapplication(pGuild->Get<GuildApplication>()->IsContain(qwRoleID));
	pInfo->set_icon(pGuild->GetIcon());
	pInfo->set_capacity(pGuild->GetCapacity());
	pInfo->set_annoucement(pGuild->GetAnnoucement());
	if (prestige > 0)
	{
		pInfo->set_prestige(prestige);
	}
	else
	{
		pInfo->set_prestige(pGuild->GetPrestige());
	}
	pInfo->set_guildexp(pGuild->GetExp());
	//CRoleSummary* role = CRoleSummaryMgr::Instance()->GetRoleSummary(pGuild->GetLeaderID());
	if(pRoleSum != NULL)
	{
		pInfo->set_titleid(pRoleSum->GetTitleID());
	}
}

void RpcC2M_ReqGuildList::OnCall(const FetchGuildListArg &roArg, FetchGuildListRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	GuildSorter &sorter = CGuildMgr::Instance()->GetSorter();

	if (!sorter.IsSorttypeSupport(roArg.sorttype()))
	{
		LogWarn("GuildList sort type %d not support!", roArg.sorttype());
		return;
	}

	UINT64 qwRoleID = pRole->GetID();

	std::vector<Guild *> &GuildSet = sorter.GetSortResult(roArg.sorttype());

	int count = roArg.count();

	if (roArg.reverse())
	{
		int start = (int)GuildSet.size() - roArg.start() - 1;


		for (int i = start; i >= 0 && count > 0; --i)
		{
			Guild *pGuild = GuildSet[i];

			if (roArg.has_name())
			{
				if (!pGuild->ContainStr(roArg.name()))
				{
					continue;
				}
			}

			--count;
			KKSG::GuildInfo *pInfo = roRes.add_guilds();
			SetGuildInfo(pGuild, pInfo, qwRoleID);
		}
	}
	else
	{
		UINT32 start = roArg.start();

		for (UINT32 i = start; i < GuildSet.size() && count > 0; ++i)
		{
			Guild *pGuild = GuildSet[i];

			if (roArg.has_name())
			{
				if (!pGuild->ContainStr(roArg.name()))
				{
					continue;
				}
			}

			--count;

			KKSG::GuildInfo *pInfo = roRes.add_guilds();
			SetGuildInfo(pGuild, pInfo, qwRoleID);
		}
	}
	UINT64 guildId = CGuildMgr::Instance()->GetGuildIdByRoleId(pRole->GetID()); 
	if (guildId != 0)
	{
		if (CPunishMgr::Instance()->CheckPunishAndNotify(pRole, guildId, (INT32)PUNISH_USER_GUILD_RANK))
		{
			return;
		}
	}
}

void RpcC2M_ReqGuildList::OnDelayReplyRpc(const FetchGuildListArg &roArg, FetchGuildListRes &roRes, const CUserData &roUserData)
{
}
