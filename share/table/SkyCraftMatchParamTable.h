#ifndef __SkyCraftMatchParamTable_H__
#define __SkyCraftMatchParamTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/12 20:22:57


class SkyCraftMatchParamTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	min;
		uint	max;
		uint	winK;
		uint	loseK;

		void CopyFrom(RowData *pRowData)
		{
			this->min=pRowData->min;
			this->max=pRowData->max;
			this->winK=pRowData->winK;
			this->loseK=pRowData->loseK;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "min", "max", "winK", "loseK",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SkyCraftMatchParamTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.min)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.max)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.winK)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.loseK)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~SkyCraftMatchParamTable ()
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