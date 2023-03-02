#include "pch.h"
#include "deprogressrank.h"
#include "pb/project.pb.h"
#include "table/DragonExpRankMgr.h"

#define DEPRANKCOUNT 50000

DEPRankInfo::DEPRankInfo(UINT64 roleid):CRankInfo(roleid)
{
	Reset();	
}

DEPRankInfo::~DEPRankInfo()
{

}

void DEPRankInfo::Reset()
{
	m_sceneid = 0;
	m_bossavghppercent = 0;
	m_groupid = 0;
}

void DEPRankInfo::RankInfoToKKSG(KKSG::RankData* rankdata) const
{
}

void DEPRankInfo::RankInfoToDBKKSG(KKSG::RankData* rankdata) const
{
	rankdata->set_roleid(GetUniqueId());
	rankdata->set_sceneid(m_sceneid);
	rankdata->set_bossavghppercent(m_bossavghppercent);
	rankdata->set_groupid(m_groupid);
}

void DEPRankInfo::DBKKSGToRankInfo(const KKSG::RankData* rankdata)
{
	SetUniqueId(rankdata->roleid());
	SetRankData(rankdata->sceneid(), rankdata->bossavghppercent());
	SetGroupID(rankdata->groupid());
}

void DEPRankInfo::SetRankData(UINT32 sceneid, int hppercent)
{
	m_sceneid = sceneid;
	m_bossavghppercent = hppercent;
}

void DEPDbhandler::PackDBData(KKSG::RankList* ranklist)
{
	DEPRankList* plist = static_cast<DEPRankList*>(m_ranklist);
	std::vector<DEPRankInfo*> rankinfo;
	plist->GetAllRankInfo(rankinfo);

	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		CRankInfo* info = (CRankInfo*)rankinfo[i];
		if(info)
		{
			KKSG::RankData* data = ranklist->add_rankdata();
			info->RankInfoToDBKKSG(data);
		}
	}
}

void DEPDbhandler::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	DEPRankList* plist = static_cast<DEPRankList*>(m_ranklist);
	DEPRankInfo* info = new DEPRankInfo(rankdata.roleid());
	info->DBKKSGToRankInfo(&rankdata);
	SSDebug << __FUNCTION__ << " roleid = " << info->GetUniqueId() << " group = " << info->GetGroupID() << END;
	plist->InsertKey(info);
}

bool DEPDbhandler::BeforeSaveToDB()
{
	DEPRankList* plist = static_cast<DEPRankList*>(m_ranklist);
	if(plist && plist->IsChanged())
	{
		plist->SetIsChanged(false);
		return true;
	}
	return false;
}

void DEPDbhandler::OnReceiveFromDB()
{
	SSDebug << __FUNCTION__ << END;
	//DragonExpRankMgr::Instance()->InitGroupByRankList();
}

DEPRankList::DEPRankList(int type):CRankList(type)
{
	RegisterDb(new DEPDbhandler);
	SaveToDBEveryPeriod* policy = new SaveToDBEveryPeriod;
	policy->SetSaveInterval(DEProgressUpdateInterVal);
	m_db->SetPolicy(policy);

	m_ischanged = false;
}

DEPRankList::~DEPRankList()
{
	Clear();
}

void DEPRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion /*= 0*/)
{

}

void DEPRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{

}

void DEPRankList::OnChangeRank(UINT64 roleid, UINT32 sceneid, int avghpprecent)
{
	DEPRankInfo* info = NULL;
	UINT32 ret = m_ranktree.GetKeyByID(roleid, info);
	if(ret == INVALID_RANK)
	{
		info = new DEPRankInfo(roleid);
		info->SetRankData(sceneid, avghpprecent);
		InsertKey(info);
	}
	else
	{
		if(info)
		{
			m_ranktree.Erase(roleid);
			info->SetRankData(sceneid, avghpprecent);
			InsertKey(info);
		}
		else
		{
			SSError << " dep rank info = NULL roleid = " << roleid << END;
		}
	}
	SetIsChanged(true);
}

void DEPRankList::OnChangeGroup(UINT64 roleid, int groupid)
{
	DEPRankInfo* info = NULL;
	m_ranktree.GetKeyByID(roleid, info);
	if(info)
	{
		info->SetGroupID(groupid);
	}
	else
	{
		SSError << " find not rankinfo roleid = " << roleid << END;
	}
	SetIsChanged(true);
}

void DEPRankList::InsertKey(DEPRankInfo* info)
{
	if(NULL == info)
	{
		return;
	}
	UINT64 roleid = info->GetUniqueId();
	m_ranktree.Insert(info, roleid);
	
	SetIsChanged(true);
}

void DEPRankList::GetAllRankInfo(std::vector<DEPRankInfo*>& vec)
{
	m_ranktree.GetAllKey(vec);
}

DEPRankInfo* DEPRankList::GetRankInfo(UINT64 roleid)
{
	DEPRankInfo* info = NULL;
	m_ranktree.GetKeyByID(roleid, info);
	return info;
}

void DEPRankList::Clear()
{
	std::vector<DEPRankInfo*> rankinfo;
	GetAllRankInfo(rankinfo);
	for(size_t i = 0; i < rankinfo.size(); ++i)
	{
		delete rankinfo[i];
	}
	m_ranktree.Clear();
}

UINT32 DEPRankList::GetNeedRankCount()
{
	return DEPRANKCOUNT;
}

UINT32 DEPRankList::GetRank(UINT64 roleid)
{
	return m_ranktree.GetRankByID(roleid);
}

UINT32 DEPRankList::GetSize()
{
	return m_ranktree.GetSize();
}

int DEPRankList::GetGroup(UINT64 roleid)
{
	DEPRankInfo* rankinfo = GetRankInfo(roleid);
	if(rankinfo)
	{
		return rankinfo->GetGroupID();
	}
	return 0;
}

void DEPRankList::FillProgressInfo(UINT64 roleid, KKSG::DEProgress& data)
{
	auto info = GetRankInfo(roleid);
	if(info)
	{
		data.set_sceneid(info->GetSceneID());
		data.set_bossavghppercent(info->GetBossAVGHP());
	}
}

bool DEPRankList::IsFull()
{
	return m_ranktree.GetSize() >= GetNeedRankCount();
}

bool DEPRankList::HaveRole(UINT64 roleid)
{
	return (NULL != GetRankInfo(roleid));
}
