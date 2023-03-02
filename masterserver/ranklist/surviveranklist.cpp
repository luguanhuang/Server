#include "pch.h"
#include "surviveranklist.h"

const UINT32 SurviveRankCount = 1000;

SurviveRankInfo::SurviveRankInfo(UINT64 roleID)
	:CRankInfo(roleID)
{

}

SurviveRankInfo::~SurviveRankInfo()
{

}

void SurviveRankInfo::RankInfoToKKSG(KKSG::RankData* rankdata) const
{
	*rankdata->mutable_survive() = m_data;
}

void SurviveRankInfo::RankInfoToDBKKSG(KKSG::RankData* rankdata) const
{
	*rankdata->mutable_survive() = m_data;
}

void SurviveRankInfo::DBKKSGToRankInfo(const KKSG::RankData* rankdata)
{
	m_data = rankdata->survive();
	SetUniqueId(m_data.roleid());//±ØÒª
}

void SurviveRankInfo::InfoPrint()
{
	SSInfo << " roleid = " << m_data.roleid() << " rolename = " << m_data.rolename() << " server = " << m_data.serverid() << " point = " << m_data.point() << END;
}

void SurviveRankDb::PackDBData(KKSG::RankList* ranklist)
{
	if(NULL == ranklist)
	{
		return;
	}
	SurviveRankList* srlist = static_cast<SurviveRankList*>(m_ranklist);
	if(NULL == srlist)
	{
		return;
	}
	std::vector<SurviveRankInfo*> allinfo;
	srlist->GetAllInfo(allinfo);
	for(size_t i = 0; i < allinfo.size(); ++i)
	{
		if(allinfo[i])
		{
			allinfo[i]->RankInfoToDBKKSG(ranklist->add_rankdata());
		}
	}
}

void SurviveRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	SurviveRankList* srlist = static_cast<SurviveRankList*>(m_ranklist);
	if(NULL == srlist)
	{
		return;
	}
	srlist->OnChange(rankdata.survive());	
}

bool SurviveRankDb::BeforeSaveToDB()
{
	SurviveRankList* srlist = static_cast<SurviveRankList*>(m_ranklist);
	if(NULL == srlist)
	{
		return false;
	}
	if(srlist->IsChanged())
	{
		srlist->SetChanged(false);
		return true;
	}
	return false;

}

SurviveRankList::SurviveRankList(int type)
	:CRankList(type)
{
	RegisterDb(new SurviveRankDb);

	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;
	policy->SetSaveInterval(GetUpdateInterval());
	m_db->SetPolicy(policy);

	SetChanged(false);
}

SurviveRankList::~SurviveRankList()
{
	Clear();
}

void SurviveRankList::Clear()
{
	std::vector<SurviveRankInfo*> allinfo;
	GetAllInfo(allinfo);
	for(size_t i = 0; i < allinfo.size(); ++i)
	{
		delete allinfo[i];
	}
	m_rankTree.Clear();
	allinfo.clear();
	SetChanged(true);
}

int SurviveRankList::GetUpdateInterval()
{
	return SurviveUpdateInterval;
}

UINT32 SurviveRankList::GetNeedRankCount()
{
	return SurviveRankCount;
}

UINT32 SurviveRankList::GetSize()
{
	return m_rankTree.GetSize();
}

void SurviveRankList::GetAllInfo(std::vector<SurviveRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}

void SurviveRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion/* = 0*/)
{
	std::vector<SurviveRankInfo*> rankinfo;
	m_rankTree.GetSomeTopKey(count, rankinfo);
	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		if(rankinfo[i])
		{
			rankinfo[i]->RankInfoToKKSG(rankdatas->add_rankdata());
		}
	}
}

void SurviveRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	if(NULL == rankdata)
	{
		return;
	}
	SurviveRankInfo* info = NULL;
	m_rankTree.GetKeyByRank(rank, info);
	if(info)
	{
		info->RankInfoToKKSG(rankdata);
	}
		
}

void SurviveRankList::OnChange(const KKSG::SurviveRankData& data)
{
	if(0 == data.roleid() || 0 == data.serverid())	
	{
		return;
	}
	SurviveRankInfo* info = NULL;
	UINT32 ret = m_rankTree.GetKeyByID(data.roleid(), info);
	if(NULL == info)
	{
		info = new SurviveRankInfo(data.roleid());
	}
	else
	{
		m_rankTree.Erase(data.roleid());
	}

	if(info)
	{
		info->SetData(data);
		m_rankTree.Insert(info, info->GetUniqueId());
	}

	if(GetSize() > GetNeedRankCount())
	{
		DeleteLast();
	}
	
	SetChanged(true);
}

void SurviveRankList::DeleteLast()
{
	if(GetSize() == 0)
	{
		return;
	}
	SurviveRankInfo* info = NULL;
	UINT32 ret = m_rankTree.GetKeyByRank(GetSize(), info);
	if(INVALID_RANK != ret && info)
	{
		m_rankTree.Erase(info->GetUniqueId());
		delete info;
		info = NULL;
		SetChanged(true);
	}
}

void SurviveRankList::InfoPrint()
{
	std::vector<SurviveRankInfo*> allinfo;
	GetAllInfo(allinfo);
	SSInfo << " size = " << allinfo.size() << END;
	for(size_t i = 0; i < allinfo.size(); ++i)
	{
		allinfo[i]->InfoPrint();
	}
}

UINT32 SurviveRankList::GetRank(UINT64 roleid)
{
	return m_rankTree.GetRankByID(roleid);
}
