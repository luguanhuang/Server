#ifndef __WorldBossRewardTable_H__
#define __WorldBossRewardTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class WorldBossRewardTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	Level;
		Sequence<uint, 2>	Rank;
		vector<Sequence<uint, 2>>	ShowReward;

		void CopyFrom(RowData *pRowData)
		{
			this->Level=pRowData->Level;
			this->Rank=pRowData->Rank;
			this->ShowReward=pRowData->ShowReward;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Level", "Rank", "ShowReward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(WorldBossRewardTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Rank)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ShowReward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~WorldBossRewardTable ()
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