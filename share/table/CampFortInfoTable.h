#ifndef __CampFortInfoTable_H__
#define __CampFortInfoTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2016/5/4 16:47:20


class CampFortInfoTable : public CVSReader
{
public:

	struct RowData
	{
		uint	CampID;
		uint	FortID;
		string	FortName;
		uint	NeedLevel;
		string	SpriteName;
		Sequence<int, 2>	SpriteSize;
		Sequence<int, 2>	UIPos;
		int	Type;

	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "CampID", "FortID", "FortName", "NeedLevel", "SpriteName", "SpriteSize", "UIPos", "Type",  };
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

		if (!Parse(Fields[m_ColMap[0]], row.CampID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.FortID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.FortName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.NeedLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.SpriteName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.SpriteSize)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.UIPos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Type)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~CampFortInfoTable ()
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