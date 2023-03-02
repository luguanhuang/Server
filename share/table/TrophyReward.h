#ifndef __TrophyReward_H__
#define __TrophyReward_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/7/13 11:18:39


class TrophyReward : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	HonourRank;
		int	TrophyScore;
		vector<Sequence<uint, 2>>	Rewards;

		void CopyFrom(RowData *pRowData)
		{
			this->HonourRank=pRowData->HonourRank;
			this->TrophyScore=pRowData->TrophyScore;
			this->Rewards=pRowData->Rewards;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "HonourRank", "TrophyScore", "Rewards",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(TrophyReward  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.HonourRank)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.TrophyScore)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Rewards)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~TrophyReward ()
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