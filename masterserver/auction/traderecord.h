#ifndef __TRADERECORD_H__
#define __TRADERECORD_H__

#include "auctionbase.h"

namespace KKSG
{
	class AuctDBTradeAccount;
}

class ATRecord
{
public:
	ATRecord(UINT64 uid, UINT32 itemid, UINT32 itemcount, UINT32 perprice, time_t attime, UINT64 auctid, const std::string& buyaccount, const std::string& saleaccount);

	UINT64 GetUID(){ return m_uid; }
	UINT32 GetItemID(){ return m_itemid; }
	UINT32 GetItemCount() { return m_itemcount; }
	UINT32 GetPerPrice(){ return m_perprice; }
	time_t GetTime(){ return m_time; }

	const std::string& BuyerAccount(){ return m_buyeraccount; }
	const std::string& SalerAccount(){ return m_saleraccount; }

	void FillDBData(KKSG::AuctDBTradeAccount& data);
	//bool IsInAccountLimit(){ return m_isinaccountlimit; }
	//void SetOutAccountLimit(){ m_isinaccountlimit = false; }
	void TestPrint();

private:
	UINT64 m_uid;
	UINT32 m_itemid;
	UINT32 m_itemcount;
	UINT32 m_perprice;
	time_t m_time;
	UINT64 m_auctuid;

	std::string m_buyeraccount;
	std::string m_saleraccount;
	//bool m_isinaccountlimit;
};

class ATItemRecord
{
public:
	ATItemRecord(UINT32 itemid);

	void AddRecord(ATRecord& record);
	void CheckDelRecord(time_t now);

	UINT32 GetAVGPrice();

	std::deque<ATRecord>& GetAllRecord() { return m_record; }

	void TestPrint();

private:	
	//需要保证队列中ATRecord::m_time从小到大
	std::deque<ATRecord> m_record;
	UINT32 m_itemid;
	UINT32 m_allprice;
	UINT32 m_allcount;;
};

class ATradeRecord
{
public:
	ATradeRecord();
	void Update();

	void AddOneDBRecord(UINT64 uid, UINT32 itemid, UINT32 itemcount, UINT32 preprice, time_t attime, UINT64 auctuid, const std::string& buyaccount, const std::string& saleaccount);
	void AddOneRecord(UINT32 itemid, UINT32 itemcount, UINT32 preprice, UINT64 auctuid, const std::string& buyaccount, const std::string& saleaccount);
	UINT32 GetItemAVGPrice(UINT32 itemid);
	
	bool IsTradeLimit(const std::string& buyaccount, const std::string& saleaccount, UINT32 TradeLimit);

	void TestPrint();
	
private:
	void Add(ATRecord& record);

	void AddTradeAmount(const std::string& buyaccount, const std::string& saleaccount, UINT32 amount);
	UINT32 GetTradeAmount(const std::string& buyaccount, const std::string& saleaccount);
	void UpdateTradeAmount(time_t now);
	void UpdateTradeRecord(time_t now);

	std::string GetAccountKey(const std::string& buyaccount, const std::string& saleaccount);

	UINT64 NewUID();

	void UpdateTradeUID(UINT64 UID);

	std::unordered_map<UINT32,ATItemRecord> m_itemrecord;
	//key = (account+_+account), value = (amount of pay)
	std::unordered_map<std::string, UINT32> m_tradeamount;
	time_t m_lastAmountUpdateTime;
	time_t m_lastRecordUpdateTime;

	UINT64  tarde_uid_index_;
};

#endif