#ifndef __SHOPMGR_H__
#define __SHOPMGR_H__

#include "table/ShopTable.h"
#include "table/ShopTypeTable.h"
#include "item.h"

struct ShopBenefit
{
	UINT32 type;
	UINT32 percent;
	ShopBenefit()
	{
		type = 0;
		percent = 0;
	}
};

struct ShopPrice
{
	UINT32 count;
	UINT32 price;
	ShopPrice()
	{
		count = 0;
		price = 0;
	}
};

struct ShopConditionParam
{
	UINT32 level;
	UINT32 ppt;
	UINT32 dailycount;
	UINT32 weekcount;
	UINT32 count;
	UINT32 vip;
	UINT32 arena;
	UINT32 pk;
	UINT32 guild;
	ShopConditionParam()
	{
		level = 0;
		ppt = 0;
		dailycount = 0;
		weekcount = 0;
		count = 0;
		vip = 0;
		arena = 0;
		guild = 0;
	}
};

struct ItemDesc;
struct ShopItemBreif
{
	UINT32 uid;
	bool isbind;
	ItemDesc item;
	ItemDesc cost;	
	ShopBenefit benefit;
	std::vector<ShopPrice> price;
	ShopConditionParam condition;

	ShopItemBreif()
	{
		uid = 0;
		isbind = true;
	}
};

struct ShopChoose
{
	UINT32 uid;
	UINT32 percent;
	bool levellimit;
	UINT32 levellow;
	UINT32 levelhigh;
	UINT32 cook_level;
	ShopChoose()
	{
		uid = 0;
		percent = 0;
		levellimit = false;
		levellow = 0;
		levelhigh = 0;
		cook_level = 0;
	}
};

struct ShopRoleParam
{
	int count;
	ShopRoleParam()
	{
		count = 0;
	}
};

enum ShopEnum
{
	Shop_Guild = 4,
	Shop_Garden = 14,
};

struct ShopOneInfo
{
	std::map<UINT32, ShopItemBreif>* GetItems(UINT32 profession)
	{
		auto i = id2shopitem.find(profession);
		if (i == id2shopitem.end())
		{
			auto j = id2shopitem.find(0);
			if (j == id2shopitem.end())
			{
				return NULL;
			}
			else
			{
				return &j->second;
			}
		}
		else
		{
			return &i->second;
		}
	}

	std::map<UINT32, std::vector<ShopChoose>>* GetChooses(UINT32 profession)
	{
		auto i = id2shopchoose.find(profession);
		if (i == id2shopchoose.end())
		{
			auto j = id2shopchoose.find(0);
			if (j == id2shopchoose.end())
			{
				return NULL;
			}
			else
			{
				return &j->second;
			}
		}
		else
		{
			return &i->second;
		}
	}

	std::map<UINT32, std::map<UINT32, ShopItemBreif>> id2shopitem;
	///> 同一个位置，在多个item里面随机一个
	std::map<UINT32, std::map<UINT32, std::vector<ShopChoose>>> id2shopchoose;
};
class Role;
class IShopCondition;
struct XItem;
class ShopMgr
{
	ShopMgr();
	~ShopMgr();
	DECLARE_SINGLETON(ShopMgr);
public:

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();
	
	// open
	bool IsOpen(Role* role, UINT32 type);
	// refresh
	bool IsNeedRefresh(UINT32 type);
	ItemDesc GetRefreshCost(UINT32 type, UINT32 count);
	bool GetRefreshTime(UINT32 type, UINT32& lastday, UINT32& lasttime, UINT32& lefttime);

	UINT32 GetType(UINT32 uid);
	UINT32 GetSystemId(UINT32 type);

	void Hint(Role* role, bool isremove, UINT32 typ);

	const ShopItemBreif* GetBreif(UINT32 type, UINT32 uid, UINT32 profession);
	// limit
	UINT32 IsLimit(Role* role, UINT32 type, UINT32 uid, const ShopItemBreif* brief, const ShopRoleParam& param);
	// cost
	const ItemDesc* GetCost(const ShopItemBreif* brief);
	UINT32 GetCostCount(const ShopItemBreif* brief, UINT32 count);
	// refresh
	std::map<UINT32, XItem> RefreshShop(UINT32 type, UINT32 rolelevel, UINT32 roleprofession);

	std::map<UINT32, ShopOneInfo>& GetShops() { return m_id2oneshop; }

private:
	UINT32 RandomItem(UINT32 rolelevel, std::vector<ShopChoose>& choooses);
	void Test();

private:
	ShopTable m_oTable;
	ShopTypeTable m_oTypeTable;
	std::map<UINT32, UINT32> m_id2type;
	std::map<UINT32, ShopOneInfo> m_id2oneshop;
	std::vector<IShopCondition*> m_shopconditions;
};
#endif