#include "pch.h"
#include "guild/rpcc2g_getguildbrief.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
// generate by ProtoGen at date: 2015/10/1 14:11:47

RPC_SERVER_IMPLEMETION(RpcC2G_GetGuildBrief, GuildBriefArg, GuildBriefRes)

void RpcC2G_GetGuildBrief::OnCall(const GuildBriefArg &roArg, GuildBriefRes &roRes)
{

	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	#if GUILD_GS
	UINT64 guildid = roArg.guildid();
	if (0 == guildid)
	{
		guildid = GuildMgr::Instance()->GetRoleGuild(pRole->GetID());
		if (guildid == INVALID_GUILDID)
		{
			return;
		}
	}

	Guild *pGuild = GuildMgr::Instance()->FindGuild(guildid);
	if (pGuild == NULL)
	{
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}

	roRes.set_name(pGuild->GetName());
	roRes.set_leadername(pGuild->GetLeaderName());
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
#endif
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcC2G_GetGuildBrief::OnDelayReplyRpc(const GuildBriefArg &roArg, GuildBriefRes &roRes, const CUserData &roUserData)
{
}
