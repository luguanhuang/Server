#ifndef __CampFortListTable_H__
#define __CampFortListTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2016/5/4 14:19:48


class CampFortListTable : public CVSReader
{
public:

	struct RowData
	{
		int	BelongCampID;
		Sequence<float, 2>	UIPos;
		int	FortID;

	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "BelongCampID", "UIPos", "FortID",  };
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

		if (!Parse(Fields[m_ColMap[0]], row.BelongCampID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.UIPos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.FortID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~CampFortListTable ()
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