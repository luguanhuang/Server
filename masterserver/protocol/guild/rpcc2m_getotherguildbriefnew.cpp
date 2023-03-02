#include "pch.h"
#include "guild/rpcc2m_getotherguildbriefnew.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"

// generate by ProtoGen at date: 2016/9/22 11:43:01

RPC_SERVER_IMPLEMETION(RpcC2M_GetOtherGuildBriefNew, GetOtherGuildBriefArg, GetOtherGuildBriefRes)

void RpcC2M_GetOtherGuildBriefNew::OnCall(const GetOtherGuildBriefArg &roArg, GetOtherGuildBriefRes &roRes)
{
	UINT64 roleid = roArg.roleid();
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roleid);
	if (pGuild == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_GUILD_NOT_IN_GUILD);
		return;
	}	

	roRes.set_guildname(pGuild->GetName());
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
	roRes.set_announcement(pGuild->GetAnnoucement());
	roRes.set_guildlevel(pGuild->GetLevel());
	roRes.set_icon(pGuild->GetIcon());
	roRes.set_allcount(pGuild->GetCapacity());
	roRes.set_currentcount(pGuild->GetMemberCount());
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcC2M_GetOtherGuildBriefNew::OnDelayReplyRpc(const GetOtherGuildBriefArg &roArg, GetOtherGuildBriefRes &roRes, const CUserData &roUserData)
{
}
