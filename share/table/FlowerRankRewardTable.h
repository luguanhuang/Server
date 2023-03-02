#ifndef __FlowerRankRewardTable_H__
#define __FlowerRankRewardTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/8 10:56:18


class FlowerRankRewardTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		Sequence<int, 2>	rank;
		vector<Sequence<int, 2>>	reward;
		uint	yesterday;
		uint	history;
		vector<Sequence<int, 2>>	valentinesdayreward;
		vector<Sequence<int, 2>>	_520reward;
		vector<Sequence<int, 2>>	activity;

		void CopyFrom(RowData *pRowData)
		{
			this->rank=pRowData->rank;
			this->reward=pRowData->reward;
			this->yesterday=pRowData->yesterday;
			this->history=pRowData->history;
			this->valentinesdayreward=pRowData->valentinesdayreward;
			this->_520reward=pRowData->_520reward;
			this->activity=pRowData->activity;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "rank", "reward", "yesterday", "history", "valentinesdayreward", "520reward", "activity",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FlowerRankRewardTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.rank)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.yesterday)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.history)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.valentinesdayreward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row._520reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.activity)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~FlowerRankRewardTable ()
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