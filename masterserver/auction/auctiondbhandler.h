#ifndef __AUCTIONDBHANDLER_H__
#define __AUCTIONDBHANDLER_H__

class AuctionItem;
class ATradeRecord;
class ATRecord;

class AuctDbHandler
{
public:
	void AddSaleItem(AuctionItem* pSaleItem);	
	void UpdateSaleItem(AuctionItem* pSaleItem); 
	void DelSaleItem(UINT64 auctuid);
	bool ReadSaleItemFromDB();

	void AddTradeRecord(ATRecord& record);
	void DelTradeRecord(UINT32 attime);
	bool ReadTradeRecordFromDB(ATradeRecord* pRecord);
};

#endif