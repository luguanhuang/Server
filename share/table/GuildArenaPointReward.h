#ifndef __GuildArenaPointReward_H__
#define __GuildArenaPointReward_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2016/11/14 16:53:02


class GuildArenaPointReward : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		uint	type;
		uint	guildexp;
		uint	guildprestige;
		vector<Sequence<uint, 2>>	reward;

	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "type", "guildexp", "guildprestige", "reward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.id)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.guildexp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.guildprestige)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.reward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GuildArenaPointReward ()
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