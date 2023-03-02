#include "pch.h"
#include "flowerranklist.h"
#include "pb/project.pb.h"
#include "util/timespecificmgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/role.h"
#include "role/rolesummarymgr.h"
#include "notice/noticemgr.h"
#include "util/gametime.h"
#include "account/punishmgr.h"
#include "util/XCommon.h"
#include "social/flower.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "platforminfo/platformfriendmgr.h"
#include "social/socialmgr.h"
#include "social/social.h"
#include "ranklist/ranklistmgr.h"
#include "table/OpenSystemMgr.h"
#include "define/systemiddef.h"
#include "notice/noticemgr.h"
#include "social/socialmgr.h"

FlowerRankInfo::FlowerRankInfo(UINT64 roleid)
:CRankInfo(roleid)
{
	m_flowercount = 0;
	m_time = 0;
}

FlowerRankInfo::~FlowerRankInfo()
{

}

void FlowerRankInfo::Reset()
{

}

void FlowerRankInfo::RankInfoToKKSG(KKSG::RankData* pRankData) const
{
	if (NULL == pRankData || 0 == GetUniqueId())
	{
		return;
	}
	CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(GetUniqueId());
	if (NULL == pRoleSummary)
	{
		return;
	}
	for (auto iter = m_mapFlowers.begin(); iter != m_mapFlowers.end(); ++iter)
	{
		KKSG::MapIntItem* item = pRankData->add_receiveflowers();
		item->set_key(iter->first);
		item->set_value(iter->second);
	}
	pRankData->set_roleid(GetUniqueId());
	pRankData->set_flowercount(m_flowercount);
	pRankData->set_rolename(pRoleSummary->GetName());
	pRankData->set_profession(pRoleSummary->GetProfession());
	pRankData->set_is_vip(pRoleSummary->IsQQVip());
	pRankData->set_is_svip(pRoleSummary->IsQQSVip());
	pRankData->set_starttype(pRoleSummary->GetStartUpType());
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(GetUniqueId());
	if (NULL != guild)
	{
		pRankData->set_guildicon(guild->GetIcon());
		pRankData->set_guildname(guild->GetName());
	}
	const PlatUserInfo* info = CPlatformFriendMgr::Instance()->GetPlatSelfInfo(pRoleSummary->GetAccount());
	if (NULL != info)
	{
		pRankData->set_headpic(info->bigpic);
	}
}

void FlowerRankInfo::RankInfoToDBKKSG(KKSG::RankData* pRankData) const
{
	if (NULL == pRankData)
	{
		return;
	}
	pRankData->set_roleid(GetUniqueId());
	pRankData->set_flowercount(m_flowercount);
}

void FlowerRankInfo::DBKKSGToRankInfo(const KKSG::RankData* pRankData)
{
	if (NULL == pRankData)
	{
		return;
	}
	SetUniqueId(pRankData->roleid());
	m_flowercount = pRankData->flowercount();
}

//////////////////////////////////////////////////////////////////////
FlowerRankList::FlowerRankList(UINT32 type)
:CRankList(type)
{
	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	m_timehandle = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&FlowerRankList::OnTimer, this));
}

FlowerRankList::~FlowerRankList()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_timehandle);

	Clear();
}

void FlowerRankList::Clear()
{
	std::vector<FlowerRankInfo*> flowerinfo;
	m_rankTree.GetAllKey(flowerinfo);
	for(auto i = flowerinfo.begin(); i != flowerinfo.end(); i++)
	{
		delete *i;
	}

	m_rankTree.Clear();
}

UINT32 FlowerRankList::GetNeedRankCount()
{
	if (CRankListMgr::Instance()->DebugGetFlowerRankNum())
	{
		return CRankListMgr::Instance()->DebugGetFlowerRankNum();
	}
	return FLOWERRANKCOUNT;
}

UINT32 FlowerRankList::GetRank(UINT64 roleid)
{
	return m_rankTree.GetRankByID(roleid);
}

UINT32 FlowerRankList::GetSize()
{
	return m_rankTree.GetSize();
}

const FlowerRankInfo* FlowerRankList::GetInfoByRank(UINT32 rank)
{
	FlowerRankInfo* rankinfo;
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

void FlowerRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	std::vector<FlowerRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);

	UINT32 vecSize = vec.size();
	for (UINT32 i = 0; i < vecSize; i++)
	{
		FlowerRankInfo* rankinfo = vec[i];
		KKSG::RankData* rankdata = rankdatas->add_rankdata();
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void FlowerRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	const FlowerRankInfo* rankinfo = GetInfoByRank(rank);
	if(rankinfo != NULL)
	{
		rankinfo->RankInfoToKKSG(rankdata);
	}	
}

void FlowerRankList::NewRankInfoToClient(KKSG::RankData* rankdata, UINT64 uniqueID)
{
	SocialInfo* social = SocialMgr::Instance()->GetSocialInfo(uniqueID);
	if (social)
	{
		Flower& flower = social->flower;
		FlowerRankInfo rankinfo(uniqueID);
		switch (GetType())
		{
		case KKSG::FlowerRank:
			rankinfo.m_flowercount = flower.GetTodayReceiveCount();
			rankinfo.m_mapFlowers = flower.m_mapReceiveFlowersToday;
			break;
		case KKSG::FlowerYesterdayRank:
			rankinfo.m_flowercount = flower.GetYestodayReceiveCount();
			rankinfo.m_mapFlowers = flower.m_mapReceiveFlowersYes;
			break;
		case KKSG::FlowerThisWeekRank:
			rankinfo.m_flowercount = flower.GetThisWeekReceiveCount();
			rankinfo.m_mapFlowers = flower.m_mapReceiveFlowersThisWeek;
			break;
		case KKSG::FlowerTotalRank:
			rankinfo.m_flowercount = flower.GetTotalReceiveCount();
			rankinfo.m_mapFlowers = flower.m_mapReceiveFlowersTotal;
			break;
		case KKSG::FlowerActivityRank:
			rankinfo.m_flowercount = flower.GetActivityReceiveCount();
			break;
		default:
			return;
		}
		rankinfo.m_time = time(0);
		rankinfo.RankInfoToKKSG(rankdata);
	}
}

void FlowerRankList::OnChange(UINT64 roleid, UINT32 flowercount, std::map<UINT32, UINT32>& flowers, UINT32 time, bool isAddPunished)
{
	if(!isAddPunished)
	{
		if(CPunishMgr::Instance()->IsPunish(roleid, KKSG::PUNISH_USER_FLOWER_RANK))
		{
			return;
		}
	}
	FlowerRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(roleid, rankinfo);
	if(ret == INVALID_RANK)
	{
		rankinfo = new FlowerRankInfo(roleid);
		rankinfo->m_flowercount = flowercount;
		rankinfo->m_time = time;
		rankinfo->m_mapFlowers = flowers;
		m_rankTree.Insert(rankinfo, roleid);

		if(m_rankTree.GetSize() > GetNeedRankCount())
		{
			UINT32 ret = m_rankTree.GetKeyByRank(m_rankTree.GetSize(), rankinfo);
			if(ret != INVALID_RANK)
			{
				m_rankTree.Erase(rankinfo->GetUniqueId());
				delete rankinfo;
			}
		}
	}
	else
	{
		m_rankTree.Erase(roleid);
		rankinfo->m_flowercount = flowercount;
		rankinfo->m_time = time;
		rankinfo->m_mapFlowers = flowers;
		m_rankTree.Insert(rankinfo, roleid);
	}
}

bool FlowerRankList::OnTimer()
{
	/*time_t nowtime = time(NULL);
	tm* time = localtime(&nowtime);
	if (1 == time->tm_wday)
	{
		Reset();
	}*/
	if (GetType() == KKSG::FlowerRank)
	{
		Clear();
	}
	if (GetType() == KKSG::FlowerThisWeekRank)
	{
		int now = time(0);
		int lastDay = now - XCommon::GetOneDayTime();
		if (!XCommon::IsWeekSame(lastDay, now))
		{
			SendThisWeekRankReward();
			Clear();
		}
	}
	return true;
}

void FlowerRankList::GetSomeTopInfo(int count, std::vector<FlowerRankInfo*>& vecInfo)
{
	m_rankTree.GetSomeTopKey(count, vecInfo);
}

void FlowerRankList::SendTShowTop3Rank()
{
	if (GetType() != KKSG::FlowerTotalRank)
	{
		return;
	}
	//CTShowRecord* record = role->Get<CTShowRecord>();
	//if (record && !record->HaveSendRank())
	std::vector<FlowerRankInfo*> vec;
	m_rankTree.GetSomeTopKey(3, vec);

	UINT32 vecSize = vec.size();
	for (UINT32 i = 0; i < vecSize; i++)
	{
		const FlowerRankInfo* info = vec[i];
		CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(info->GetUniqueId());
		if (!pRoleSummary)
		{
			continue;
		}

		//NoticeMgr::Instance()->TShowTop3Notice(pRoleSummary, i);
			/*PtcG2C_TShowVoteRankNtf ntf;
			ntf.m_Data.set_rank(rank);
			role->Send(ntf);*/
		//record->SetSendRank(true);
	}
}


void FlowerRankList::SendTShowTop3Rank(CRoleSummary* role)
{
	if (GetType() != KKSG::FlowerTotalRank || !role)
	{
		return;
	}
}

void FlowerRankList::SendTShowWalkRank(CRoleSummary* role)
{
	if (GetType() != KKSG::FlowerYesterdayRank || !role)
	{
		return;
	}
}

void FlowerRankList::DeleteRole(UINT64 roleid)
{
	FlowerRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(roleid, rankinfo);
	if(ret != INVALID_RANK)
	{
		m_rankTree.Erase(roleid);
		delete rankinfo;
	}
}

void FlowerRankList::InsertPunishedRankInfo(std::string info)
{
	if(GetType() != KKSG::FlowerRank)
	{
		return;
	}
	KKSG::RankData rankdata;
	if(!rankdata.ParseFromString(info))
	{
		LogError("flowerrankinfo deserialize failed");
		return;
	}
	std::map<UINT32, UINT32> flowers;
	for(int i = 0; i < rankdata.receiveflowers_size(); i++)
	{
		const KKSG::MapIntItem& item = rankdata.receiveflowers(i);
		flowers[(UINT32)item.key()] = item.value();
	}
	OnChange(rankdata.roleid(), rankdata.flowercount(), flowers, rankdata.time(), true);
}

std::string FlowerRankList::GetPunishedRankInfo(UINT64 roleid)
{
	FlowerRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(roleid, rankinfo);
	if(ret == INVALID_RANK)
	{
		return "";
	}
	KKSG::RankData rankdata;
	rankdata.set_roleid(rankinfo->GetUniqueId());
	rankdata.set_flowercount(rankinfo->m_flowercount);
	rankdata.set_time(rankinfo->m_time);
	for (auto iter = rankinfo->m_mapFlowers.begin(); iter != rankinfo->m_mapFlowers.end(); ++iter)
	{
		KKSG::MapIntItem* item = rankdata.add_receiveflowers();
		item->set_key(iter->first);
		item->set_value(iter->second);
	}
	return rankdata.SerializeAsString();
}

void FlowerRankList::SendThisWeekRankReward()
{
	std::vector<FlowerRankInfo*> infos;
	GetSomeTopInfo(10, infos);
	int rank = 1;
	for (auto iter = infos.begin(); iter != infos.end(); ++iter, ++rank)
	{
		auto data = FlowerConfig::Instance()->GetFlowerWeekRewardData(rank);
		if (!data)
		{
			continue;
		}

		std::vector<ItemDesc> items;
		for (auto i = data->Reward.begin(); i != data->Reward.end(); ++i)
		{
			ItemDesc item(i->seq[0], i->seq[1]);
			items.push_back(item);
		}
		if (items.size())
		{
			MailData mailData( GetGlobalConfig().FlowerWeekRank_MailTitle, GetGlobalConfig().FlowerWeekRank_MailContent, items, MAIL_EXPIRE_TIME);
			mailData.SetReason(ItemFlow_FlowerWeekRankAward);
			CMailMgr::Instance()->SendMail((*iter)->GetUniqueId(), mailData);
		}
		SocialMgr::Instance()->DoFlowerRank((*iter)->GetUniqueId(), KKSG::FlowerThisWeekRank, rank);
	}
}

void FlowerRankList::SendValentinesDayReward()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_VALENTINESDAY))
	{
		return;
	}
	std::vector<FlowerRankInfo*> infos;
	GetSomeTopInfo(10, infos);
	int rank = 1;
	for (auto iter = infos.begin(); iter != infos.end(); ++iter, ++rank)
	{
		auto data = FlowerConfig::Instance()->GetFlowerRewardData(rank);
		if (!data)
		{
			continue;
		}

		std::vector<ItemDesc> items;
		for (auto i = data->valentinesdayreward.begin(); i != data->valentinesdayreward.end(); ++i)
		{
			ItemDesc item(i->seq[0], i->seq[1]);
			items.push_back(item);
		}
		if (items.size())
		{
			MailData mailData( GetGlobalConfig().FlowerValentinesDayRank_MailTitle, GetGlobalConfig().FlowerValentinesDayRank_MailContent, items, MAIL_EXPIRE_TIME);
			mailData.SetReason(ItemFlow_FlowerValentinesDayRankAward);
			CMailMgr::Instance()->SendMail((*iter)->GetUniqueId(), mailData);
		}

		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary((*iter)->GetUniqueId());
		if (summary)
		{
			if (rank == 1)
			{
				NoticeMgr::Instance()->FlowerValentinesFirstNotice(summary);
			}
			else
			{
				NoticeMgr::Instance()->FlowerValentinesOtherNotice(summary, rank);
			}
		}

		LogInfo("give valentinesday reward, %llu.", (*iter)->GetUniqueId());
	}
}

void FlowerRankList::Send520DayReward()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_520DAY))
	{
		return;
	}
	std::vector<FlowerRankInfo*> infos;
	GetSomeTopInfo(10, infos);
	int rank = 1;
	for (auto iter = infos.begin(); iter != infos.end(); ++iter, ++rank)
	{
		auto data = FlowerConfig::Instance()->GetFlowerRewardData(rank);
		if (!data)
		{
			continue;
		}

		std::vector<ItemDesc> items;
		for (auto i = data->_520reward.begin(); i != data->_520reward.end(); ++i)
		{
			ItemDesc item(i->seq[0], i->seq[1]);
			items.push_back(item);
		}
		if (items.size())
		{
			MailData mailData( GetGlobalConfig().Flower520DayRank_MailTitle , GetGlobalConfig().Flower520DayRank_MailContent , items, MAIL_EXPIRE_TIME);
			mailData.SetReason(ItemFlow_Flower520DayRankAward);
			CMailMgr::Instance()->SendMail((*iter)->GetUniqueId(), mailData);
		}

		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary((*iter)->GetUniqueId());
		if (summary)
		{
			if (rank == 1)
			{
				NoticeMgr::Instance()->Flower520FirstNotice(summary);
			}
			else
			{
				NoticeMgr::Instance()->Flower520OtherNotice(summary, rank);
			}
		}

		LogInfo("give 520 reward, %llu, rank %u.", (*iter)->GetUniqueId(), rank);
	}
}

void FlowerRankList::SendActiviyDayReward()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SysFlowerActiviyId))
	{
		return;
	}
	std::vector<FlowerRankInfo*> infos;
	GetSomeTopInfo(10, infos);
	int rank = 1;
	for (auto iter = infos.begin(); iter != infos.end(); ++iter, ++rank)
	{
		auto data = FlowerConfig::Instance()->GetFlowerRewardData(rank);
		if (!data)
		{
			continue;
		}

		std::vector<ItemDesc> items;
		for (auto i = data->activity.begin(); i != data->activity.end(); ++i)
		{
			ItemDesc item(i->seq[0], i->seq[1]);
			items.push_back(item);
		}
		if (items.size())
		{
			MailData mailData( GetGlobalConfig().FlowerActivityDayRank_MailTitle , GetGlobalConfig().FlowerActivityDayRank_MailContent , items, MAIL_EXPIRE_TIME);
			mailData.SetReason(ItemFlow_FlowerActivityDayRankAward);
			CMailMgr::Instance()->SendMail((*iter)->GetUniqueId(), mailData);
		}

		LogInfo("give flower activity reward, %llu, rank %u.", (*iter)->GetUniqueId(), rank);
	}
}
