#include "pch.h"
#include "mail/mailmgr.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/briefranklist.h"
#include "mail/mailconfig.h"
#include "role/rolesummarymgr.h"
#include "team/rpcg2m_tryteamcostreward.h"
#include "global/GlobalConfigDBBuild.h"
#include "util/gametime.h"
#include "table/globalconfig.h"
#include "levelseal/levelsealMgr.h"

// generate by ProtoGen at date: 2016/11/27 18:37:53

RPC_SERVER_IMPLEMETION(RpcG2M_TryTeamCostReward, TryTeamCostReward, TryTeamCostRewardRes)

void RpcG2M_TryTeamCostReward::OnCall(const TryTeamCostReward &roArg, TryTeamCostRewardRes &roRes)
{
	///> 世界等级
	UINT32 currentworldlevel = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eWorldLevel));
	if (currentworldlevel < (UINT32)(GetGlobalConfig().TeamCostWorldLevel))
	{
		roRes.set_ishit(false);
		SSInfo<<"world level limit, current level:"<<currentworldlevel<<" limit:"<<GetGlobalConfig().TeamCostWorldLevel<<END;
		return;
	}

	///> 每日邮件奖励限制
	std::string strlimit = CGlobalConfigDBBuild::Instance()->GetConfigValue(eTeamCostMailLimit);
	UINT64 limit = convert<UINT64>(strlimit.c_str());
	UINT32 day = (UINT32)(limit >> 32);
	UINT32 todayunique = GameTime::GetTodayUnique();
	if (day == todayunique)
	{
		UINT32 temp = (UINT32)(limit & 0x00000000ffffffff);
		if (temp >= GetGlobalConfig().TeamCostMailLimit)
		{
			roRes.set_ishit(false);
			SSInfo<<"team cost reward limit:"<<temp<<END;
			return;
		}
	}
	else
	{
		limit = ((UINT64)todayunique << 32);
		CGlobalConfigDBBuild::Instance()->SetConfigValue(eTeamCostMailLimit, ToString(limit));
		CGlobalConfigDBBuild::Instance()->UpdateToDb(eTeamCostMailLimit);

		SSInfo<<"team cost reward reset:"<<ToString(limit)<<END;
	}

	UINT64 roleid = roArg.roleid();
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
	if (NULL == summary)
	{
		roRes.set_ishit(false);
		SSWarn<<"invalid roleid:"<<roleid<<END;
		return;
	}

	UINT32 selflevel = summary->GetLevel();
	UINT32 levelseal = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
	if (!CLevelSealMgr::Instance()->CanGetTeamCost(levelseal, currentworldlevel, selflevel))
	{
		roRes.set_ishit(false);
	}
	else
	{
		if (roArg.item_size() > 0)
		{
			///> 发邮件
			const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_TeamCostReward);

			std::vector<ItemDesc> items;
			ItemDesc item;
			item.ToThis(roArg.item(0));
			items.push_back(item);

			MailData mail(NULL == conf ? "team cost" : conf->m_title, NULL == conf ? "team cost" : conf->m_content, items, MAIL_EXPIRE_TIME);
			mail.SetReason(ItemFlow_TeamCostAward);
			mail.m_latesend = roArg.delaytime(0) + (UINT32)time(NULL);
			CMailMgr::Instance()->SendMail(roleid, mail);

			SSInfo<<"add mail, role:"<<roArg.roleid()<<" world level:"<<currentworldlevel<<" level:"<<selflevel<<" item:"<<item.itemID<<" count:"<<item.itemCount<<" late:"<<roArg.delaytime(0)<<END;

			UINT32 temp = (UINT32)(limit & 0x00000000ffffffff);
			limit = ((UINT64)todayunique << 32) + (temp + 1);
			CGlobalConfigDBBuild::Instance()->SetConfigValue(eTeamCostMailLimit, ToString(limit));
			CGlobalConfigDBBuild::Instance()->UpdateToDb(eTeamCostMailLimit);

			SSInfo<<"team cost reward give:"<<ToString(limit)<<END;

			roRes.set_ishit(true);
		}
	}
}

void RpcG2M_TryTeamCostReward::OnDelayReplyRpc(const TryTeamCostReward &roArg, TryTeamCostRewardRes &roRes, const CUserData &roUserData)
{
}
