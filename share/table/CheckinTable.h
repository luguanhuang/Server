#ifndef __CheckinTable_H__
#define __CheckinTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class CheckinTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	attendance;
		uint	reward;
		uint	count;
		vector<Sequence<uint, 2>>	bonus;

		void CopyFrom(RowData *pRowData)
		{
			this->attendance=pRowData->attendance;
			this->reward=pRowData->reward;
			this->count=pRowData->count;
			this->bonus=pRowData->bonus;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "attendance", "reward", "count", "bonus",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CheckinTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.attendance)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.count)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.bonus)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~CheckinTable ()
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