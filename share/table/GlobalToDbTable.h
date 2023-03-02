#ifndef __GlobalToDbTable_H__
#define __GlobalToDbTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GlobalToDbTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	name;
		string	value;
		uint	index;

		void CopyFrom(RowData *pRowData)
		{
			this->name=pRowData->name;
			this->value=pRowData->value;
			this->index=pRowData->index;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "name", "value", "index",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GlobalToDbTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.value)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.index)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GlobalToDbTable ()
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