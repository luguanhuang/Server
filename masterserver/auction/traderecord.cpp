#include "pch.h"
#include "traderecord.h"
#include "util/XCommon.h"
#include "table/globalconfig.h"
#include "auctionmgr.h"
#include "auctiondbhandler.h"
#include "config.h"

ATRecord::ATRecord(UINT64 uid, UINT32 itemid, UINT32 itemcount, UINT32 perprice, time_t attime, UINT64 auctid, const std::string& buyaccount, const std::string& saleaccount)
	:m_uid(uid),m_itemid(itemid),m_itemcount(itemcount),m_perprice(perprice),m_time(attime), m_auctuid(auctid),m_buyeraccount(buyaccount),m_saleraccount(saleaccount)
{
	//m_isinaccountlimit = false;
}

void ATRecord::TestPrint()
{
	SSDebug << " atrecord itemid = " << m_itemid << " itemcount = " << m_itemcount << " auctid = " << m_auctuid << " buyaccount = " << m_buyeraccount << " saleaccount = " << m_saleraccount << " time = " << m_time << END;
}

void ATRecord::FillDBData(KKSG::AuctDBTradeAccount& data)
{
	data.set_recorduid(m_uid);
	data.set_itemid(m_itemid);
	data.set_itemcount(m_itemcount);
	data.set_perprice(m_perprice);
	data.set_attime((UINT32)m_time);
	data.set_auctuid(m_auctuid);
	data.set_buyaccount(m_buyeraccount);
	data.set_saleaccount(m_saleraccount);
}


ATItemRecord::ATItemRecord(UINT32 itemid)
	:m_itemid(itemid)
{
	m_allprice = 0;
	m_allcount = 0;
}

void ATItemRecord::AddRecord(ATRecord& record)
{
	m_record.push_back(record);
	m_allprice += (record.GetPerPrice() * record.GetItemCount());
	m_allcount += record.GetItemCount();
}

void ATItemRecord::CheckDelRecord(time_t now)
{
	UINT32 EffectTime = GetGlobalConfig().AuctTradeRecordTime;
	while(!m_record.empty())
	{
		auto& rc = m_record.front();
		//if(!rc.IsInAccountLimit() && rc.GetTime() + EffectTime < now)
		if(rc.GetTime() + EffectTime < now)
		{
			ATRecord& atr = m_record.front();
			UINT32 tmp = (atr.GetPerPrice() * atr.GetItemCount());
			m_allprice = (m_allprice>=tmp)?(m_allprice-tmp):0;
			tmp = atr.GetItemCount();
			m_allcount = (m_allcount>=tmp)?(m_allcount-tmp):0;

			m_record.pop_front();
		}
		else
		{
			break;
		}
	}
}

void ATItemRecord::TestPrint()
{
	SSDebug << " itemid = " << m_itemid << " avgprice = " << GetAVGPrice() << END;
	for(size_t i = 0; i < m_record.size(); ++i)
	{
		m_record[i].TestPrint();
	}
}

UINT32 ATItemRecord::GetAVGPrice()
{
	if(0 == m_allcount)
	{
		return 0;
	}
	return m_allprice/m_allcount;
}

//这个Update的频率要低
void ATradeRecord::Update()
{
	time_t now = GameTime::GetTime();
	UpdateTradeAmount(now);
	UpdateTradeRecord(now);
}

void ATradeRecord::AddOneRecord(UINT32 itemid, UINT32 itemcount, UINT32 preprice, UINT64 auctuid, const std::string& buyaccount, const std::string& saleaccount)
{
	UINT64 uid = NewUID();
	ATRecord onerecord(uid, itemid, itemcount, preprice, GameTime::GetTime(), auctuid, buyaccount, saleaccount);
	Add(onerecord);

	auto pDbHandler = AuctionMgr::Instance()->GetDBHandler();
	if(pDbHandler)
	{
		pDbHandler->AddTradeRecord(onerecord);
	}
}

UINT32 ATradeRecord::GetItemAVGPrice(UINT32 itemid)
{
	auto it = m_itemrecord.find(itemid);
	if(it != m_itemrecord.end())
	{
		return (it->second).GetAVGPrice();
	}
	return 0;
}

UINT32 ATradeRecord::GetTradeAmount(const std::string& buyaccount, const std::string& saleaccount)
{
	std::string tmp = GetAccountKey(buyaccount, saleaccount);
	auto it = m_tradeamount.find(tmp);
	if(it != m_tradeamount.end())
	{
		return (it->second);
	}
	return 0;
}

void ATradeRecord::UpdateTradeAmount(time_t now)
{
	if(m_lastAmountUpdateTime && XCommon::IsWeekSame(m_lastAmountUpdateTime, now))
	{
		return;
	}

	m_lastAmountUpdateTime = now;

	m_tradeamount.clear();
}

ATradeRecord::ATradeRecord()
{
	m_lastAmountUpdateTime = GameTime::GetTime();
	m_lastRecordUpdateTime = GameTime::GetTime();
	tarde_uid_index_ = 0;
}

void ATradeRecord::AddTradeAmount(const std::string& buyaccount, const std::string& saleaccount, UINT32 amount)
{
	std::string tmp = GetAccountKey(buyaccount, saleaccount);
	m_tradeamount[tmp] += amount;
}

void ATradeRecord::TestPrint()
{
	SSDebug << " lastamountupt = " << m_lastAmountUpdateTime << END;
	for(auto it = m_itemrecord.begin(); it != m_itemrecord.end(); ++it)
	{
		(it->second).TestPrint();
	}
	for(auto it = m_tradeamount.begin(); it != m_tradeamount.end(); ++it)
	{
		SSDebug << " accountkey = " << it->first << " amount = " << it->second << END;
	}
}

std::string ATradeRecord::GetAccountKey(const std::string& buyaccount, const std::string& saleaccount)
{
	return (buyaccount + "_" + saleaccount);
}

bool ATradeRecord::IsTradeLimit(const std::string& buyaccount, const std::string& saleaccount, UINT32 TradeLimit)
{
	return (GetTradeAmount(buyaccount, saleaccount) >= TradeLimit);
}

UINT64 ATradeRecord::NewUID()
{
	++tarde_uid_index_;
	UINT64 servergroupid = (UINT64)MSConfig::Instance()->GetServerID();
	UINT64 UID  = ((servergroupid << 32) | tarde_uid_index_);
	return UID;
}

void ATradeRecord::UpdateTradeUID(UINT64 UID)
{
	if(tarde_uid_index_ < (UID&0x00000000ffffffff))
	{
		tarde_uid_index_ = (UID&0x00000000ffffffff);
	}
}

void ATradeRecord::AddOneDBRecord(UINT64 uid, UINT32 itemid, UINT32 itemcount, UINT32 preprice, time_t attime, UINT64 auctuid, const std::string& buyaccount, const std::string& saleaccount)
{
	ATRecord onerecord(uid, itemid, itemcount, preprice, attime, auctuid, buyaccount, saleaccount);
	Add(onerecord);
	// 更新uid
	UpdateTradeUID(uid);
}

void ATradeRecord::Add(ATRecord& record)
{
	auto it = m_itemrecord.find(record.GetItemID());
	if(it == m_itemrecord.end())
	{
		ATItemRecord itemrecord(record.GetItemID());
		m_itemrecord.insert(std::make_pair(record.GetItemID(), itemrecord));

		it = m_itemrecord.find(record.GetItemID());
	}
	if(it != m_itemrecord.end())
	{
		(it->second).AddRecord(record);
	}

	if(XCommon::IsWeekSame(record.GetTime(), m_lastAmountUpdateTime))
	{
		AddTradeAmount(record.BuyerAccount(), record.SalerAccount(), record.GetPerPrice()*record.GetItemCount());
	}
}

void ATradeRecord::UpdateTradeRecord(time_t now)
{
	for(auto it = m_itemrecord.begin(); it != m_itemrecord.end(); ++it)
	{
		(it->second).CheckDelRecord(now);
	}
	if(!XCommon::IsDaySame(m_lastRecordUpdateTime, now))
	{
		m_lastRecordUpdateTime = now;

		UINT32 historyTime = UINT32(XCommon::GetOneDayTime() * 7);
		if(historyTime <= GetGlobalConfig().AuctTradeRecordTime)
		{
			historyTime = GetGlobalConfig().AuctTradeRecordTime;
		}

		auto pDBHandler = AuctionMgr::Instance()->GetDBHandler();
		if(pDBHandler)
		{
			pDBHandler->DelTradeRecord((UINT32)now - historyTime);
		}
	}
}
