#ifndef __ITEMDROPCONFIG_H__
#define __ITEMDROPCONFIG_H__

#include "table/DropList.h"
#include "define/itemdef.h"
#include "util/XRandom.h"

typedef DropList::RowData  DropConf;

struct DropItem
{
	UINT32 prop;
	UINT32 confprop;
	ItemDesc item;

	DropItem()
	{
		prop = 0;
		confprop = 0;
	}
};

struct DropInfo
{
	UINT32 end;
	std::vector<DropItem> items;

	DropInfo()
	{
		end = 0;
	}
};

struct DropLevelInfo
{
	std::map<UINT32, DropInfo*> m_level2drop;	

	~DropLevelInfo()
	{
		for (auto i = m_level2drop.begin(); i != m_level2drop.end(); ++i)
		{
			delete i->second;
			i->second = NULL;
		}
		m_level2drop.clear();
	}
};

class ItemDropConfig
{
	ItemDropConfig();
	~ItemDropConfig();
	DECLARE_SINGLETON(ItemDropConfig);
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	const ItemDesc* GetRandomDrop(UINT32 dropid, UINT32 level = 0, XRandomType randType = CommonRandom);
	void GetAllDropItem(UINT32 dropid, std::vector<ItemDesc>& vecItems, int* hasNullItem = NULL);

	void Test();

private:
	///> ·¶Î§[Min, Max)
	UINT32 GetRandomInt(UINT32 Min, UINT32 Max, XRandomType randType);

private:
	DropList m_droptable;	
	std::unordered_map<UINT32, DropLevelInfo*> m_id2drop;
};
#endif