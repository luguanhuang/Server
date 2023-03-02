#ifndef __GUILDAUCTMGR_H__
#define __GUILDAUCTMGR_H__

#include "timer.h"
#include "guildauctbase.h"
#include "define/itemdef.h"
class CRole;

class GuildAuctMgr : public ITimer
{
	GuildAuctMgr();
	~GuildAuctMgr();
	DECLARE_SINGLETON(GuildAuctMgr)

public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void AddGuildAuctData(UINT64 guildID, int actType, int rank);
	void AddGuildAuctData(UINT64 guildID, int actType, int rank, std::vector<UINT64>& roles);
	void DelGuildAuctData(UINT64 UID);
	GABaseData* GetGuildAuctData(UINT64 UID);

	void AddGuildAuctDBData(KKSG::GAItemDBData& itemDBData);

	void FillActTypeList(UINT64 guildID, int actType, KKSG::GuildAuctReqRes& roRes, UINT64 roleID);
	void FillItemTypeList(int itemType, KKSG::GuildAuctReqRes& roRes, UINT64 roleID);
	void FillGuildSaleHistory(KKSG::GuildAuctReqRes& roRes, UINT64 guildID);
	void FillWorldSaleHistory(KKSG::GuildAuctReqRes& roRes);
	bool HaveGuildSale(UINT64 guildID);

	void AddRoleProfit(UINT64 roleID, UINT32 act_type,UINT32 price);
	UINT32 GetRoleProfit(UINT64 roleID,UINT32 act_type);

	void OnRoleLogin(CRole* pRole);
	void HintNotify(CRole* pRole);

	//上架到拍卖行(公会和世界)
	void TLogAddItem(GABaseData* data);
	//(竞拍 一口价)(公会和世界)
	void TLogBuy(UINT64 roleID, GABaseData* data);
	//分红 
	void TLogMakeProfit(UINT64 guildID, UINT64 roleID, int actType, UINT32 price);
	//给物品（竞拍 一口价）(公会和世界)
	void TLogGiveItem(UINT64 roleID, GABaseData* data);

	UINT32 GetGuildAuctProfitMaxByActType(UINT32 act_type);	

	void DelayItemTime(UINT64 guildID, int actType);

	void TestPrint();
	void TestClear();

	UINT32 GetItemNum() { return m_allData.size();}
private:
	void CheckSaleState();
	void CheckProfitData();

	void AddUniOne(GABaseData* data);
	void DelUniOne(GABaseData* data);

	void UpdateUID(UINT64 UID);
	UINT64 NewUID();

	// 工会拍卖活动结束后额外按比率投递到世界拍卖行
	void CheckAuctItem(GABaseData* data);
	void AuctionToWroldEx();
	bool CheckItemAttachType(UINT32 act_type,UINT32 item_id);
	std::unordered_map<UINT64, GABaseData*> m_allData;//唯一标准

	std::unordered_map<UINT64, std::unordered_map<UINT32,UINT32>> m_profitData;
	time_t m_lastUpProfitDataTime;

	UINT64 m_UIDIndex;

	HTIMER m_handler;
	std::unordered_map<UINT32,UINT32> type_profit_max_;
	std::unordered_map<UINT32,float> type_profit_rate_;	
		
	std::map<UINT32,std::map<UINT32,ItemDesc>> item_auct_;
	
};

#endif