#ifndef __ItemBackTable_H__
#define __ItemBackTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/22 10:15:51


class ItemBackTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ID;
		int	SystemID;
		string	SystemName;
		int	CostGold;
		vector<Sequence<int, 2>>	ItemGold;
		int	CostDragonCoin;
		vector<Sequence<int, 2>>	ItemDragonCoin;
		int	count;
		string	Desc;
		Sequence<int, 2>	Level;
		int	FindBackDays;
		bool	IsWeekBack;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->SystemID=pRowData->SystemID;
			this->SystemName=pRowData->SystemName;
			this->CostGold=pRowData->CostGold;
			this->ItemGold=pRowData->ItemGold;
			this->CostDragonCoin=pRowData->CostDragonCoin;
			this->ItemDragonCoin=pRowData->ItemDragonCoin;
			this->count=pRowData->count;
			this->Desc=pRowData->Desc;
			this->Level=pRowData->Level;
			this->FindBackDays=pRowData->FindBackDays;
			this->IsWeekBack=pRowData->IsWeekBack;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "SystemID", "SystemName", "CostGold", "ItemGold", "CostDragonCoin", "ItemDragonCoin", "count", "Desc", "Level", "FindBackDays", "IsWeekBack",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ItemBackTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SystemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SystemName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.CostGold)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.ItemGold)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.CostDragonCoin)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.ItemDragonCoin)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.count)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Desc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.FindBackDays)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.IsWeekBack)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~ItemBackTable ()
	{
		Clear();
	}

	void Clear()
	{
		for (UINT32 i = 0; i < Table.size(); ++i)
		{
			delete Table[i];
		}

		Table.clear();


	}

	std::vector<RowData*> Table;
};

#endif