#ifndef __GUILDAUCTCONFIG_H__
#define __GUILDAUCTCONFIG_H__

#include "table/GuildAuctReward.h"
#include "table/GuildAuctItem.h"
#include "define/itemdef.h"

struct GAActConf
{
	GAActConf()
	{
		profitType = 0;
		flow = 0;
		rankCount = 0;
	}
	std::string actName;
	int profitType;
	int flow;
	int rankCount;
};

class GuildAuctConfig
{
	GuildAuctConfig();
	~GuildAuctConfig();
	DECLARE_SINGLETON(GuildAuctConfig)

public:

	bool Init();
	bool Reload();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	GuildAuctReward::RowData* GetItems(int actType, int rank, std::vector<ItemDesc>& items, UINT32 roleCount);

	GuildAuctItem::RowData* GetItemConf(UINT32 itemID);

	int GetProfitType(int actType);
	int GetItemType(UINT32 itemID);
	std::string GetActName(int actType);
	bool IsActFlow(int actType);
	int GetRankCount(int actType);
	void GetSItemsByActType();
	const std::map<UINT32,std::unordered_set<UINT32>>& GetCertainItems() 
	{
		return certainly_items_;
	}
private:
	float GetSItemPo(const UINT32 role_count,const UINT32 item_id,vector<Sequence<float, 3>>& sitem_pro);
private:

	std::unordered_map<int, GAActConf> m_actConf;

	GuildAuctReward m_GAReward;
	GuildAuctItem m_GAItem;

	std::map<UINT32,std::unordered_set<UINT32>>  certainly_items_;
};

#endif