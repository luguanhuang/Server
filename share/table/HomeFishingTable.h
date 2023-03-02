#ifndef __HomeFishingTable_H__
#define __HomeFishingTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2016/10/24 10:40:35


class HomeFishingTable : public CVSReader
{
public:

	struct RowData
	{
		uint	Level;
		uint	LevelUpExp;
		vector<Sequence<uint, 2>>	Fish;
		int	FishRate;

	};

	std::unordered_map<uint,int> Level_Index;


        RowData *GetByLevel(const uint &key)
        {
            auto i = Level_Index.find(key);
            return (i == Level_Index.end()) ? NULL : Table[i->second];
        }
        

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Level", "LevelUpExp", "Fish", "FishRate",  };
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

		if (!Parse(Fields[m_ColMap[0]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.LevelUpExp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Fish)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.FishRate)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Level_Index[row.Level] = Table.size()-1;


		return true;
	}

	~HomeFishingTable ()
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