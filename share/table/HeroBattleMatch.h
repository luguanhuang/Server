#ifndef __HeroBattleMatch_H__
#define __HeroBattleMatch_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class HeroBattleMatch : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		Sequence<uint, 2>	PointBlock;
		uint	ExpandTime;
		uint	SearchTime;

		void CopyFrom(RowData *pRowData)
		{
			this->PointBlock=pRowData->PointBlock;
			this->ExpandTime=pRowData->ExpandTime;
			this->SearchTime=pRowData->SearchTime;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "PointBlock", "ExpandTime", "SearchTime",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(HeroBattleMatch  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.PointBlock)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ExpandTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SearchTime)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~HeroBattleMatch ()
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