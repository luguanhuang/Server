﻿#ifndef __GuildArenaReward_H__
#define __GuildArenaReward_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/2/7 0:14:50


class GuildArenaReward : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ID;
		Sequence<uint, 2>	range;
		uint	type;
		vector<Sequence<uint, 2>>	reward;
		vector<Sequence<uint, 2>>	guildreward;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->range=pRowData->range;
			this->type=pRowData->type;
			this->reward=pRowData->reward;
			this->guildreward=pRowData->guildreward;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "range", "type", "reward", "guildreward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildArenaReward  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.range)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.guildreward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GuildArenaReward ()
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