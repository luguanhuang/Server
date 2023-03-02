#include "pch.h"
#include "guild/guildboss.h"
#include "guild/rpcm2g_noticeguildbossdoendtogsn.h"
#include "commondef.h"
#include "network/gslink.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "ranklist/ranklistmgr.h"
#include "loghelper/tlogr.h"

// generate by ProtoGen at date: 2016/7/30 21:47:11

RPC_CLIENT_IMPLEMETION(RpcM2G_NoticeGuildBossDoEndToGSN, NoticeGuildBossDoEndArg, NoticeGuildBossDoEndRes)

void RpcM2G_NoticeGuildBossDoEndToGSN::OnReply(const NoticeGuildBossDoEndArg &roArg, const NoticeGuildBossDoEndRes &roRes, const CUserData &roUserData)
{
	/*PtcM2G_GuildBossResultToGS msg;
	for (UINT32 cnt = 0; cnt < roRes.resultdata_size(); ++cnt)
	{
		msg.m_Data.add_resultdatas()->CopyFrom(roRes.resultdata(cnt));
		CGuildBossMgr::Instance()->DelGuild(roRes.resultdata(cnt).guildid());
	}

	if (CGuildBossMgr::Instance()->IsGuildMapZero())
	{
		msg.m_Data.set_isall(true);
	}
	else
	{
		msg.m_Data.set_isall(false);
	}

	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, msg);*/
	//////////////

	for (size_t cnt = 0; cnt < roRes.resultdata_size(); ++cnt)
	{
		KKSG::GuildBossResultData pData = roRes.resultdata(cnt);
		CGuildBossMgr::Instance()->DelGuild(pData.guildid());
		if (pData.damage() <= 0)
		{
			continue;
		}

		//  刷新伤害排行榜
		std::map<UINT64, std::set<GuildBossRoleDamage> >& damageMap = CGuildBossMgr::Instance()->GetGuildDamage();
		std::set<GuildBossRoleDamage>& damageSet = damageMap[pData.guildid()];
		/*damageSet.clear();
		for (UINT32 i = 0; i < pData.unitdamage_size(); ++i)
		{
			GuildBossRoleDamage damageData;
			damageData.roleid = pData.unitdamage(i).roleid();
			damageData.damage = pData.unitdamage(i).damage();
			damageSet.insert(damageData);

			//记录tlog数据;
			std::string szTransTag = TLogMgr::Instance()->GetTagString();
			TGuildBossFlow oLog(szTransTag);
			oLog.m_lRoleID = damageData.roleid;
			oLog.m_lGuildID = pData.guildid();
			oLog.Do();
		}
		*/
		std::string dpsName;
		if (damageSet.size() > 0)
		{
			CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(damageSet.begin()->roleid);
			if (pRoleSummary != NULL)
			{
				dpsName = pRoleSummary->GetName();
			}
		}
		/*if (pData.unitdamage_size() > 0)
		{
			CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(pData.unitdamage(pData.unitdamage_size() - 1).roleid());
			if (pRoleSummary != NULL)
			{
				dpsName = pRoleSummary->GetName();
			}
		}*/

		// 刷新公会排行榜
		CGuildBossMgr::Instance()->GetRankList()->Update(pData.guildid(), pData.bossindex(), pData.endtime(), pData.damage(), dpsName, pData.bossname());	

		//CGuildBossMgr::Instance()->DelGuild(pData.guildid());
		//Guild* pGuild = CGuildMgr::Instance()->GetGuild(pData.guildid());
		//CGuildBossMgr::Instance()->HintNotify(pGuild, true);
	}

	if (CGuildBossMgr::Instance()->IsGuildMapZero())
	{
		CGuildBossMgr::Instance()->GetRankList()->SaveToDb();
		CGuildBossMgr::Instance()->GiveLeaderReward();
		CGuildBossMgr::Instance()->GiveAuctReward();
	}
}

void RpcM2G_NoticeGuildBossDoEndToGSN::OnTimeout(const NoticeGuildBossDoEndArg &roArg, const CUserData &roUserData)
{
}
