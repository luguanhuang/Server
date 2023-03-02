#include "pch.h"
#include "ranklist/rpcc2m_queryranklistlua.h"
#include "pb/project.pb.h"
#include "pb/enum.pb.h"
#include "ranklist/ranklist.h"
#include "ranklist/ranklistmgr.h"
#include "role/role.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/rolemanager.h"
#include "scene/scene.h"
#include "guild/guildboss.h"
#include "role/rolesummarymgr.h"
#include "account/punishmgr.h"
#include "common/ptcm2c_idippunishinfomsntf.h"
#include "leagueteam/leagueteammgr.h"
#include "leagueteam/crossleagueteammgr.h"
#include "leaguebattle/leagueseasonmgr.h"
#include "other/serverinfomgr.h"

// generate by ProtoGen at date: 2017/3/17 15:47:43

RPC_SERVER_IMPLEMETION(RpcC2M_QueryRankListLua, ClientQueryRankListArg, ClientQueryRankListRes)

static void FillGuildBossRankList(CRole* pRole, UINT32 sendPunishData, KKSG::RankList* rankdatas, KKSG::RankData* rankdata, UINT64 guildId)
{
	const std::map<UINT64, std::set<GuildBossRoleDamage> >& rankMap = CGuildBossMgr::Instance()->GetGuildDamage();
	auto iter = rankMap.find(guildId);
	if (iter != rankMap.end())
	{
		UINT32 rank = 0;
		for (auto i = iter->second.begin(); i != iter->second.end(); ++i)
		{
			KKSG::RankData* data = rankdatas->add_rankdata();
			data->set_roleid(i->roleid);
			data->set_damage((float)i->damage);
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(i->roleid);
			if (NULL != summary)
			{
				data->set_rolename(summary->GetName());
				data->set_starttype(summary->GetStartUpType());
				data->set_is_vip(summary->IsQQVip());
				data->set_is_svip(summary->IsQQSVip());
			}
			if (i->roleid == pRole->GetID())
			{
				rankdata->CopyFrom(*data);
				rankdata->set_rank(rank);
			}
			++rank;
		}

	}
	//sendPunishData为0代表要发, 1代表不发
	if(sendPunishData == 0 && CPunishMgr::Instance()->CheckPunishAndNotify(pRole, pRole->GetID(), KKSG::PUNISH_USER_ROLE_GUILDBOSS))
	{
		return;
	}
}

static void FillRankList(CRole* pRole, UINT32 ranktype, UINT32 sendPunishData, CRankList* ranklist, const ClientQueryRankListArg& roArg, ClientQueryRankListRes &roRes)
{
	KKSG::RankList* rankdatas = roRes.mutable_ranklist();
	KKSG::RankData* rankdata = roRes.mutable_rolerankdata();
	rankdata->set_rank(INVALID_RANK);

	roRes.set_timestamp(ranklist->GetTimeStamp());
	roRes.set_rankallcount(ranklist->GetNeedRankCount());

	///> 所有排行信息
	ranklist->RankListToClient(rankdatas, ranklist->GetClientQueryCount(), roArg.profession());

	///> 自己的排行信息
	UINT32 rank = INVALID_RANK;
	UINT64 uid = pRole->GetID();
	if (KKSG::GuildBossRank == ranktype || KKSG::WorldBossGuildRank == ranktype)
	{
		Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());	
		uid = NULL != guild ? guild->GetID() : 0;
	}

	// 联赛排行榜
	if (KKSG::LeagueTeamRank == ranktype || KKSG::CrossLeagueRank == ranktype)
	{
		uid = LeagueTeamMgr::Instance()->GetLeagueTeamIdByRoleId(uid);
	}

	if (0 != uid)
	{
		rank = ranklist->GetRank(uid);
	}
	ranklist->RankInfoToClient(rankdata, rank);
	rankdata->set_rank((INVALID_RANK == rank) ? INVALID_RANK : (rank - 1));

	// 没有排名也需要信息
	if (KKSG::LeagueTeamRank == ranktype && INVALID_RANK == rank)
	{
		KKSG::LeagueRankData* ldata = rankdata->mutable_leagueinfo();
		ldata->set_league_teamid(uid);
		LeagueTeam* lt = LeagueTeamMgr::Instance()->GetLeagueTeamById(uid);
		if (lt)
		{
			ldata->set_serverid(MSConfig::Instance()->GetServerID());
			ldata->set_servername(MSConfig::Instance()->GetName());
			ldata->set_teamname(lt->GetName());
			ldata->set_point(lt->GetScore());
			ldata->set_winnum(lt->GetLeagueTeamBattleRecord().GetTotalWinNum());
			ldata->set_winrate(lt->GetLeagueTeamBattleRecord().GetTotalWinRate());
		}
	}
	if (KKSG::CrossLeagueRank == ranktype && INVALID_RANK == rank)
	{
		KKSG::LeagueRankData* ldata = rankdata->mutable_leagueinfo();
		ldata->set_league_teamid(uid);
		CrossLeagueTeam* lt = CrossLeagueTeamMgr::Instance()->GetTeam(uid);
		if (lt)
		{
			ldata->set_serverid(lt->m_serverId);
			ldata->set_servername(ZServerInfoMgr::Instance()->GetServerName(lt->m_serverId));
			ldata->set_teamname(lt->m_name);
			ldata->set_point(lt->m_score);
			ldata->set_winnum(lt->m_winNum);
			ldata->set_winrate(lt->GetWinRate());
		}
	}

	UINT32 punishType = CRankListMgr::Instance()->GetPunishType((KKSG::RankeType)ranktype);
	if((UINT32)(-1) == punishType)
	{
		return;
	}
	//sendPunishData为0代表要发，1代表不发
	if(sendPunishData == 0 && CPunishMgr::Instance()->CheckPunishAndNotify(pRole, uid, (INT32)punishType))
	{
		return;
	}
}

void RpcC2M_QueryRankListLua::OnCall(const ClientQueryRankListArg &roArg, ClientQueryRankListRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	KKSG::RankeType ranktype = (KKSG::RankeType)roArg.ranktype();
	roRes.set_ranktype(ranktype);

	if (KKSG::RankeType_MAX < ranktype)
	{
		SSWarn<<"rank type:"<<ranktype<<END;
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}

	///> guild boss 
	if (KKSG::GuildBossRoleRank == ranktype)
	{
		if (roArg.has_guildid())
		{
			KKSG::RankList* rankdatas = roRes.mutable_ranklist();
			KKSG::RankData* rankdata = roRes.mutable_rolerankdata();
			//rankdata->set_rank(INVALID_RANK);

			FillGuildBossRankList(pRole, roArg.sendpunishdata(), rankdatas, rankdata, roArg.guildid());
			roRes.set_errorcode(ERR_SUCCESS);
		}
		return;
	}

	// 联赛战队排行不同阶段拉取不同的榜(无论客户端传哪个type)
	if (ranktype == KKSG::LeagueTeamRank || ranktype == KKSG::CrossLeagueRank)
	{
		if (LeagueSeasonMgr::Instance()->IsInCrossState())
		{
			ranktype = KKSG::CrossLeagueRank;
		}
		else
		{
			ranktype = KKSG::LeagueTeamRank;
		}
	}

	UINT32 type = ranktype;
	if (type == KKSG::FirstPassRank)
	{
		type = CRankListMgr::Instance()->GetFirstPassRankKey(roArg.firstpassid());
	}

	///> common
	CRankList* ranklist = CRankListMgr::Instance()->GetRankList(type, pRole->GetServerId());
	if (NULL == ranklist)
	{
		SSWarn<<"ranklist is NULL, rank type:"<<ranktype<<END;
		return;
	}
	FillRankList(pRole, ranktype, roArg.sendpunishdata(), ranklist, roArg, roRes);
	roRes.set_errorcode(ERR_SUCCESS);

	return;
}

void RpcC2M_QueryRankListLua::OnDelayReplyRpc(const ClientQueryRankListArg &roArg, ClientQueryRankListRes &roRes, const CUserData &roUserData)
{
}

