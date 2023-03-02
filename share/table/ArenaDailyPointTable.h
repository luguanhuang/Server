#ifndef __ArenaDailyPointTable_H__
#define __ArenaDailyPointTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2015/9/2 17:52:08


class ArenaDailyPointTable : public CVSReader
{
public:

	struct RowData
	{
		uint	Point;
		vector<Sequence<uint, 2>>	Reward;

	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Point", "Reward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.Point)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Reward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~ArenaDailyPointTable ()
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