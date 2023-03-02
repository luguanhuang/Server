#ifndef __GuildArenaBattlePvpReward_H__
#define __GuildArenaBattlePvpReward_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2016/11/28 23:26:26


class GuildArenaBattlePvpReward : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ID;
		uint	Level;
		uint	MineralReward;
		vector<Sequence<int, 2>>	MineralBuffReward;
		vector<uint>	MineralDrop;

	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "Level", "MineralReward", "MineralBuffReward", "MineralDrop",  };
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

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.MineralReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.MineralBuffReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.MineralDrop)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GuildArenaBattlePvpReward ()
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