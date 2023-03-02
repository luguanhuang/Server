#ifndef __PkPrepareTable_H__
#define __PkPrepareTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/16 16:08:51


class PkPrepareTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		Sequence<uint, 2>	point;
		uint	time;
		uint	span;
		uint	percent;
		int	robotlookupid;
		int	type;

		void CopyFrom(RowData *pRowData)
		{
			this->point=pRowData->point;
			this->time=pRowData->time;
			this->span=pRowData->span;
			this->percent=pRowData->percent;
			this->robotlookupid=pRowData->robotlookupid;
			this->type=pRowData->type;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "point", "time", "span", "percent", "robotlookupid", "type",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PkPrepareTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[3]], row.percent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.robotlookupid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.type)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PkPrepareTable ()
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