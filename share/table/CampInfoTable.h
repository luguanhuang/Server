#ifndef __CampInfoTable_H__
#define __CampInfoTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2016/5/6 15:19:52


class CampInfoTable : public CVSReader
{
public:

	struct RowData
	{
		int	CampID;
		string	CampName;
		string	BgTextureName;
		string	FlagSpriteName;
		Sequence<float, 4>	TextColorRGBA;

	};

	std::unordered_map<int,int> CampID_Index;


        RowData *GetByCampID(const int &key)
        {
            auto i = CampID_Index.find(key);
            return (i == CampID_Index.end()) ? NULL : Table[i->second];
        }
        

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "CampID", "CampName", "BgTextureName", "FlagSpriteName", "TextColorRGBA",  };
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
		if (!Parse(Fields[m_ColMap[1]], row.CampName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.BgTextureName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.FlagSpriteName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.TextColorRGBA)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		CampID_Index[row.CampID] = Table.size()-1;


		return true;
	}

	~CampInfoTable ()
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