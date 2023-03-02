#ifndef __MytheriousShopTable_H__
#define __MytheriousShopTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2015/6/24 14:53:08


class MythShopTable : public CVSReader
{
public:

	struct RowData
	{
		Sequence<int, 2>	LevelRuquire;
		vector<int>	DropID;

	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "LevelRuquire", "DropID",  };
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

		if (!Parse(Fields[m_ColMap[0]], row.LevelRuquire)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.DropID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~MythShopTable ()
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