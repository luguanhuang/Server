#ifndef __RiskTriggerBuyTable_H__
#define __RiskTriggerBuyTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class RiskTriggerBuyTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	MapID;
		int	GridType;
		int	Probability;
		vector<Sequence<int, 2>>	CostItem;
		vector<Sequence<int, 2>>	RewardItem;

		void CopyFrom(RowData *pRowData)
		{
			this->MapID=pRowData->MapID;
			this->GridType=pRowData->GridType;
			this->Probability=pRowData->Probability;
			this->CostItem=pRowData->CostItem;
			this->RewardItem=pRowData->RewardItem;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "MapID", "GridType", "Probability", "CostItem", "RewardItem",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(RiskTriggerBuyTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.MapID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.GridType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Probability)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.CostItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.RewardItem)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~RiskTriggerBuyTable ()
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