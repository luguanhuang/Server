#ifndef __XSkillSpecialStatusTable_H__
#define __XSkillSpecialStatusTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"

// generate by ProtoGen at date: 2015/12/1 16:53:23


class XSkillSpecialStatusTable : public CVSReader
{
public:

	struct RowData
	{
		string	Name;
		uint	Value;

	};

	std::unordered_map<uint,int> Value_Index;


        RowData *GetByValue(const uint &key)
        {
            auto i = Value_Index.find(key);
            return (i == Value_Index.end()) ? NULL : Table[i->second];
        }
        

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Name", "Value",  };
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

		if (!Parse(Fields[m_ColMap[0]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Value)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Value_Index[row.Value] = Table.size()-1;


		return true;
	}

	~XSkillSpecialStatusTable ()
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