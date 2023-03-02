#ifndef __TipsTable_H__
#define __TipsTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2016/7/4 19:18:46


class TipsTable : public CVSReader
{
public:

	struct RowData
	{
		int	id;
		string	content;

	};

	std::unordered_map<int,int> id_Index;


        RowData *GetByid(const int &key)
        {
            auto i = id_Index.find(key);
            return (i == id_Index.end()) ? NULL : Table[i->second];
        }
        

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "content",  };
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
		if (!Parse(Fields[m_ColMap[1]], row.content)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		id_Index[row.id] = Table.size()-1;


		return true;
	}

	~TipsTable ()
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