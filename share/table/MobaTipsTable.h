#ifndef __MobaTipsTable_H__
#define __MobaTipsTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/13 11:03:45


class MobaTipsTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ID;
		string	Tips;
		string	Effect;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Tips=pRowData->Tips;
			this->Effect=pRowData->Effect;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "Tips", "Effect",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(MobaTipsTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Tips)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Effect)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~MobaTipsTable ()
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