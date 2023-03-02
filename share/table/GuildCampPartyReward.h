#ifndef __GuildCampPartyReward_H__
#define __GuildCampPartyReward_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/5 14:05:27


class GuildCampPartyReward : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ID;
		vector<Sequence<uint, 2>>	Items;
		vector<Sequence<uint, 2>>	Reward;
		uint	Count;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Items=pRowData->Items;
			this->Reward=pRowData->Reward;
			this->Count=pRowData->Count;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "Items", "Reward", "Count",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildCampPartyReward  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Items)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Count)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GuildCampPartyReward ()
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