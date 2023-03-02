#include "pch.h"
#include "scene/ptcg2m_synscenerewardg2m.h"
#include "scene/ptcm2g_synscenerewardm2g.h"
#include "network/gslink.h"
#include "commondef.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "reswar/reswarmgr.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/nestweekranklist.h"
#include "notice/notice.h"
#include "role/rolesummarymgr.h"
#include "nestweek/nestweekmgr.h"
#include "ranklist/biohelllrankList.h"
#include "ranklist/competedragonranklist.h"

// generate by ProtoGen at date: 2016/8/5 19:31:09

void PtcG2M_SynSceneRewardG2M::Process(UINT32 dwConnID)
{
	/*PtcM2G_SynSceneRewardM2G msg;
	msg.m_Data.CopyFrom(m_Data);
	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, msg);*/

	///////
	std::map<UINT64, UINT32> guildExp;
	for (UINT32 i = 0; i < m_Data.guildexps_size(); ++i)
	{
		const KKSG::GuildExpData& data = m_Data.guildexps(i);
		Guild* pGuild = CGuildMgr::Instance()->GetGuild(data.guildid());
		if (pGuild != NULL)
		{
			pGuild->AddExp(data.guildexp());
			pGuild->AddPrestige(data.guildprestige());
			guildExp[data.guildid()] += data.guildexp(); 
		}
	}

	std::set<UINT64> roleSet;
	for (UINT32 j = 0; j < m_Data.guildcons_size(); ++j)
	{
		const KKSG::GuildConData& data = m_Data.guildcons(j);
		Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(data.roleid());
		if (pGuild != NULL)
		{
			//pGuild->AddRoleContribute(data.roleid(), data.guildcon());
			roleSet.insert(data.roleid());
		}
	}

	if (m_Data.has_reswar())
	{
		KKSG::ResWarBase* pInfo = m_Data.mutable_reswar();
		ResWarMgr::Instance()->SetGuildResData(*pInfo);
	}

	CGuildMgr::Instance()->DoGuildDareReward(roleSet, m_Data.scenename(), guildExp);

	if(m_Data.has_weeknestinfo())
	{
		const KKSG::WeekNestSceneInfo& info = m_Data.weeknestinfo();
		if(info.iswin())
		{
			int curexp = NestWeekMgr::Instance()->GetCurExp();
			if(info.expid() == curexp)
			{
				NestWeekRankList* pRankList = (NestWeekRankList*)CRankListMgr::Instance()->GetRankList(KKSG::NestWeekRank);
				if(pRankList)
				{
					UINT32 now = UINT32(time(NULL));
					NWRKey roles;
					for(int i = 0; i < info.roles_size(); ++i)
					{
						roles.push_back(info.roles(i));
					}
					pRankList->OnChange(roles, info.usetime(), now);
				}
				//公告
				for(int i = 0; i < info.roles_size(); ++i)
				{
					UINT64 roleID = info.roles(i);
					UINT64 guildID = CGuildMgr::Instance()->GetGuildIdByRoleId(roleID);
					if(INVALID_GUILDID != guildID)
					{
						Notice notice(NoticeWeekNestWin);
						std::string name = CRoleSummaryMgr::Instance()->GetRoleName(roleID);
						notice.Replace("$R", name);
						notice.SendGuild(guildID);
					}
				}
			}
			else
			{
				SSInfo << " exp changed epxid = " << info.expid() << " curexp = " << curexp << END;	
			}
		}
	}

	if (m_Data.has_biohellinfo())
	{
		const KKSG::BioHellSceneInfo& info = m_Data.biohellinfo();
		BioHelllRankList* pRankList = (BioHelllRankList*)CRankListMgr::Instance()->GetRankList(KKSG::BioHelllRank);
		if(pRankList)
		{
			UINT32 now = UINT32(time(NULL));
			NWRKey roles;
			for(int i = 0; i < info.roles_size(); ++i)
			{
				roles.push_back(info.roles(i));
			}
			pRankList->OnChange(roles, info.usetime(), now);
		}
	}

	if (m_Data.has_competedragoninfo())
	{
		const KKSG::CompeteDragonSceneInfo& info = m_Data.competedragoninfo();
		CompeteDragonRankList* pRankList = (CompeteDragonRankList*)CRankListMgr::Instance()->GetRankList(KKSG::CompeteDragonRank);
		if (pRankList)
		{
			CDRKey roles;
			for(int i = 0; i < info.roles_size(); ++i)
			{
				roles.push_back(info.roles(i));
			}
			pRankList->OnChange(roles, info.usetime(), info.passtime());
		}
	}
}
