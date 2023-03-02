#include "pch.h"
#include "worldbossranklist.h"
#include "role/rolesummarymgr.h"
#include "pb/project.pb.h"
#include "account/punishmgr.h"


void BossDamageRankInfo::RankInfoToKKSG(KKSG::RankData* pRankData) const
{
	pRankData->set_roleid(GetUniqueId());
	pRankData->set_damage(m_damage);
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(GetUniqueId());
	if (summary)
	{
		pRankData->set_rolename(summary->GetName());
		pRankData->set_is_vip(summary->IsQQVip());
		pRankData->set_is_svip(summary->IsQQSVip());
		pRankData->set_starttype(summary->GetStartUpType());
		pRankData->set_profession(summary->GetProfession());
	}
}

void BossDamageRankInfo::RankInfoToDBKKSG(KKSG::RankData* pRankData) const
{
	pRankData->set_roleid(GetUniqueId());
	pRankData->set_damage(m_damage);
}

void BossDamageRankInfo::DBKKSGToRankInfo(const KKSG::RankData* pRankData)
{
	SetUniqueId(pRankData->roleid());
	m_damage = pRankData->damage();
}

BossDamageRankInfo::BossDamageRankInfo(UINT64 qwRoleId) : CRankInfo(qwRoleId)
{
	Reset();
}

void BossDamageRankInfo::Reset()
{
	m_damage = 0;
}
 
/////////////////////////////////////////////////////////////////////////////////////
void BossDamageRankDb::PackDBData(KKSG::RankList* ranklist)
{
	BossDamageRankList* pBossRanklist = static_cast<BossDamageRankList*>(m_ranklist);
	std::vector<BossDamageRankInfo*> damageinfo;
	pBossRanklist->GetRankInfo(damageinfo);

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

void BossDamageRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	BossDamageRankList* ranklist = static_cast<BossDamageRankList*>(m_ranklist);

	BossDamageRankInfo rankinfo(rankdata.roleid());
	rankinfo.DBKKSGToRankInfo(&rankdata);

	ranklist->AddDamage(rankinfo.GetUniqueId(), rankinfo.m_damage);
}

//////////////////////////////////////////////////////////////////////////////////////
BossDamageRankList::BossDamageRankList(int type)
:CRankList(type)
{
	RegisterDb(new BossDamageRankDb);
}

BossDamageRankList::~BossDamageRankList()
{
	Clear();
}

void BossDamageRankList::Clear()
{
	std::vector<BossDamageRankInfo*> damageinfo;
	m_rankTree.GetAllKey(damageinfo);
	for(auto i = damageinfo.begin(); i != damageinfo.end(); i++)
	{
		delete *i;
	}
	
	m_rankTree.Clear();
}

const BossDamageRankInfo* BossDamageRankList::GetInfoByRank(UINT32 rank)
{
	BossDamageRankInfo* rankinfo;
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

void BossDamageRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	std::vector<BossDamageRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);

	UINT32 vecSize = vec.size();
	for (UINT32 i = 0; i < vecSize; i++)
	{
		BossDamageRankInfo* rankinfo = vec[i];
		KKSG::RankData* pRankData = rankdatas->add_rankdata();
		rankinfo->RankInfoToKKSG(pRankData);
	}
}

void BossDamageRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	const BossDamageRankInfo* rankinfo = GetInfoByRank(rank);
	if(rankinfo != NULL)
	{
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

UINT32 BossDamageRankList::GetDamage(UINT64 roleid)
{
	BossDamageRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(roleid, rankinfo);
	if(ret == INVALID_RANK)
	{
		return 0;
	}
	return (UINT32)rankinfo->m_damage;
}

void BossDamageRankList::AddDamage(UINT64 roleid, float damage)
{
	if(CPunishMgr::Instance()->IsPunish(roleid, KKSG::PUNISH_USER_WORLDBOSS_RANK))
	{
		SSWarn<<"roleid:"<<roleid<<" damage:"<<damage<<END;
		return;
	}
	BossDamageRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(roleid, rankinfo);
	if(ret == INVALID_RANK)
	{
		BossDamageRankInfo* rankinfo = new BossDamageRankInfo(roleid);
		rankinfo->m_damage = damage;
		m_rankTree.Insert(rankinfo, roleid);
	}
	else
	{
		m_rankTree.Erase(roleid);
		rankinfo->m_damage += damage;
		m_rankTree.Insert(rankinfo, roleid);
	}
}

void BossDamageRankList::InsertKey(BossDamageRankInfo* rankinfo)
{
	if(NULL == rankinfo)
	{
		LogWarn("Insert a NULL pointer in BossDamageRanklist");
		return;
	}
	UINT64 roleid = rankinfo->GetUniqueId();
	m_rankTree.Insert(rankinfo, roleid);
}

void BossDamageRankList::SaveToDb()
{
	m_db->SendToDB();
	SetTimeStamp();
}

void BossDamageRankList::DeleteRole(UINT64 roleid)
{
	BossDamageRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(roleid, rankinfo);
	if(ret != INVALID_RANK)
	{
		m_rankTree.Erase(roleid);
		delete rankinfo;
	}
}