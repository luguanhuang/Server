#ifndef __SkyArenaReward_H__
#define __SkyArenaReward_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class SkyArenaReward : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	LevelSegment;
		int	Floor;
		vector<Sequence<uint, 2>>	Reward;

		void CopyFrom(RowData *pRowData)
		{
			this->LevelSegment=pRowData->LevelSegment;
			this->Floor=pRowData->Floor;
			this->Reward=pRowData->Reward;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "LevelSegment", "Floor", "Reward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SkyArenaReward  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.LevelSegment)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Floor)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Reward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~SkyArenaReward ()
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