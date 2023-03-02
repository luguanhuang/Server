#ifndef __BigMeleeRankReward_H__
#define __BigMeleeRankReward_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/22 1:01:46


class BigMeleeRankReward : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		Sequence<int, 2>	levelrange;
		Sequence<int, 2>	rank;
		vector<Sequence<int, 2>>	reward;

		void CopyFrom(RowData *pRowData)
		{
			this->levelrange=pRowData->levelrange;
			this->rank=pRowData->rank;
			this->reward=pRowData->reward;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "levelrange", "rank", "reward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(BigMeleeRankReward  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.levelrange)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.rank)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.reward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~BigMeleeRankReward ()
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