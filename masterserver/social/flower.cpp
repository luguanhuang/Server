#include "pch.h"
#include "flower.h"
#include "time.h"
#include "util/XCommon.h"
#include "pb/project.pb.h"
#include "foreach.h"
#include "social/socialmgr.h"
#include "table/globalconfig.h"
#include "mail/maildata.h"
#include "ranklist/ranklistmgr.h"
#include "mail/mailmgr.h"
#include "define/eventdef.h"
#include "event/eventmgr.h"
#include "loghelper/tlogr.h"
#include "idutil.h"
#include "spactivity/spactivitymgr.h"
#include "define/spactivityiddef.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/flowerranklist.h"
#include "util/gametime.h"

void Flower::Update()
{
	/*time_t nowweek = time(NULL);
	if (!XCommon::IsWeekSame(time_t(m_week), nowweek))
	{
		m_weekcount = 0;
		m_week = (UINT32)nowweek;
		m_ischanged = true;
	}*/

	time_t now = GameTime::GetTime();
	if (!XCommon::IsDaySame(time_t(m_day), now))
	{
		if (!XCommon::IsWeekSame(m_day, now))
		{
			m_mapReceiveFlowersThisWeek.clear();
		}
		m_day = now;
		m_mapReceiveFlowersYes.clear();
		m_mapReceiveFlowersYes = m_mapReceiveFlowersToday;
		foreach(i in m_mapReceiveFlowersToday)
		{
			m_mapReceiveFlowersTotal[i->first] += i->second;
		}
		m_mapReceiveFlowersToday.clear();
		m_ischanged = true;
		m_updatetimeYes = m_updatetime;

		SocialMgr::Instance()->SetChanged(m_roleID);
	}
}

UINT32 Flower::GetReceiveCount(const std::map<UINT32, UINT32>& receiveCount)
{
	UINT32 charm = 0;
	foreach(i in receiveCount)
	{
		foreach(j in GetGlobalConfig().Flower2Charm)
		{
			if (i->first == j->seq[0])
			{
				charm += i->second * j->seq[1];
				break;
			}
		}
	
	}
	return charm;
}

UINT32 Flower::GetTodayReceiveCount()
{
	return GetReceiveCount(m_mapReceiveFlowersToday);
}

UINT32 Flower::GetYestodayReceiveCount()
{
	return GetReceiveCount(m_mapReceiveFlowersYes);
}

UINT32 Flower::GetThisWeekReceiveCount()
{
	return GetReceiveCount(m_mapReceiveFlowersThisWeek);
}

UINT32 Flower::GetActivityReceiveCount()
{
	return GetReceiveCount(m_activityFlowerData.mapReceiveFlowersActivity);
}

UINT32 Flower::GetTotalReceiveCount2Client(std::map<UINT32, UINT32>& mapReceiveFlowers2Client)
{
	mapReceiveFlowers2Client.clear();
	foreach (i in m_mapReceiveFlowersToday)
	{
		mapReceiveFlowers2Client[i->first] += i->second;
	}

	foreach (i in m_mapReceiveFlowersTotal)
	{
		mapReceiveFlowers2Client[i->first] += i->second;
	}

	return 0;
}

void Flower::DebugDayPass()
{
	m_mapReceiveFlowersYes.clear();
	m_mapReceiveFlowersYes = m_mapReceiveFlowersToday;
	foreach(i in m_mapReceiveFlowersToday)
	{
		m_mapReceiveFlowersTotal[i->first] += i->second;
	}
	m_mapReceiveFlowersToday.clear();
	m_ischanged = true;
	m_updatetimeYes = m_updatetime;

	SocialMgr::Instance()->SetChanged(m_roleID);

	OnDayPass();
}

void Flower::_CheckAddActivityFlower(UINT64 send, UINT64 besend, UINT32 itemID, UINT32 count)
{
	const OneActivityData* data = SpActivityMgr::Instance()->GetActivityData(SpActivity_FlowerId);
	if (!data)
	{
		return;
	}

	//int now = time(0);
	int now = GameTime::GetTime();

	if (now >= data->m_endTime || now < data->m_startTime)
	{
		return;
	}
	if (data->m_stage != 1)
	{
		return;
	}
	if (data->m_endTime != m_activityFlowerData.activityEndTime)
	{
		m_activityFlowerData.Clear();
	}

	m_activityFlowerData.activityID = SpActivity_FlowerId;
	m_activityFlowerData.activityEndTime = data->m_endTime;
	m_activityFlowerData.updateTime = now;
	m_activityFlowerData.mapReceiveFlowersActivity[itemID] += count;

	SocialMgr::Instance()->SetChanged(besend);

	CRankListMgr::Instance()->OnChangeFlowerActivityCount(besend, GetActivityReceiveCount(), m_activityFlowerData.mapReceiveFlowersActivity, m_activityFlowerData.updateTime);

	LogInfo("%llu,receive %u,%u in activiy", besend, itemID, count);
}

void Flower::DebugWeekPass()
{
	m_mapReceiveFlowersToday.clear();
	m_mapReceiveFlowersThisWeek.clear();
	m_activityFlowerData.Clear();
	SocialMgr::Instance()->SetChanged(m_roleID);
	m_ischanged = true;
}

UINT32 Flower::GetTotalReceiveCount()
{
	return GetReceiveCount(m_mapReceiveFlowersTotal);
}

void Flower::AddFlower(UINT64 send, UINT64 besend, UINT32 itemID, UINT32 count)
{
	Update();

	UINT32 uplevel = 0;

	//PushRecord(send, itemID, count, uplevel, false);
	RecordRoleCount(send, itemID, count);

	m_updatetime = GameTime::GetTime();

	m_mapReceiveFlowersToday[itemID] += count;
	m_mapReceiveFlowersThisWeek[itemID] += count;

	CRankListMgr::Instance()->OnChangeFlowerCount(besend, GetTodayReceiveCount(), m_mapReceiveFlowersToday, m_updatetime);

	CRankListMgr::Instance()->OnChangeFlowerThisWeekCount(besend, GetThisWeekReceiveCount(), m_mapReceiveFlowersThisWeek, m_updatetime);

	_CheckAddActivityFlower(send, besend, itemID, count);

	const FlowerReturnTable::RowData* data = FlowerConfig::Instance()->GetFlowerReturnData(itemID, count);
	if (data)
	{
		std::vector<ItemDesc> items;
		for (auto i = data->returnReward.begin(); i != data->returnReward.end(); ++i)
		{
			ItemDesc item(i->seq[0], i->seq[1]);
			items.push_back(item);
		}
		if (items.size())
		{
			MailData mailData( GetGlobalConfig().SendFlower_MailTitle, GetGlobalConfig().SendFlower_MailContent, items, MAIL_EXPIRE_TIME);
			mailData.SetReason(ItemFlow_Flower_Send);
			CMailMgr::Instance()->SendMail(m_roleID, mailData);
		}
	}
	DoTxLog(besend);

	m_ischanged = true;
}

void Flower::AddSendFlower(UINT64 send, UINT64 besend, UINT32 itemID, UINT32 count)
{
	Update();

	m_mapSendFlowersTotal[itemID] += count;

	PushRecord(besend, itemID, count, 0, true);

	m_ischanged = true;

	if (itemID == FLOWER_RED_ITEMID)
	{
		EventMgr::Instance()->AddEvent(send, ACH_EVENT_OFFSET + ACHIEVE_COM_TYPE_GIVE_FLOWER, m_mapSendFlowersTotal[itemID]);
	}
}

void Flower::PushRecord(UINT64 send, UINT32 itemID, UINT32 count, UINT32 uplevel, bool sendout)
{
	FlowerSendRecord record;
	record.roleid = send;
	record.count = count;
	record.updegreelevel = uplevel;
	record.timestamp = GameTime::GetTime();
	record.itemID = itemID;

	if(sendout)
	{
		m_sendrecords.push_back(record);
	}
	if (m_sendrecords.size() > FLOWERRECORDLIMIT)
	{
		m_sendrecords.pop_front();
	}
}

void Flower::RecordRoleCount(UINT64 send, UINT32 itemid, UINT32 count)
{
	bool isin = false;
	for (auto i = m_rolecount.begin(); i != m_rolecount.end(); ++i)
	{
		if (i->roleid == send)
		{
			i->flowersCount[itemid] += count;
			isin = true;
		}
	}
	if (!isin)
	{
		RoleCountRecord record;
		record.roleid = send;
		record.flowersCount[itemid] = count;
		m_rolecount.push_back(record);

		if (m_rolecount.size() > FLOWERROLETEMPLIMIT)
		{
			Sort(true);
			m_rolecount.pop_back();
			LogInfo("%llu, receive from too much people, size:%u.", m_roleID, m_rolecount.size());
		}
	}

}

bool Flower::IsLess(const RoleCountRecord& a, const RoleCountRecord& b)
{
	return GetReceiveCount(a.flowersCount) > GetReceiveCount(b.flowersCount);
	//return a.GetConvertNum() > b.GetConvertNum();
}

void Flower::Sort(bool isall)
{
	if (isall)
	{
		std::sort(m_rolecount.begin(), m_rolecount.end(), IsLess);
	}
	else
	{
		UINT32 count = (m_rolecount.size() >= FLOWERROLELIMIT) ? FLOWERROLELIMIT : m_rolecount.size();
		std::partial_sort(m_rolecount.begin(), m_rolecount.begin() + count, m_rolecount.end(), IsLess);
	}
}

void Flower::LoadFromDb(const KKSG::SocialInfo2Db* socialinfo2db)
{
	if (socialinfo2db->has_flower() && !(socialinfo2db->flower().empty()))
	{
		KKSG::Flower2Db flower2db;
		flower2db.ParseFromString(socialinfo2db->flower());
		m_allcount = flower2db.allcount();
		m_weekcount = flower2db.weekcount();
		//m_week = flower2db.week();
		m_day = flower2db.week();
		m_updatetime = flower2db.updatetime();
		if (flower2db.has_updatetimeyestoday())
		{
			m_updatetimeYes = flower2db.updatetimeyestoday();
		}
		for (int i = 0; i < flower2db.receiveflowerstoday_size(); ++i)
		{
			m_mapReceiveFlowersToday[(UINT32)flower2db.receiveflowerstoday(i).key()] = flower2db.receiveflowerstoday(i).value();
		}
		for (int i = 0; i < flower2db.receiveflowersthisweek_size(); ++i)
		{
			m_mapReceiveFlowersThisWeek[(UINT32)flower2db.receiveflowersthisweek(i).key()] = flower2db.receiveflowersthisweek(i).value();
		}
		for (int i = 0; i < flower2db.receiveflowersyestoday_size(); ++i)
		{
			m_mapReceiveFlowersYes[(UINT32)flower2db.receiveflowersyestoday(i).key()] = flower2db.receiveflowersyestoday(i).value();
		}
		for (int i = 0; i < flower2db.receiveflowerstotal_size(); ++i)
		{
			m_mapReceiveFlowersTotal[(UINT32)flower2db.receiveflowerstotal(i).key()] = flower2db.receiveflowerstotal(i).value();
		}
		for (int i = 0; i < flower2db.sendinfo_size(); ++i)
		{
			FlowerSendRecord record;
			record.roleid = flower2db.sendinfo(i).roleid();
			record.count = flower2db.sendinfo(i).count();
			record.timestamp = flower2db.sendinfo(i).timestamp();
			record.itemID = flower2db.sendinfo(i).itemid();
			m_sendrecords.push_back(record);
		}
		for (int i = 0; i < flower2db.sendflowerstotal_size(); ++i)
		{
			const KKSG::MapIntItem& item = flower2db.sendflowerstotal(i);
			m_mapSendFlowersTotal[item.key()] = item.value();
		}
		for (int i = 0; i < flower2db.receiveflowerroledetail_size(); ++i)
		{
			RoleCountRecord record;
			record.roleid = flower2db.receiveflowerroledetail(i).roleid();
			auto ref = flower2db.receiveflowerroledetail(i).flowers();
			for (int j = 0; j < ref.size(); ++j)
			{
				record.flowersCount[ref.Get(j).key()] = ref.Get(j).value();
			}
			m_rolecount.push_back(record);
		}
		if (m_rolecount.size() > FLOWERROLELIMIT)
		{
			Sort(true);
			while (m_rolecount.size() > FLOWERROLELIMIT)
			{
				m_rolecount.pop_back();
			}
		}
		if (flower2db.has_activityflowerdata())
		{
			m_activityFlowerData.gotReward = flower2db.activityflowerdata().gotreward();
			m_activityFlowerData.activityID = flower2db.activityflowerdata().activityid();
			m_activityFlowerData.activityEndTime = flower2db.activityflowerdata().activityendtime();
			m_activityFlowerData.updateTime = flower2db.activityflowerdata().updatetime();

			for (int i = 0; i < flower2db.activityflowerdata().activityflower_size(); ++i)
			{
				m_activityFlowerData.mapReceiveFlowersActivity[(UINT32)flower2db.activityflowerdata().activityflower(i).key()] = flower2db.activityflowerdata().activityflower(i).value();
			}
		}
		Update();

		OnLoadFromDb();
	}

}

void Flower::SaveToDb(KKSG::SocialInfo2Db* info)
{
	if (m_ischanged)
	{
		KKSG::Flower2Db flower2db;
		flower2db.set_allcount(m_allcount);
		flower2db.set_weekcount(m_weekcount);
		flower2db.set_week(m_day);
		flower2db.set_updatetime(m_updatetime);
		flower2db.set_updatetimeyestoday(m_updatetimeYes);
		Sort(true);
		foreach(i in m_mapReceiveFlowersToday)
		{
			KKSG::MapIntItem* item = flower2db.add_receiveflowerstoday();
			item->set_key(i->first);
			item->set_value(i->second);
		}
		foreach(i in m_mapReceiveFlowersThisWeek)
		{
			KKSG::MapIntItem* item = flower2db.add_receiveflowersthisweek();
			item->set_key(i->first);
			item->set_value(i->second);
		}
		foreach(i in m_mapReceiveFlowersYes)
		{
			KKSG::MapIntItem* item = flower2db.add_receiveflowersyestoday();
			item->set_key(i->first);
			item->set_value(i->second);
		}
		foreach(i in m_mapReceiveFlowersTotal)
		{
			KKSG::MapIntItem* item = flower2db.add_receiveflowerstotal();
			item->set_key(i->first);
			item->set_value(i->second);
		}
		for (int i = 0; i < m_sendrecords.size(); ++i)
		{
			KKSG::FlowerInfo* info = flower2db.add_sendinfo();
			info->set_roleid(m_sendrecords[i].roleid);
			info->set_count(m_sendrecords[i].count);
			info->set_timestamp(m_sendrecords[i].timestamp);
			info->set_itemid(m_sendrecords[i].itemID);
		}
		for (auto i = m_mapSendFlowersTotal.begin(); i != m_mapSendFlowersTotal.end(); ++i)
		{
			KKSG::MapIntItem* item = flower2db.add_sendflowerstotal();
			item->set_key(i->first);
			item->set_value(i->second);
		}
		UINT32 count = 0;
		for (int i = 0; i < m_rolecount.size(); ++i)
		{
			KKSG::ReceiveRoleFlowerInfo* detail = flower2db.add_receiveflowerroledetail();
			detail->set_roleid(m_rolecount[i].roleid);
			for (auto j = m_rolecount[i].flowersCount.begin(); j != m_rolecount[i].flowersCount.end(); ++j)
			{
				KKSG::MapIntItem* item = detail->add_flowers();
				item->set_key(j->first);
				item->set_value(j->second);
			}
			if (++count >= FLOWERROLELIMIT)
			{
				break;
			}
		}

		KKSG::FlowerActivityData* flowerActivity = flower2db.mutable_activityflowerdata();
		flowerActivity->set_gotreward(m_activityFlowerData.gotReward);
		flowerActivity->set_activityid(m_activityFlowerData.activityID);
		flowerActivity->set_activityendtime(m_activityFlowerData.activityEndTime);
		flowerActivity->set_updatetime(m_activityFlowerData.updateTime);
		for (auto i = m_activityFlowerData.mapReceiveFlowersActivity.begin(); i != m_activityFlowerData.mapReceiveFlowersActivity.end(); ++i)
		{
			KKSG::MapIntItem* item = flowerActivity->add_activityflower();
			item->set_key(i->first);
			item->set_value(i->second);
		}

		info->set_flower(flower2db.SerializeAsString());

		m_ischanged = false;
	}

}

void Flower::OnLoadFromDb()
{
	if (0 != GetTodayReceiveCount())
	{
		CRankListMgr::Instance()->OnChangeFlowerCount(m_roleID, GetTodayReceiveCount(), m_mapReceiveFlowersToday, m_updatetime);
	}
	if (GetThisWeekReceiveCount())
	{
		CRankListMgr::Instance()->OnChangeFlowerThisWeekCount(m_roleID, GetThisWeekReceiveCount(), m_mapReceiveFlowersThisWeek, m_updatetime);
	}
	if (GetTotalReceiveCount())
	{
		CRankListMgr::Instance()->OnChangeFlowerTotalCount(m_roleID, GetTotalReceiveCount(), m_mapReceiveFlowersTotal, m_updatetimeYes);
	}
	if (GetYestodayReceiveCount())
	{
		CRankListMgr::Instance()->OnChangeFlowerYestodayCount(m_roleID, GetYestodayReceiveCount(), m_mapReceiveFlowersYes, m_updatetimeYes);
	}

	if (GetActivityReceiveCount())
	{
		CRankListMgr::Instance()->OnChangeFlowerActivityCount(m_roleID, GetActivityReceiveCount(), m_activityFlowerData.mapReceiveFlowersActivity, m_activityFlowerData.updateTime);
	}
}

void Flower::OnDayPass()
{
	Update();

	int charm = GetTotalReceiveCount();
	if (charm)
	{
		CRankListMgr::Instance()->OnChangeFlowerTotalCount(m_roleID, charm, m_mapReceiveFlowersTotal, m_updatetimeYes);
	}

	charm = GetYestodayReceiveCount();
	if (charm)
	{
		CRankListMgr::Instance()->OnChangeFlowerYestodayCount(m_roleID, charm, m_mapReceiveFlowersYes, m_updatetimeYes);
	}
	DoTxLog(m_roleID);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
INSTANCE_SINGLETON(FlowerConfig);

FlowerConfig::FlowerConfig()
{

}

FlowerConfig::~FlowerConfig()
{

}

bool FlowerConfig::Init()
{
	if (!m_flowertable.LoadFile("table/Flower.txt"))
	{
		SSWarn<<"load file table/Flower.txt failed"<<END;
		return false;
	}
	if (!m_flowerReturnTable.LoadFile("table/FlowerReturn.txt"))
	{
		SSWarn<<"load file table/FlowerReturn.txt failed"<<END;
		return false;
	}
	if (!m_flowerRankTable.LoadFile("table/FlowerRankReward.txt"))
	{
		SSWarn<<"load file table/FlowerRankReward.txt failed"<<END;
		return false;
	}
	if (!m_flowerRainTable.LoadFile("table/FlowerRain.txt"))
	{
		SSWarn<<"load file table/FlowerRain.txt failed"<<END;
		return false;
	}
	if (!m_flowerNotice.LoadFile("table/FlowerNotice.txt"))
	{
		SSWarn<<"load file table/FlowerNotice.txt failed"<<END;
		return false;
	}
	if (!m_flowerWeekRankTable.LoadFile("table/FlowerWeekRankReward.txt"))
	{
		SSWarn<<"load file table/FlowerWeekRankReward.txt failed"<<END;
		return false;
	}
	return true;
}

void FlowerConfig::Uninit()
{

}	


const FlowerRankRewardTable::RowData* FlowerConfig::GetFlowerRewardData(UINT32 rank)
{
	foreach(i in m_flowerRankTable.Table)
	{
		if (rank >= (*i)->rank.seq[0] && rank <= (*i)->rank.seq[1])
		{
			return *i;
		}
	}

	return NULL;
}

const FlowerReturnTable::RowData* FlowerConfig::GetFlowerReturnData(UINT32 itemID, UINT32 itemNum)
{
	foreach(i in m_flowerReturnTable.Table)
	{
		if (itemID == (*i)->receiveFlower.seq[0] && itemNum == (*i)->receiveFlower.seq[1])
		{
			return *i;
		}
	}

	return NULL;
}

bool FlowerConfig::IsFlowerRain(int itemID, int itemCount)
{
	foreach(i in m_flowerRainTable.Table)
	{
		if ((*i)->FlowerID == itemID && itemCount >= (*i)->Count)
		{
			return true;
		}
	}
		
	return false;
}

const FlowerSendNoticeTable::RowData* FlowerConfig::GetNoticeData(UINT32 itemID, UINT32 count)
{
	foreach(i in m_flowerNotice.Table)
	{
		if ((*i)->ItemID == itemID && (*i)->Num == count)
		{
			return (*i);
		}
	}

	return NULL;
}

const FlowerWeekRankReward::RowData* FlowerConfig::GetFlowerWeekRewardData(UINT32 rank)
{
	foreach(i in m_flowerWeekRankTable.Table)
	{
		if (rank >= (*i)->Rank.seq[0] && rank <= (*i)->Rank.seq[1])
		{
			return *i;
		}
	}

	return NULL;
}

void Flower::DoTxLog(UINT64 roleID)
{
	if (GetTotalReceiveCount() || GetTodayReceiveCount() || GetYestodayReceiveCount() || GetThisWeekReceiveCount())
	{
		TFlowerFlow oLog(TLogMgr::Instance()->GetTagString());
		oLog.m_uRoleID = roleID;
		oLog.m_TotalNum = GetTotalReceiveCount();
		oLog.m_TodayNum = GetTodayReceiveCount();
		oLog.m_YesNum = GetYestodayReceiveCount();
		oLog.m_ThisWeekNum = GetThisWeekReceiveCount();
		oLog.Do();
	}
}

bool Flower::CheckGetFlowerActivityReward()
{
	const OneActivityData* data = SpActivityMgr::Instance()->GetActivityData(SpActivity_FlowerId);
	if (!data)
	{
		return false;
	}
	if (m_activityFlowerData.gotReward)
	{
		return false;
	}

	int now = GameTime::GetTime();
	if (now >= data->m_endTime)
	{
		return false;
	}
	if (data->m_stage != 2)
	{
		return false;
	}
	FlowerRankList* list = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerActivityRank);
	if (!list)
	{
		return false;
	}
	int rank = list->GetRank(m_roleID);
	if (rank <= 10)
	{
		return false;
	}
	int value = GetGlobalConfig().GetInt("FlowerActivityAward", 100);
	if (GetActivityReceiveCount() < value)
	{
		return false;
	}
	
	return true;
}

void Flower::CheckUpdateFlowerActivity()
{
	const OneActivityData* data = SpActivityMgr::Instance()->GetActivityData(SpActivity_FlowerId);
	if (!data)
	{
		return;
	}

	if (!m_activityFlowerData.activityEndTime)
	{
		return;
	}

	int now = GameTime::GetTime();

	if (data->m_endTime != m_activityFlowerData.activityEndTime)
	{
		m_activityFlowerData.Clear();
		SocialMgr::Instance()->SetChanged(m_roleID);
	}
	else if (now >= data->m_endTime)
	{
		m_activityFlowerData.Clear();
		SocialMgr::Instance()->SetChanged(m_roleID);
	}
	else if(GetActivityReceiveCount())
	{
		CRankListMgr::Instance()->OnChangeFlowerActivityCount(m_roleID, GetActivityReceiveCount(), m_activityFlowerData.mapReceiveFlowersActivity, m_activityFlowerData.updateTime);
	}
}

void Flower::ClearActivityFlowerInfo()
{
	m_activityFlowerData.Clear();
	SocialMgr::Instance()->SetChanged(m_roleID);
}
