#include "pch.h"
#include "guild/rpcc2m_askguildwageinfo.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildwage.h"
#include "guild/guildactivity.h"

// generate by ProtoGen at date: 2016/10/26 16:47:51

RPC_SERVER_IMPLEMETION(RpcC2M_AskGuildWageInfo, AskGuildWageInfoArg, AskGuildWageInfoRes)

void RpcC2M_AskGuildWageInfo::OnCall(const AskGuildWageInfoArg &roArg, AskGuildWageInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		return ;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild == NULL)
	{
		return ;
	}

	CGuildWage* pGuildWage = pGuild->Get<CGuildWage>();
	if (pGuildWage == NULL)
	{
		return ;
	}

	BaseData& base = pGuildWage->GetBaseData();
	roRes.set_activity(pGuild->Get<CGuildActivity>()->GetActivity());
	roRes.set_exp((UINT32)pGuild->GetAllExp() <= base.m_exp ? 0 : (UINT32)pGuild->GetAllExp() - base.m_exp);
	roRes.set_prestige(pGuild->GetPrestige() <= base.m_prestige ? 0 : pGuild->GetPrestige() - base.m_prestige);
	roRes.set_rolenum(pGuild->GetMemberCount());
	roRes.set_lastscore(pGuildWage->GetLastScore());
	roRes.set_wagelvl(pGuildWage->GetWageLvl());
	roRes.set_guildlvl(pGuildWage->GetGuildLvl());
	roRes.set_rewardstate((KKSG::WageRewardState)pGuildWage->IsReward(pRole->GetID()));
	roRes.set_lastposition(pGuildWage->GetPosition(pRole->GetID()));

	CGuildActivity* pGuildAct = pGuild->Get<CGuildActivity>();
	pGuildAct->FillActivityRank(roRes);




}

void RpcC2M_AskGuildWageInfo::OnDelayReplyRpc(const AskGuildWageInfoArg &roArg, AskGuildWageInfoRes &roRes, const CUserData &roUserData)
{

}
