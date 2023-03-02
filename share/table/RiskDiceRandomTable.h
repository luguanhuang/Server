#ifndef __RiskDiceRandomTable_H__
#define __RiskDiceRandomTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class RiskDiceRandomTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	RandomValue;
		int	One;
		int	Two;
		int	Three;
		int	Four;
		int	Five;
		int	Six;

		void CopyFrom(RowData *pRowData)
		{
			this->RandomValue=pRowData->RandomValue;
			this->One=pRowData->One;
			this->Two=pRowData->Two;
			this->Three=pRowData->Three;
			this->Four=pRowData->Four;
			this->Five=pRowData->Five;
			this->Six=pRowData->Six;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "RandomValue", "One", "Two", "Three", "Four", "Five", "Six",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(RiskDiceRandomTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.RandomValue)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.One)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Two)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Three)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Four)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Five)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Six)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~RiskDiceRandomTable ()
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