#include "pch.h"
#include "skycityranklist.h"

const UINT32 SkyCityRankCount = 30;

SkyCityRankInfo::SkyCityRankInfo(UINT64 roleID)
	:CRankInfo(roleID)
{

}

SkyCityRankInfo::~SkyCityRankInfo()
{

}

void SkyCityRankInfo::RankInfoToKKSG(KKSG::RankData* rankdata) const
{
	*rankdata->mutable_skycity() = m_data;
}

void SkyCityRankInfo::RankInfoToDBKKSG(KKSG::RankData* rankdata) const
{
	*rankdata->mutable_skycity() = m_data;
}

void SkyCityRankInfo::DBKKSGToRankInfo(const KKSG::RankData* rankdata)
{
	m_data = rankdata->skycity();
	SetUniqueId(m_data.roleid());//±ØÒª
}

void SkyCityRankInfo::InfoPrint()
{
	SSInfo << " roleid = " << m_data.roleid() << " rolename = " << m_data.rolename()  << " floor = " << m_data.floor() << END;
}

void SkyCityRankDb::PackDBData(KKSG::RankList* ranklist)
{
	if(NULL == ranklist)
	{
		return;
	}
	SkyCityRankList* srlist = static_cast<SkyCityRankList*>(m_ranklist);
	if(NULL == srlist)
	{
		return;
	}
	std::vector<SkyCityRankInfo*> allinfo;
	srlist->GetAllInfo(allinfo);
	for(size_t i = 0; i < allinfo.size(); ++i)
	{
		if(allinfo[i])
		{
			allinfo[i]->RankInfoToDBKKSG(ranklist->add_rankdata());
		}
	}
}

void SkyCityRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	SkyCityRankList* srlist = static_cast<SkyCityRankList*>(m_ranklist);
	if(NULL == srlist)
	{
		return;
	}
	srlist->OnChange(rankdata.skycity());	
}

bool SkyCityRankDb::BeforeSaveToDB()
{
	SkyCityRankList* srlist = static_cast<SkyCityRankList*>(m_ranklist);
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

SkyCityRankList::SkyCityRankList(int type)
	:CRankList(type)
{
	RegisterDb(new SkyCityRankDb);

	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;
	policy->SetSaveInterval(GetUpdateInterval());
	m_db->SetPolicy(policy);

	SetChanged(false);
}

SkyCityRankList::~SkyCityRankList()
{
	Clear();
}

void SkyCityRankList::Clear()
{
	std::vector<SkyCityRankInfo*> allinfo;
	GetAllInfo(allinfo);
	for(size_t i = 0; i < allinfo.size(); ++i)
	{
		delete allinfo[i];
	}
	m_rankTree.Clear();
	allinfo.clear();
	SetChanged(true);
}

int SkyCityRankList::GetUpdateInterval()
{
	return SkyCityUpdateInterval;
}

UINT32 SkyCityRankList::GetNeedRankCount()
{
	return SkyCityRankCount;
}

UINT32 SkyCityRankList::GetSize()
{
	return m_rankTree.GetSize();
}

void SkyCityRankList::GetAllInfo(std::vector<SkyCityRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}

void SkyCityRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion/* = 0*/)
{
	std::vector<SkyCityRankInfo*> rankinfo;
	m_rankTree.GetSomeTopKey(count, rankinfo);
	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		if(rankinfo[i])
		{
			rankinfo[i]->RankInfoToKKSG(rankdatas->add_rankdata());
		}
	}
}

void SkyCityRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	if(NULL == rankdata)
	{
		return;
	}
	SkyCityRankInfo* info = NULL;
	m_rankTree.GetKeyByRank(rank, info);
	if(info)
	{
		info->RankInfoToKKSG(rankdata);
	}
		
}

void SkyCityRankList::OnChange(const KKSG::SkyCityRankData& data)
{
	if(0 == data.roleid())	
	{
		return;
	}
	SkyCityRankInfo* info = NULL;
	UINT32 ret = m_rankTree.GetKeyByID(data.roleid(), info);
	if(NULL == info)
	{
		info = new SkyCityRankInfo(data.roleid());
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

void SkyCityRankList::DeleteLast()
{
	if(GetSize() == 0)
	{
		return;
	}
	SkyCityRankInfo* info = NULL;
	UINT32 ret = m_rankTree.GetKeyByRank(GetSize(), info);
	if(INVALID_RANK != ret && info)
	{
		m_rankTree.Erase(info->GetUniqueId());
		delete info;
		info = NULL;
		SetChanged(true);
	}
}

void SkyCityRankList::InfoPrint()
{
	std::vector<SkyCityRankInfo*> allinfo;
	GetAllInfo(allinfo);
	SSInfo << " size = " << allinfo.size() << END;
	for(size_t i = 0; i < allinfo.size(); ++i)
	{
		allinfo[i]->InfoPrint();
	}
}

UINT32 SkyCityRankList::GetRank(UINT64 roleid)
{
	return m_rankTree.GetRankByID(roleid);
}
