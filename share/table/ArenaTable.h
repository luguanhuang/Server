#ifndef __ArenaTable_H__
#define __ArenaTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2016/5/21 11:37:04


class ArenaTable : public CVSReader
{
public:

	struct RowData
	{
		Sequence<uint, 2>	ArenaRank;
		Sequence<uint, 2>	CanAttack;
		Sequence<uint, 2>	DailyDragonCoin;
		Sequence<uint, 2>	DailyGoldCoin;
		Sequence<uint, 2>	OccupyNpcRatio;
		Sequence<uint, 2>	OccupyNpcPowerPoint;
		float	uprewardcoef;
		bool	canattackrole;
		Sequence<uint, 2>	ArenaNpcLevel;

	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ArenaRank", "CanAttack", "DailyDragonCoin", "DailyGoldCoin", "OccupyNpcRatio", "OccupyNpcPowerPoint", "uprewardcoef", "canattackrole", "ArenaNpcLevel",  };
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

		if (!Parse(Fields[m_ColMap[0]], row.ArenaRank)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.CanAttack)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.DailyDragonCoin)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.DailyGoldCoin)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.OccupyNpcRatio)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.OccupyNpcPowerPoint)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.uprewardcoef)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.canattackrole)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.ArenaNpcLevel)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~ArenaTable ()
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