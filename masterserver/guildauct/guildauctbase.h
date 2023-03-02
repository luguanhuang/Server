#ifndef __GUILDAUCTBASE_H__
#define __GUILDAUCTBASE_H__

namespace KKSG
{
	class GAItemDBData;
	class GASaleItem;
	class GASaleHistory;
	class GuildAuctReqRes;
}

const UINT32 GAItemLockTime = 10;//10s

enum GAActType
{
	GA_ACT_GUILD_BOSS = 1,//公会boss
	GA_ACT_GUILD_GMF = 2,//公会擂台战
	GA_ACT_GUILD_RESOURCE = 3,//公会资源争夺
	GA_ACT_WORLD_BOSS = 4,//世界BOSS
	GA_ACT_TERRITORY = 5,	//工会领地
	GA_ACT_TERRITORY_DAY = 6, //工会领地每日
	GA_ACT_TERRITORY_FAILD = 7,//公会领地失败

};

enum GAProfitType
{
	GAPROFIT_TYPE_ALL = 1,
	GAPROFIT_TYPE_JOIN = 2,
};

enum GAuctSaleType
{
	GAST_GUILD_AUCT_ING = 1,//在公会拍卖中
	GAST_GUILD_BUY_NOW_UNPROF = 2,//在公会被一口价卖掉 未分红
	GAST_GUILD_BUY_NOW_PROFIT = 3,//在公会被一口价卖掉 且分红
	GAST_GUILD_BUY_AUCT = 4,//在公会被竞价卖掉
	GAST_WORLD_AUCT_ING = 5,//在世界拍卖中
	GAST_WORLD_BUY_NOW = 6,//在世界被一口价卖掉
	GAST_WORLD_BUY_AUCT = 7,//在世界被竞拍掉
};

typedef std::pair<UINT64, int> GAPAIR_KEY;

class GABaseData
{
public:
	GABaseData(UINT64 UID, UINT32 itemID, UINT32 itemCount, int actType, UINT64 guildID, time_t now, std::vector<UINT64>& joinRoles);
	GABaseData(const KKSG::GAItemDBData& data);
	~GABaseData();

	void InitDefault();

	void SetSaleTypeData(int saleType, time_t now, bool init = false);
	inline int GetSaleType() { return m_DBData.saletype();}

	inline UINT64 GetUID() { return m_DBData.uid(); }
	inline UINT32 GetItemID() { return m_DBData.itemid(); }
	inline UINT32 GetItemCount() { return m_DBData.itemcount(); }

	inline UINT64 GetGuildID() { return m_DBData.guildid(); }
	inline int GetActType() { return m_DBData.acttype(); }
	GAPAIR_KEY GetPairKey(); 

	inline bool HaveAucted() { return (0 != m_DBData.auctroleid()); }
	inline UINT64 GetAuctRoleID() { return m_DBData.auctroleid(); }
	inline UINT32 GetAuctPrice() { return m_DBData.auctprice(); }
	void SetAuctData(UINT64 roleID, UINT32 price); 
	UINT32 GetAuctUpPrice();
	inline UINT32 GetMaxPrice() { return m_DBData.maxprice(); }

	bool IsSaled(); 
	bool IsAuctSaled();
	bool IsWorldSaled();
	inline UINT32 GetSaledTime(){ return m_DBData.saledtime(); }

	void GetJoinRoles(std::unordered_set<UINT64>& joinRoles);
	UINT32 GetJoinRoleCount();

	UINT32 GetSaleLeftTime(UINT32 now);
	bool NeedDel(time_t now);
	UINT32 GetGwTime() { return m_DBData.gwtime(); }

	void AddGwTime(UINT32 add_time) { m_DBData.set_gwtime(m_DBData.gwtime()+add_time);}

	void FillClientOnSaleData(KKSG::GASaleItem& data, UINT64 roleID);
	void FillClientSaleHistoryData(KKSG::GASaleHistory& data, bool guild = false);
	void FillDBData(KKSG::GAItemDBData& data);

	void SetLock();
	bool IsLocked();
	void ClearLock();

	void TestPrint();
private:
	time_t m_lockTime;
	KKSG::GAItemDBData m_DBData;
};

class GAGuildActType
{
public:	
	GAGuildActType(GAPAIR_KEY gakey);
	void AddBaseData(GABaseData* baseData);
	UINT32 GetRoleProfit(UINT64 roleID);
	UINT32 GetProfit();
	void MakeProfit();

private:
	std::unordered_set<UINT64> m_joinRoles;
	double m_allPrice;
	GAPAIR_KEY m_key;
};

class GAGuildActProfit
{
public:
	void AddGABaseData(GABaseData* baseData);
	void MakeProfit();

private:	
	std::map<GAPAIR_KEY, GAGuildActType> m_waitProfit;
};

#endif