#ifndef __LeagueMatchTable_H__
#define __LeagueMatchTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class LeagueMatchTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		Sequence<uint, 2>	point;
		uint	time;
		uint	span;

		void CopyFrom(RowData *pRowData)
		{
			this->point=pRowData->point;
			this->time=pRowData->time;
			this->span=pRowData->span;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "point", "time", "span",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(LeagueMatchTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.point)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.time)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.span)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~LeagueMatchTable ()
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