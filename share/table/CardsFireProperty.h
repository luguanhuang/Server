#ifndef __CardsFireProperty_H__
#define __CardsFireProperty_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class CardsFireProperty : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	GroupId;
		uint	FireCounts;
		vector<Sequence<uint, 2>>	Promote;
		uint	BreakLevel;

		void CopyFrom(RowData *pRowData)
		{
			this->GroupId=pRowData->GroupId;
			this->FireCounts=pRowData->FireCounts;
			this->Promote=pRowData->Promote;
			this->BreakLevel=pRowData->BreakLevel;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "GroupId", "FireCounts", "Promote", "BreakLevel",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CardsFireProperty  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.GroupId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.FireCounts)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Promote)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.BreakLevel)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~CardsFireProperty ()
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