#include "pch.h"
#include "military_manager.h"
#include "ranklist/military_ranklist.h"
#include "ranklist/ranklistmgr.h"
#include "table/globalconfig.h"
#include "event/eventmgr.h"

#include "util/gametime.h"
#include "util/XCommon.h"
#include "global/GlobalConfigDBBuild.h"
#include "mail/mailmgr.h"
#include "mail/mailconfig.h"
#include "define/maildef.h"
#include "network/dblink.h"
#include "role/rolemanager.h"
#include "militaryrank/ptcm2g_militaryrankchange.h"
#include "role/rolesummarymgr.h"
#include "notice/notice.h"

const UINT32 TopMilitaryRank  = 5;

INSTANCE_SINGLETON(MilitaryManager);
MilitaryManager::MilitaryManager()
{

}

MilitaryManager::~MilitaryManager()
{

}

bool MilitaryManager::Init()
{
	handler_ = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);	

	last_month_reward_ = GameTime::GetTime();

	std::string lastTimeValue = CGlobalConfigDBBuild::Instance()->GetConfigValue(eMilitaryRankTime);
	if (lastTimeValue.empty())
	{
		CGlobalConfigDBBuild::Instance()->SetConfigValue(eHeroRankUpdateTime, ToString(last_month_reward_));
		CGlobalConfigDBBuild::Instance()->UpdateToDb(eMilitaryRankTime);		
	}
	else
	{
		last_month_reward_ = convert<time_t>(lastTimeValue);
	}

	return LoadFile();
}

void MilitaryManager::Uninit()
{
	CTimerMgr::Instance()->KillTimer(handler_);
}

void MilitaryManager::ClearFile()
{
	militaryrank_reward_.Clear();
}
bool MilitaryManager::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();

	if (!militaryrank_reward_.LoadFile("table/MilitaryRankReward.txt"))
	{
		SSWarn<<"load file table/MilitaryRankReward.txt failed"<<END;
		return false;
	}
	return true;
}

bool MilitaryManager::CheckFile()
{
	MilitaryRankReward temp_reward;
	if (!temp_reward.LoadFile("table/MilitaryRankReward.txt"))
	{
		SSWarn<<"load file table/MilitaryRankReward.txt failed"<<END;
		return false;
	}
	return true;
}

UINT32 MilitaryManager::GetRank(UINT64 roleid)
{
	MilitaryRankList* ranklist = (MilitaryRankList*)CRankListMgr::Instance()->GetRankList(KKSG::MilitaryRank);
	if (NULL == ranklist)
	{
		SSError << " find not MilitaryRankList " << END;
		return INVALID_RANK;
	}
	return ranklist->GetRank(roleid);
}

UINT32 MilitaryManager::GetAllRank()
{
	MilitaryRankList* ranklist = (MilitaryRankList*)CRankListMgr::Instance()->GetRankList(KKSG::MilitaryRank);
	if (NULL == ranklist)
	{
		SSError << " find not MilitaryRankList " << END;
		return 0;
	}
	return ranklist->GetSize();
}

void MilitaryManager::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	CheckMonthRankRewrad();	
}

void MilitaryManager::OnDesignation(UINT64 nRoleID,UINT32 nRank)
{
	if (nRank<=100)
	{
		EventMgr::Instance()->AddEvent(nRoleID,DESIGNATION_COM_MILITARY,nRank);
	}
}

//
void MilitaryManager::CheckMonthRankRewrad()
{
	time_t now = GameTime::GetTime();
	time_t offset = time_t(GetGlobalConfig().PkWeekOffset);
	if(XCommon::IsMonthSame(last_month_reward_ + offset, now + offset, false))
	{
		return ;
	}

	last_month_reward_ = now;
	if (DBLink::Instance()->IsConnected())
	{
		CGlobalConfigDBBuild::Instance()->SetConfigValue(eMilitaryRankTime, ToString(last_month_reward_));
		CGlobalConfigDBBuild::Instance()->UpdateToDb(eMilitaryRankTime);
	}
	MilitaryRankList* ranklist = (MilitaryRankList*)CRankListMgr::Instance()->GetRankList(KKSG::MilitaryRank);
	if (NULL == ranklist)
	{
		SSError << " find not MilitaryRank " << END;
		return ;
	}

	std::vector<MilitaryRankInfo*> allinfo;
	ranklist->GetRankInfo(allinfo);

	std::string mailtitle = "null";
	std::string mailcontent;
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(MailConf_Military_Award);
	if(NULL == pMailConf)
	{
		SSError << " find not pMailConf by MailConf_Military_Award " << END;
		return ;
	}
	mailtitle = pMailConf->m_title;
	mailcontent = pMailConf->m_content;

	std::vector<std::string> role_top_rank;
	int RankIndex = 0;
	std::vector<ItemDesc> rewards;
	for(size_t i = 0; i < allinfo.size(); ++i)
	{
		UINT64 roleid = allinfo[i]->GetUniqueId();		
		UINT32 rank = (++RankIndex);
		OnDesignation(roleid,rank);
		rewards.clear();

		GetMilitaryRankReward(rank,rewards);
		if(!rewards.empty())
		{
			std::string content = MailConfig::Instance()->ReplaceString("%s", ToString(rank), mailcontent);

			MailData maildata(mailtitle, content, rewards, MAIL_EXPIRE_TIME);
			maildata.SetReason(ItemFlow_MilitaryRankAward);
			CMailMgr::Instance()->SendMail(roleid, maildata);
		}

		CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);	
		if (NULL != role)
		{
			PtcM2G_MilitaryRankChange ptc;
			ptc.m_Data.set_role_id(roleid);
			ptc.m_Data.set_military_rank(0);
			role->SendMsgToGS(ptc);
			if (role_top_rank.size() < TopMilitaryRank)
			{
				role_top_rank.push_back(role->GetName());
			}
		}
		else
		{
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
			if (NULL != summary)
			{
				if (role_top_rank.size() < TopMilitaryRank)
				{
					role_top_rank.push_back(summary->GetName());
				}
			}
		}
	}

	if (role_top_rank.size() > 0)
	{
		std::string names;
		auto it = role_top_rank.begin();
		auto it_end = role_top_rank.end();
		for (; it != it_end; ++it)
		{
			names += (*it) +",";
		}
		auto pos = names.rfind(',');
		auto  newname = names.substr(0,pos);

		Notice notice(NoticeMilitaryRankAward);
		notice.Replace("$R", newname);
		notice.Send();
	}
	ranklist->Clear();//≈≈––∞Ò«Âø’
}

void  MilitaryManager::GetMilitaryRankReward(UINT32 rank,std::vector<ItemDesc>& items)
{
	for (auto it = militaryrank_reward_.Table.begin(); it != militaryrank_reward_.Table.end(); ++it)
	{	
		if ((*it)->Rank[0] <= rank && (*it)->Rank[1] >= rank)
		{
			for (auto j = (*it)->Reward.begin(); j != (*it)->Reward.end(); ++j)
			{
				items.push_back(ItemDesc(j->seq[0], j->seq[1]));	
			}
		}
	}
}

void MilitaryManager::TestPrint()
{
	MilitaryRankList* ranklist = (MilitaryRankList*)CRankListMgr::Instance()->GetRankList(KKSG::MilitaryRank);
	if (NULL == ranklist)
	{
		SSError << " find not MilitaryRankList " << END;
		return ;
	}
	std::vector<MilitaryRankInfo*> vec;
	ranklist->GetRankInfo(vec);

	for (auto it = vec.begin(); it != vec.end(); ++it)
	{
		LogDebug("roleid:%llu,rank:%d,exploit:%d,mililaryrank:%d,time:%d",
			(*it)->GetUniqueId(),
			ranklist->GetRank((*it)->GetUniqueId()),
			(*it)->military_exploit_,
			(*it)->military_rank_,
			(*it)->time_);
	}
}

void MilitaryManager::TestClear()
{
	MilitaryRankList* ranklist = (MilitaryRankList*)CRankListMgr::Instance()->GetRankList(KKSG::MilitaryRank);
	if (NULL == ranklist)
	{
		SSError << " find not MilitaryRankList " << END;
		return ;
	}
	ranklist->Clear();
}
