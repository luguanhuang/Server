#include "pch.h"
#include "backflowbuyconfig.h"

INSTANCE_SINGLETON(BackFlowBuyConfig);

bool BackFlowBuyConfig::Init()
{
	return LoadFile();
}

void BackFlowBuyConfig::Uninit()
{

}

bool BackFlowBuyConfig::CheckFile()
{
	BackFlowPlayBuy temptable;
	if (!temptable.LoadFile("table/BackFlowPlayBuy.txt"))
	{
		SSWarn<<"load file table/BackFlowPlayBuy.txt failed"<<END;
		return false;
	}
	return true;
}

bool BackFlowBuyConfig::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}
	ClearFile();

	m_table.LoadFile("table/BackFlowPlayBuy.txt");

	return true;
}

void BackFlowBuyConfig::ClearFile()
{
	m_table.Clear();
}

bool BackFlowBuyConfig::IsBuyMax(UINT32 index)
{
	if (NULL == m_table.GetByid(index))
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::vector<ItemDesc> BackFlowBuyConfig::GetItems(UINT32 index)
{
	std::vector<ItemDesc> items;
	BackFlowPlayBuy::RowData* data = m_table.GetByid(index);
	if (NULL != data)
	{
		for (auto i = data->item.begin(); i != data->item.end(); ++i)
		{
			ItemDesc item;
			item.itemID = i->seq[0];
			item.itemCount = i->seq[1];
			items.push_back(item);
		}
	}
	return items;
}

ItemDesc BackFlowBuyConfig::GetCost(UINT32 index)
{
	ItemDesc item;
	BackFlowPlayBuy::RowData* data = m_table.GetByid(index);
	if (NULL != data)
	{
		item.itemID = data->pay.seq[0];
		item.itemCount = data->pay.seq[1];
	}
	return item;
}

UINT32 BackFlowBuyConfig::GetCanBuyCount(UINT32 index)
{
	BackFlowPlayBuy::RowData* data = m_table.GetByid(index);
	if (NULL != data)
	{
		return data->countlimit;
	}
	return 0;
}
