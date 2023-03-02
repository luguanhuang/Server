#ifndef __EnhanceAttr_H__
#define __EnhanceAttr_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2016/11/12 16:36:16


class EnhanceAttr : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EquipPos;
		uint	EnhanceLevel;
		vector<Sequence<uint, 2>>	EnhanceAttribute;

	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EquipPos", "EnhanceLevel", "EnhanceAttribute",  };
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

		if (!Parse(Fields[m_ColMap[0]], row.EquipPos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.EnhanceLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.EnhanceAttribute)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~EnhanceAttr ()
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