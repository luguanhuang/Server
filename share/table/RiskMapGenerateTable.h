#ifndef __RiskMapGenerateTable_H__
#define __RiskMapGenerateTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class RiskMapGenerateTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	MapID;
		string	MapGridNum;
		int	GridType;
		int	Probability;
		int	DropID;
		int	Quantity;

		void CopyFrom(RowData *pRowData)
		{
			this->MapID=pRowData->MapID;
			this->MapGridNum=pRowData->MapGridNum;
			this->GridType=pRowData->GridType;
			this->Probability=pRowData->Probability;
			this->DropID=pRowData->DropID;
			this->Quantity=pRowData->Quantity;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "MapID", "MapGridNum", "GridType", "Probability", "DropID", "Quantity",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(RiskMapGenerateTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.MapGridNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.GridType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Probability)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.DropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Quantity)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~RiskMapGenerateTable ()
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