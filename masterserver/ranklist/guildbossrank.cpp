#include "pch.h"
#include "guildbossrank.h"
#include "guild/guildmgr.h"
#include "pb/project.pb.h"
#include "guild/guild.h"
#include "account/punishmgr.h"

GuildBossRankInfo::GuildBossRankInfo(UINT64 guildid)
:CRankInfo(guildid)
{
	Reset();
}

void GuildBossRankInfo::Reset()
{
	m_damage = 0;
	m_time = (UINT32)(-1);
	m_guildname = "";
}

void GuildBossRankInfo::RankInfoToKKSG(KKSG::RankData* pRankData) const
{
	pRankData->set_roleid(GetUniqueId());
	pRankData->set_time((UINT32)m_time);
	pRankData->set_damage(m_damage);
	pRankData->set_guildname(m_guildname);
	pRankData->set_guildbossname(m_bossName);
	pRankData->set_guildbossdpsmax(m_dpsMaxrole);
	pRankData->set_guildbossindex(m_bossIndex);
}

void GuildBossRankInfo::RankInfoToDBKKSG(KKSG::RankData* pRankData) const
{
	pRankData->set_roleid(GetUniqueId());
	pRankData->set_time((UINT32)m_time);
	pRankData->set_damage(m_damage);
	pRankData->set_guildname(m_guildname);
	pRankData->set_guildbossname(m_bossName);
	pRankData->set_guildbossdpsmax(m_dpsMaxrole);
	pRankData->set_guildbossindex(m_bossIndex);
}

void GuildBossRankInfo::DBKKSGToRankInfo(const KKSG::RankData* pRankData)
{
	SetUniqueId(pRankData->roleid());
	m_time = pRankData->time();
	m_damage = pRankData->damage();
	m_guildname = pRankData->guildname();
	m_bossName = pRankData->guildbossname();
	m_dpsMaxrole = pRankData->guildbossdpsmax();
	m_bossIndex = pRankData->guildbossindex();
}

//////////////////////////////////////////////////////////////////////////
void GuildBossRankDb::PackDBData(KKSG::RankList* ranklist)
{
	GuildBossRankList* pBossRankList = static_cast<GuildBossRankList*>(m_ranklist);
	std::vector<GuildBossRankInfo*> damageinfo;
	pBossRankList->GetRankInfo(damageinfo);
	
	for(auto i = damageinfo.begin(); i != damageinfo.end(); i++)
	{
		CRankInfo* rankinfo = *i;
		if(NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}
}

void GuildBossRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	GuildBossRankList* ranklist = static_cast<GuildBossRankList*>(m_ranklist);

	GuildBossRankInfo rankinfo(rankdata.roleid());
	rankinfo.DBKKSGToRankInfo(&rankdata);

	ranklist->Update(rankinfo.GetUniqueId(), rankinfo.m_bossIndex, rankinfo.m_time, rankinfo.m_damage, rankinfo.m_dpsMaxrole, rankinfo.m_bossName);
}

//////////////////////////////////////////////////////////////////////////
GuildBossRankList::GuildBossRankList(int type)
:CRankList(type)
{
	RegisterDb(new GuildBossRankDb);
}

GuildBossRankList::~GuildBossRankList()
{

}

void GuildBossRankList::Clear()
{
	std::vector<GuildBossRankInfo*> rankinfos;
	m_rankTree.GetAllKey(rankinfos);
	for(auto i = rankinfos.begin(); i != rankinfos.end(); i++)
	{
		delete *i;
	}

	m_rankTree.Clear();
}

UINT32 GuildBossRankList::GetNeedRankCount()
{
	return GUILDBOSSRANKCOUNT;
}

UINT32 GuildBossRankList::GetRank(UINT64 guildid)
{
	return m_rankTree.GetRankByID(guildid);
}

UINT32 GuildBossRankList::GetSize()
{
	return m_rankTree.GetSize();
}

const GuildBossRankInfo* GuildBossRankList::GetInfoByRank(UINT32 rank)
{
	GuildBossRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByRank(rank, rankinfo);
	if(ret != INVALID_RANK)
	{
		return rankinfo;
	}
	else
	{
		return NULL;
	}
}

void GuildBossRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	std::vector<GuildBossRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);

	for(UINT32 i = 0; i < vec.size(); i++)
	{
		KKSG::RankData* pRankData = rankdatas->add_rankdata();
		vec[i]->RankInfoToKKSG(pRankData);
	}
}

void GuildBossRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	const GuildBossRankInfo* rankinfo = GetInfoByRank(rank);
	if(rankinfo != NULL)
	{
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void GuildBossRankList::Update(UINT64 guildid, UINT32 bossIndex, UINT32 time, double damage, std::string dpsRole, std::string bossName)
{
	if(CPunishMgr::Instance()->IsPunish(guildid, KKSG::PUNISH_USER_GUILDBOSS_RANK))
	{
		return;
	}
	GuildBossRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(guildid, rankinfo);
	if(ret == INVALID_RANK)
	{
		Guild* guild = CGuildMgr::Instance()->GetGuild(guildid);
		GuildBossRankInfo* rankinfo = new GuildBossRankInfo(guildid);
		rankinfo->m_time = time;
		rankinfo->m_damage = damage;
		rankinfo->m_guildname = (NULL == guild) ? "" : guild->GetName();
		rankinfo->m_bossIndex = bossIndex; 
		rankinfo->m_dpsMaxrole = dpsRole;
		rankinfo->m_bossName = bossName;
		m_rankTree.Insert(rankinfo, guildid);
	}
	else
	{
		m_rankTree.Erase(guildid);
		rankinfo->m_time = time;
		rankinfo->m_damage = damage;
		rankinfo->m_bossIndex = bossIndex;
		rankinfo->m_dpsMaxrole = dpsRole;
		rankinfo->m_bossName = bossName;
		m_rankTree.Insert(rankinfo, guildid);
	}
}

void GuildBossRankList::InsertKey(GuildBossRankInfo* rankinfo)
{
	if(NULL == rankinfo)
	{
		LogWarn("Insert a NULL pointer in GuildBossRankList");
		return;
	}
	UINT64 guildid = rankinfo->GetUniqueId();
	m_rankTree.Insert(rankinfo, guildid);
}

void GuildBossRankList::GetRankInfo(std::vector<GuildBossRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}

void GuildBossRankList::SaveToDb()
{
	m_db->SendToDB();
	SetTimeStamp();
}

void GuildBossRankList::DeleteGuild(UINT64 guildid)
{
	GuildBossRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(guildid, rankinfo);
	if(ret != INVALID_RANK)
	{
		m_rankTree.Erase(guildid);
		delete rankinfo;
	}
}
