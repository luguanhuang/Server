#ifndef __CombatCoff_H__
#define __CombatCoff_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2015/2/9 15:38:21


class CombatCoff : public CVSReader
{
public:

	struct RowData
	{
		string	CoffName;
		double	Value;

	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "CoffName", "Value",  };
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

		if (!Parse(Fields[m_ColMap[0]], row.CoffName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Value)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~CombatCoff ()
	{
		for (UINT32 i = 0; i < Table.size(); ++i)
		{
			delete Table[i];
		}
	}

	std::vector<RowData*> Table;
};

#endif