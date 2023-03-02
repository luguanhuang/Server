#include "pch.h"
#include "worldbossguildranklist.h"
#include "guild/guildmgr.h"

void BossGuildRankInfo::RankInfoToKKSG(KKSG::RankData* pRankData) const
{
	pRankData->set_roleid(GetUniqueId());
	pRankData->set_damage(m_damage);
	pRankData->set_rolename(CGuildMgr::Instance()->GetName(GetUniqueId()));	
}

void BossGuildRankInfo::RankInfoToDBKKSG(KKSG::RankData* pRankData) const
{
	pRankData->set_roleid(GetUniqueId());
	pRankData->set_damage(m_damage);
}

void BossGuildRankInfo::DBKKSGToRankInfo(const KKSG::RankData* pRankData)
{
	SetUniqueId(pRankData->roleid());
	m_damage = pRankData->damage();
}

BossGuildRankInfo::BossGuildRankInfo(UINT64 qwRoleId) : CRankInfo(qwRoleId)
{
	Reset();
}

void BossGuildRankInfo::Reset()
{
	m_damage = 0;
}

/////////////////////////////////////////////////////////////////////////////////////
void BossGuildRankDb::PackDBData(KKSG::RankList* ranklist)
{
	WorldBossGuildRankList* guildranklist = static_cast<WorldBossGuildRankList*>(m_ranklist);
	std::vector<BossGuildRankInfo*> damageinfo;
	guildranklist->GetRankInfo(damageinfo);

	for (auto i = damageinfo.begin(); i != damageinfo.end(); i++)
	{
		CRankInfo* rankinfo = *i;
		if (NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}
}

void BossGuildRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	WorldBossGuildRankList* ranklist = static_cast<WorldBossGuildRankList*>(m_ranklist);

	BossGuildRankInfo rankinfo(rankdata.roleid());
	rankinfo.DBKKSGToRankInfo(&rankdata);

	ranklist->AddDamage(rankinfo.GetUniqueId(), 0, rankinfo.m_damage);
}

//////////////////////////////////////////////////////////////////////////////////////
WorldBossGuildRankList::WorldBossGuildRankList(int type)
:CRankList(type)
{
	RegisterDb(new BossGuildRankDb);
}

WorldBossGuildRankList::~WorldBossGuildRankList()
{
	Clear();
}

void WorldBossGuildRankList::Clear()
{
	std::vector<BossGuildRankInfo*> damageinfo;
	m_rankTree.GetAllKey(damageinfo);
	for(auto i = damageinfo.begin(); i != damageinfo.end(); i++)
	{
		delete *i;
	}
	
	m_rankTree.Clear();
	m_guild2info.clear();
}

const BossGuildRankInfo* WorldBossGuildRankList::GetInfoByRank(UINT32 rank)
{
	BossGuildRankInfo* rankinfo;
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

void WorldBossGuildRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	std::vector<BossGuildRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);

	UINT32 vecSize = vec.size();
	for (UINT32 i = 0; i < vecSize; i++)
	{
		BossGuildRankInfo* rankinfo = vec[i];
		KKSG::RankData* pRankData = rankdatas->add_rankdata();
		rankinfo->RankInfoToKKSG(pRankData);
	}
}

void WorldBossGuildRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	const BossGuildRankInfo* rankinfo = GetInfoByRank(rank);
	if(rankinfo != NULL)
	{
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

UINT32 WorldBossGuildRankList::GetDamage(UINT64 guildid)
{
	BossGuildRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(guildid, rankinfo);
	if(ret == INVALID_RANK)
	{
		return 0;
	}
	return (UINT32)rankinfo->m_damage;
}

void WorldBossGuildRankList::AddDamage(UINT64 guildid, UINT64 roleid, float damage)
{
	BossGuildRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(guildid, rankinfo);
	if(ret == INVALID_RANK)
	{
		BossGuildRankInfo* rankinfo = new BossGuildRankInfo(guildid);
		rankinfo->m_damage = damage;
		m_rankTree.Insert(rankinfo, guildid);
	}
	else
	{
		m_rankTree.Erase(guildid);
		rankinfo->m_damage += damage;
		m_rankTree.Insert(rankinfo, guildid);
	}
	if (roleid != 0)
	{
		///> 每个工会记录
		m_guild2info[guildid][roleid] += damage;
	}
}

void WorldBossGuildRankList::InsertKey(BossGuildRankInfo* rankinfo)
{
	if(NULL == rankinfo)
	{
		LogWarn("Insert a NULL pointer in BossDamageRanklist");
		return;
	}
	UINT64 roleid = rankinfo->GetUniqueId();
	m_rankTree.Insert(rankinfo, roleid);
}

void WorldBossGuildRankList::SaveToDb()
{
	m_db->SendToDB();
	SetTimeStamp();
}

void WorldBossGuildRankList::RemoveRole(UINT64 guildid, UINT64 roleid)
{
	auto i = m_guild2info.find(guildid);
	if (i != m_guild2info.end())
	{
		auto j = i->second.find(roleid);
		if (j != i->second.end())
		{
			float damage = j->second;
			AddDamage(guildid, roleid, -damage);
		}
	}
}

const std::unordered_map<UINT64, float>* WorldBossGuildRankList::GetGuildRoleRank(UINT64 guildid)
{
	auto i = m_guild2info.find(guildid);
	if (i == m_guild2info.end())
	{
		return NULL;
	}
	else
	{
		return &i->second;
	}
}
