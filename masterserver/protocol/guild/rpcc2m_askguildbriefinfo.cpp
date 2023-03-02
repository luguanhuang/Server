#include "pch.h"
#include "guild/rpcc2m_askguildbriefinfo.h"

#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildactivity.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"

// generate by ProtoGen at date: 2016/8/26 15:44:15

RPC_SERVER_IMPLEMETION(RpcC2M_AskGuildBriefInfo, GuildBriefArg, GuildBriefRes)

void RpcC2M_AskGuildBriefInfo::OnCall(const GuildBriefArg &roArg, GuildBriefRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn << "role is NULL and session is :" << m_sessionID <<END;
		return;
	}

	Guild* pGuild = NULL;
	if (roArg.has_guildid())
	{
		pGuild = CGuildMgr::Instance()->GetGuild(roArg.guildid());
	}
	
	if (pGuild == NULL)
	{
		pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID()); 
	}

	if (pGuild == NULL)
	{
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}

	roRes.set_name(pGuild->GetName());
	CRoleSummary* pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pGuild->GetLeaderID());
	if (pRoleSum != NULL)
	{
		roRes.set_leadername(pRoleSum->GetName());
	}
	else
	{
		roRes.set_leadername(pGuild->GetLeaderName());
	}
	//roRes.set_leadername(pGuild->GetLeaderName());
	roRes.set_leaderid(pGuild->GetLeaderID());
	roRes.set_annoucement(pGuild->GetAnnoucement());
	roRes.set_level(pGuild->GetLevel());
	roRes.set_membercount(pGuild->GetMemberCount());
	roRes.set_capacity(pGuild->GetCapacity());
	roRes.set_icon(pGuild->GetIcon());
	roRes.set_recuritppt(pGuild->GetRecuritPPT());
	roRes.set_needapproval(pGuild->GetNeedApproval());
	roRes.set_exp(pGuild->GetExp());
	roRes.set_rank(pGuild->GetRank());
	roRes.set_prestige(pGuild->GetPrestige());

	/*
	CRoleSummary* pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pGuild->GetLeaderID());
	if (pRoleSum != NULL)
	{
		roRes.set_leadername(pRoleSum->GetName());
	}
	*/

	CGuildActivity* pActivity = pGuild->Get<CGuildActivity>();
	if (pActivity != NULL)
	{
		UINT32 activity = 0;
		UINT32 activityOne = 0;
		UINT32 activityTwo = 0;
		UINT32 activityThree = 0;
		pActivity->GetStand(activity, activityOne, activityTwo, activityThree);
		roRes.set_activity(activity);
		roRes.set_activityone(activityOne);
		roRes.set_activitytwo(activityTwo);
		roRes.set_activitythree(activityThree);
	}

	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2M_AskGuildBriefInfo::OnDelayReplyRpc(const GuildBriefArg &roArg, GuildBriefRes &roRes, const CUserData &roUserData)
{
}
