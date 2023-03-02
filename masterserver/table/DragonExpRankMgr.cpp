#include "pch.h"
#include "DragonExpRankMgr.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/deprogressrank.h"
#include "role/rolesummarymgr.h"
#include "mail/mailmgr.h"
#include "table/globalconfig.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "stage/ptcm2c_derankchangntf.h"
#include "mail/mailconfig.h"
#include "global/GlobalConfigDBBuild.h"
#include "network/dblink.h"

INSTANCE_SINGLETON(DragonExpRankMgr)

DragonExpRankMgr::DragonExpRankMgr()
{
	m_handler = 0;
	m_lastGiveRewardTime = 0;
}

DragonExpRankMgr::~DragonExpRankMgr()
{

}

bool DragonExpRankMgr::Init()
{
	if (!m_rrTable.LoadFile("table/DragonExpRankReward.txt"))
	{
		SSWarn<<"load file table/DragonExpRankReward.txt failed"<<END;
		return false;
	}

	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 3000, -1, __FILE__, __LINE__);	

	ReadDBData();
	InitGroupByRankList();

	return true;
}

void DragonExpRankMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
}

//分组和排名相关联 需要保持一致 现在个数硬性限制
void DragonExpRankMgr::SynDragonExpProgress(UINT64 roleid, UINT32 sceneid, int avghppercent, bool isbattleend)
{
	DEPRankList* ranklist = GetDEPRankList();
	if(NULL == ranklist)
	{
		return ;	
	}

	if(!ranklist->HaveRole(roleid) && ranklist->IsFull())
	{
		SSWarn << " de rank full roleid = " << roleid << END;
		return ;
	}

	int tmpgroup = ranklist->GetGroup(roleid);
	int oldgrouprank = 0;
	if(tmpgroup)
	{
		auto pGroup = GetGroupInfo(tmpgroup);
		if(pGroup)
		{
			oldgrouprank = pGroup->GetRoleGroupRank(roleid);
		}
	}
	
	ranklist->OnChangeRank(roleid, sceneid, avghppercent);
	int group = ranklist->GetGroup(roleid);
	if(0 == group)//新加的
	{
		UINT32 rank = ranklist->GetRank(roleid);
		if(rank)
		{
			group = AddNewRole(roleid);		
			ranklist->OnChangeGroup(roleid, group);
		}
		else
		{
			SSError << " add new role rank = 0 roleid = " << roleid << END;
		}
	}
	else if(isbattleend)
	{   
		int newgrouprank = 0;
		auto pGroup = GetGroupInfo(group);
		if(pGroup)
		{
			newgrouprank = pGroup->GetRoleGroupRank(roleid);
		}
		if(oldgrouprank && newgrouprank && oldgrouprank > newgrouprank)
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
			if(pRole && ROLE_STATE_IN_GAME == pRole->GetState())
			{
				PtcM2C_DERankChangNtf ptc;
				ptc.m_Data.set_oldrank(oldgrouprank);
				ptc.m_Data.set_newrank(newgrouprank);
				pRole->Send(ptc);
				//SSWarn << " oldgr = " << oldgrouprank << " newgr = " << newgrouprank << END;
			}
		}

	}

}

int DragonExpRankMgr::AddNewRole(UINT64 roleid)
{
	if(m_groups.empty())
	{
		DragonExpGroup deg;
		deg.SetGroup(1);
		deg.AddRole(roleid);
		m_groups.insert(std::make_pair(deg.GetGroup(), deg));
		return deg.GetGroup();
	}
	else
	{
		auto it = m_groups.end();
		--it;
		DragonExpGroup& lastdeg = (it->second);
		if(lastdeg.IsFull())
		{
			DragonExpGroup deg;
			deg.SetGroup(lastdeg.GetGroup() + 1);
			deg.AddRole(roleid);
			m_groups.insert(std::make_pair(deg.GetGroup(), deg));
			return deg.GetGroup();
		}
		else
		{
			lastdeg.AddRole(roleid);
			return lastdeg.GetGroup();
		}
	}

	return 0;
}

DragonExpGroup* DragonExpRankMgr::GetGroupInfo(int groupid)
{
	auto it = m_groups.find(groupid);
	if(it != m_groups.end())
	{
		return &(it->second);		
	}
	return NULL;
}

void DragonExpRankMgr::FillRankData(UINT64 roleid, KKSG::DERankRes& roRes)
{
	auto* ranklist = GetDEPRankList();
	if(NULL == ranklist)
	{
		return ;	
	}
	int group = ranklist->GetGroup(roleid);
	if(0 == group)
	{
		return ;
	}
	DragonExpGroup* groupinfo = GetGroupInfo(group);
	if(groupinfo)
	{
		int groupRank = 0;
		std::vector<UINT64> grouproles;
		groupinfo->GetRankAllRoleid(grouproles);
		for(auto it = grouproles.begin(); it != grouproles.end(); ++it)
		{
			++groupRank;
			UINT64 roleid = (*it);
			KKSG::DERank* derank = roRes.add_ranks();
			
			FillRankInfo(roleid, groupRank, *derank, ranklist);
			
			//SSDebug << __FUNCTION__ << " grouprank = " << groupRank << " roleid = " << roleid << " sceenid = " << derank->progress().sceneid() << " avg = " << derank->progress().bossavghppercent() << END;
		}
	}
	roRes.set_rewardlefttime(GetGiveRewardLeftTime(GameTime::GetTime()));
}

void DragonExpRankMgr::FillRankInfo(UINT64 roleid, UINT32 grouprank, KKSG::DERank& info, DEPRankList* ranklist)
{
	if(NULL == ranklist)
	{
		return;
	}
	info.set_rank(grouprank);
	info.set_roleid(roleid);
	info.set_rolename(CRoleSummaryMgr::Instance()->GetRoleName(roleid));
	FillRankReward(grouprank, info);

	ranklist->FillProgressInfo(roleid, *info.mutable_progress());
}

void DragonExpRankMgr::FillRankReward(UINT32 grouprank, KKSG::DERank& info)
{
	for(size_t i = 0; i < m_rrTable.Table.size(); ++i)
	{
		DragonExpRankReward::RowData* pConf = m_rrTable.Table[i];
		if(pConf->Rank[0] <= (int)grouprank && (int)grouprank <= pConf->Rank[1])
		{
			for(size_t j = 0; j < pConf->Reward.size(); ++j)
			{
				KKSG::ItemBrief* ibrief = info.add_reward();
				ibrief->set_itemid(pConf->Reward[j][0]);
				ibrief->set_itemcount(pConf->Reward[j][1]);
			}
			break;
		}
	}
}

void DragonExpRankMgr::GetRankReward(UINT32 grouprank, std::vector<ItemDesc>& rewards)
{
	rewards.clear();
	for(size_t i = 0; i < m_rrTable.Table.size(); ++i)
	{
		DragonExpRankReward::RowData* pConf = m_rrTable.Table[i];
		if(pConf->Rank[0] <= (int)grouprank && (int)grouprank <= pConf->Rank[1])
		{
			for(size_t j = 0; j < pConf->Reward.size(); ++j)
			{
				ItemDesc desc;
				desc.itemID = pConf->Reward[j][0];
				desc.itemCount = pConf->Reward[j][1];
				desc.isbind = false;
				rewards.push_back(desc);
			}
			break;
		}
	}
}

void DragonExpRankMgr::GiveRankReward()
{
	SSDebug << __FUNCTION__ << END;
	std::string mailtitle = "null";
	std::string mailcontent = "null";
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(MailConf_DEProgress);
	if(pMailConf)
	{
		mailtitle = pMailConf->m_title;
		mailcontent = pMailConf->m_content;
	}
	std::vector<std::string> vec;
	vec = Split(mailcontent, "x");

	std::vector<ItemDesc> rewards;
	for(auto it = m_groups.begin(); it != m_groups.end(); ++it)
	{
		DragonExpGroup& group = (it->second);
		std::vector<UINT64> grouproles;
		group.GetRankAllRoleid(grouproles);
		int grouprank = 0;
		for(auto j = grouproles.begin(); j != grouproles.end(); ++j)
		{
			++grouprank;
			rewards.clear();
			GetRankReward(grouprank, rewards);
			std::string tmpcontent = mailcontent;
			if(2 == vec.size())
			{
				tmpcontent = vec[0] + ToString(grouprank) + vec[1];		
			}
			MailData maildata(mailtitle, tmpcontent, rewards, MAIL_EXPIRE_TIME);
			maildata.SetReason(ItemFlow_DragonExpRankAward);
			CMailMgr::Instance()->SendMail(*j, maildata);
		}
	}
}

void DragonExpRankMgr::ResetGroup()
{
	//SSDebug << __FUNCTION__ << END;
	m_groups.clear();
	DEPRankList* ranklist = GetDEPRankList();
	if(NULL == ranklist)
	{
		return ;	
	}
	std::vector<DEPRankInfo*> allrank;
	ranklist->GetAllRankInfo(allrank);
	int GroupIndex = 0;
	int PGroupRoleCount = GetGlobalConfig().DEGroupRoleCount;
	int PRandomGroupCount = RANDOMGROUPCOUNT;
	for(size_t i = 0; i < allrank.size(); i = i + (PGroupRoleCount * PRandomGroupCount))
	{
		std::vector<UINT32> randIndex;
		for(size_t j = i; j < allrank.size() && j < i + (PGroupRoleCount * PRandomGroupCount); ++j)
		{
			randIndex.push_back(j);
		}
		if(randIndex.empty())
		{
			continue;
		}
		std::random_shuffle(randIndex.begin(), randIndex.end());
		DragonExpGroup group;
		for(size_t rindex = 0; rindex < randIndex.size(); ++rindex)
		{
			UINT32 j = randIndex[rindex];
			if(j >= allrank.size())
			{
				SSError << " dep rank index err j = " << j << " allsize = " << (int)(allrank.size()) << END;
				continue;
			}

			if(group.IsEmpty() && 0 == group.GetGroup())
			{
				group.SetGroup(++GroupIndex);// 每一个group 只执行一次
			}
			auto info = allrank[j];
			group.AddRole(info->GetUniqueId());
			info->SetGroupID(group.GetGroup());

			if(group.IsFull())
			{
				m_groups.insert(std::make_pair(group.GetGroup(), group));
				group.Clear();
			}
		}
		if(!group.IsEmpty() && group.GetGroup())
		{
			m_groups.insert(std::make_pair(group.GetGroup(), group));
		}
	}
}

void DragonExpRankMgr::TestPrint()
{
	auto ranklist = GetDEPRankList();
	if(NULL == ranklist)
	{
		return ;
	}
	std::vector<DEPRankInfo*> allrank;
	ranklist->GetAllRankInfo(allrank);
	for(size_t i = 0; i < allrank.size(); ++i)
	{
		SSDebug << " rank = " << (ranklist->GetRank(allrank[i]->GetUniqueId())) << " roleid = " << allrank[i]->GetUniqueId() 
			<< " group = " << allrank[i]->GetGroupID() << " sceneid = " << allrank[i]->GetSceneID() 
			<< " avghppercent = " << allrank[i]->GetBossAVGHP() << END; 
	}
	for(auto it = m_groups.begin(); it != m_groups.end(); ++it)
	{
		(it->second).TestPrint();
	}
}

DEPRankList* DragonExpRankMgr::GetDEPRankList()
{
	DEPRankList* ranklist = (DEPRankList*)(CRankListMgr::Instance()->GetRankList(KKSG::DEProgressRank));
	if(NULL == ranklist)
	{
		SSError << " find dep ranklist err null " << END;
		return NULL;
	}
	return ranklist;
}

void DragonExpRankMgr::InitGroupByRankList()
{
	m_groups.clear();
	auto ranklist = GetDEPRankList();
	if(NULL == ranklist)
	{
		return;
	}
	std::vector<DEPRankInfo*> allrank;
	ranklist->GetAllRankInfo(allrank);
	for(size_t i = 0; i < allrank.size(); ++i)
	{
		UINT64 roleid = allrank[i]->GetUniqueId();
		UINT32 group = allrank[i]->GetGroupID();
		auto groupinfo = GetGroupInfo(group);
		if(groupinfo)
		{
			groupinfo->AddRole(roleid);	
		}
		else
		{
			DragonExpGroup groupnew;
			groupnew.SetGroup(group);
			groupnew.AddRole(roleid);
			m_groups.insert(std::make_pair(groupnew.GetGroup(), groupnew));
		}
	}
}

void DragonExpRankMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if(!DBLink::Instance()->IsConnected())
	{
		return;
	}
	time_t now = GameTime::GetTime();

	if(0 == m_lastGiveRewardTime)
	{
		m_lastGiveRewardTime = now;
		WriteDBData();
		return;
	}

	if(0 == GetGiveRewardLeftTime(now))
	{
		GiveRankReward();
		ResetGroup();
		m_lastGiveRewardTime = now;
		WriteDBData();
	}
	//SSWarn << __FUNCTION__ << " lefttime = " << GetGiveRewardLeftTime(now) << END;
}

UINT32 DragonExpRankMgr::GetGiveRewardLeftTime(time_t now)
{
	if( GetGlobalConfig().DERankRewardDay.empty())
	{
		return UINT32(-1);
	}
	const auto& conf = GetGlobalConfig().DERankRewardDay;
	UINT32 onedaytime = (UINT32)XCommon::GetOneDayTime();
	
	for(auto it = conf.begin(); it != conf.end(); ++it)
	{
		UINT32 tmp = (*it) * onedaytime;
		if(!XCommon::IsWeekSame(m_lastGiveRewardTime - tmp, now - tmp))//偏移5hour
		{
			return 0;
		}
	}
	
	UINT32 nowday = XCommon::GetWeekNum(now, true);
	auto it = conf.lower_bound(nowday);
	if(it == conf.end())
	{
		it = conf.begin();
	}
	UINT32 tmp = (*it >= nowday)?(*it - nowday):(*it + 7 - nowday);
	tmp = tmp * onedaytime + (onedaytime - UINT32(now - XCommon::GetDayBeginTime(now, true)));
	return tmp;
}

void DragonExpRankMgr::ReadDBData()
{
	std::string t = CGlobalConfigDBBuild::Instance()->GetConfigValue(eDragonExpLastUpTime);
	m_lastGiveRewardTime = time_t(convert<UINT32>(t));

	SSInfo << t << " " << m_lastGiveRewardTime << END;
}

void DragonExpRankMgr::WriteDBData()
{
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eDragonExpLastUpTime, ToString(m_lastGiveRewardTime));
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eDragonExpLastUpTime);

	SSInfo << m_lastGiveRewardTime << END;
}

DragonExpGroup::DragonExpGroup()
{
	m_groupid = 0;
}

void DragonExpGroup::SetGroup(int group)
{
	m_groupid = group;
}

void DragonExpGroup::AddRole(UINT64 roleid)
{
	m_roleids.insert(roleid);
}

bool DragonExpGroup::HaveRole(UINT64 roleid)
{
	return (m_roleids.find(roleid) != m_roleids.end());
}

typedef std::pair<UINT32, UINT64> DEPPAIR;
void DragonExpGroup::GetRankAllRoleid(std::vector<UINT64>& roleids)
{
	auto ranklist = DragonExpRankMgr::Instance()->GetDEPRankList();
	if(NULL == ranklist)
	{
		return ;
	}
	roleids.clear();
	std::priority_queue<DEPPAIR, std::vector<DEPPAIR>, std::greater<DEPPAIR> > prioQueue;
	for(auto it = m_roleids.begin(); it != m_roleids.end(); ++it)
	{
		UINT64 roleid = (*it);
		UINT32 rank = ranklist->GetRank(roleid);
		prioQueue.push(DEPPAIR(rank, roleid));
	}
	while(!prioQueue.empty())
	{
		roleids.push_back(prioQueue.top().second);
		prioQueue.pop();
	}
}

int DragonExpGroup::GetRoleGroupRank(UINT64 roleid)
{
	std::vector<UINT64> roleids;
	GetRankAllRoleid(roleids);
	for(int i = 0; i < (int)roleids.size(); ++i)
	{
		if(roleids[i] == roleid)
		{
			return (i+1);
		}
	}
	return 0;
}

void DragonExpGroup::Clear()
{
	m_groupid = 0;
	m_roleids.clear();
}

bool DragonExpGroup::IsFull()
{
	return ((int)m_roleids.size() >= GetGlobalConfig().DEGroupRoleCount);
}

void DragonExpGroup::TestPrint()
{
	auto ranklist = DragonExpRankMgr::Instance()->GetDEPRankList();
	if(NULL == ranklist)
	{
		return ;
	}
	SSDebug << __FUNCTION__ << " group = " << GetGroup() << END;
	for(auto it = m_roleids.begin(); it != m_roleids.end(); ++it)
	{
		SSDebug << __FUNCTION__ << " rank = " << ranklist->GetRank(*it) << " roleid = " << (*it) << END;
	}
	std::vector<UINT64> grouproles;
	GetRankAllRoleid(grouproles);
	int GroupRank = 0;
	for(size_t i = 0; i < grouproles.size(); ++i)
	{
		++GroupRank;
		SSDebug << __FUNCTION__ << " grouprank = " << GroupRank << " roleid = " << grouproles[i] << END;
	}
}
