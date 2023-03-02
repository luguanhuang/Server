#ifndef __BACKFLOWBUYCONFIG_H__
#define __BACKFLOWBUYCONFIG_H__

#include "table/BackFlowPlayBuy.h"
#include "define/itemdef.h"

class BackFlowBuyConfig
{
	DECLARE_SINGLETON(BackFlowBuyConfig);
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	bool IsBuyMax(UINT32 index);
	UINT32 GetCanBuyCount(UINT32 index);
	ItemDesc GetCost(UINT32 index);
	std::vector<ItemDesc> GetItems(UINT32 index);

private:
	BackFlowPlayBuy m_table;
};

#endif