#include "pch.h"
#include "ranklist/rpcc2m_clientqueryranklistntf.h"
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
#include "social/socialmgr.h"
#include "ranklist/flowerranklist.h"
#include "skyteam/skyteammgr.h"
#include "ranklist/worldbossguildranklist.h"

// generate by ProtoGen at date: 2016/8/22 14:50:06

RPC_SERVER_IMPLEMETION(RpcC2M_ClientQueryRankListNtf, ClientQueryRankListArg, ClientQueryRankListRes)

struct WorldBossGuildRoleData
{
	UINT64 roleid;
	float damage;

	WorldBossGuildRoleData()
	{
		roleid = 0;
		damage = 0;
	}

	bool operator < (const WorldBossGuildRoleData& data) const
	{
		if (damage > data.damage)
		{
			return true;
		}
		else if (damage < data.damage)
		{
			return false;
		}
		else
		{
			return roleid < data.roleid;
		}
	}
};

static void FillWorldBossGuildRoleRank(CRole* pRole, KKSG::RankList* rankdatas, KKSG::RankData* rankdata)
{
	UINT64 guildid = CGuildMgr::Instance()->GetGuildIdByRoleId(pRole->GetID());
	if (INVALID_GUILDID == guildid)
	{
		return;
	}
	WorldBossGuildRankList* guildranklist = (WorldBossGuildRankList*)CRankListMgr::Instance()->GetRankList(KKSG::WorldBossGuildRank);	
	if (NULL == guildranklist)
	{
		SSWarn<<"role:"<<pRole->GetID()<<" guildid:"<<guildid<<" ranklist NULL";
		return;
	}
	
	const std::unordered_map<UINT64, float>* guildroledamage = guildranklist->GetGuildRoleRank(guildid);
	if (NULL == guildroledamage)
	{
		SSWarn<<"role:"<<pRole->GetID()<<" guildid:"<<guildid<<" damage NULL";
		return;
	}
	std::set<WorldBossGuildRoleData> datas;
	for (auto i = guildroledamage->begin(); i != guildroledamage->end(); ++i)
	{
		WorldBossGuildRoleData data;
		data.roleid = i->first;
		data.damage = i->second;
		datas.insert(data);
	}
	UINT32 rank = 0;
	for (auto i = datas.begin(); i != datas.end(); ++i)
	{
		if (rank <= 5)
		{
			KKSG::RankData* data = rankdatas->add_rankdata();
			data->set_roleid(i->roleid);
			data->set_damage((float)i->damage);
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(i->roleid);
			if (NULL != summary)
			{
				data->set_rolename(summary->GetName());
			}
		}
		// self
		if (i->roleid == pRole->GetID())
		{
			rankdata->set_roleid(i->roleid);
			rankdata->set_damage((float)i->damage);
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(i->roleid);
			if (NULL != summary)
			{
				rankdata->set_rolename(summary->GetName());
			}
			rankdata->set_rank(rank);
		}
		++rank;
	}
}

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
	roRes.set_timestamp(ranklist->GetTimeStamp());
	roRes.set_rankallcount(ranklist->GetNeedRankCount());

	///> 所有排行信息
	KKSG::RankList* rankdatas = roRes.mutable_ranklist();
	ranklist->RankListToClient(rankdatas, ranklist->GetClientQueryCount(), roArg.profession());

	///> 自己的排行信息
	KKSG::RankData* rankdata = roRes.mutable_rolerankdata();
	rankdata->set_rank(INVALID_RANK);

	UINT32 rank = INVALID_RANK;
	UINT64 uid = pRole->GetID();
	if (KKSG::GuildBossRank == ranktype || KKSG::WorldBossGuildRank == ranktype)
	{
		Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());	
		uid = NULL != guild ? guild->GetID() : 0;
	}
	// 联赛排行榜
	else if (KKSG::LeagueTeamRank == ranktype || KKSG::CrossLeagueRank == ranktype)
	{
		uid = LeagueTeamMgr::Instance()->GetLeagueTeamIdByRoleId(uid);
	}
	else if (KKSG::SkyCraftRank == ranktype)
	{
		uid = SkyTeamMgr::Instance()->GetSkyTeamIdByRoleId(uid);
	}

	if (0 != uid)
	{
		rank = ranklist->GetRank(uid);
	}
	ranklist->RankInfoToClient(rankdata, rank);
	rankdata->set_rank((INVALID_RANK == rank) ? INVALID_RANK : (rank - 1));
	
	// 没有排名也需要信息
	ranklist->NewRankInfoToClient(rankdata, uid);

	if (KKSG::MilitaryRank == ranktype && INVALID_RANK == rank)
	{
		KKSG::MilitaryRankData* data = rankdata->mutable_military_info();
		data->set_military_rank(0);
		data->set_military_exploit(0);
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

void RpcC2M_ClientQueryRankListNtf::OnCall(const ClientQueryRankListArg &roArg, ClientQueryRankListRes &roRes)
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
	// world boss guild role rank
	if (KKSG::WorldBossGuildRoleRank == ranktype)
	{
		KKSG::RankList* rankdatas = roRes.mutable_ranklist();
		KKSG::RankData* rankdata = roRes.mutable_rolerankdata();

		FillWorldBossGuildRoleRank(pRole, rankdatas, rankdata);

		roRes.set_errorcode(KKSG::ERR_SUCCESS);
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
	if (ranklist == NULL)
	{
		SSWarn<<"ranklist is NULL, rank type:"<<type<<END;
		return;
	}
	FillRankList(pRole, ranktype, roArg.sendpunishdata(), ranklist, roArg, roRes);
	roRes.set_errorcode(ERR_SUCCESS);

	return;
}

void RpcC2M_ClientQueryRankListNtf::OnDelayReplyRpc(const ClientQueryRankListArg &roArg, ClientQueryRankListRes &roRes, const CUserData &roUserData)
{
}
