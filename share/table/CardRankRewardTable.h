﻿#ifndef __CardRankRewardTable_H__
#define __CardRankRewardTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/9 21:21:48


class CardRankRewardTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	type;
		uint	rank;
		vector<Sequence<uint, 2>>	reward;
		Sequence<int, 2>	levelseal;

		void CopyFrom(RowData *pRowData)
		{
			this->type=pRowData->type;
			this->rank=pRowData->rank;
			this->reward=pRowData->reward;
			this->levelseal=pRowData->levelseal;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "type", "rank", "reward", "levelseal",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CardRankRewardTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.rank)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.levelseal)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~CardRankRewardTable ()
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