#ifndef __GMAddItem_H__
#define __GMAddItem_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/26 11:19:17


class GMAddItem : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	itemid;
		uint	itemcounts;

		void CopyFrom(RowData *pRowData)
		{
			this->itemid=pRowData->itemid;
			this->itemcounts=pRowData->itemcounts;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "itemid", "itemcounts",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GMAddItem  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.itemid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.itemcounts)) return false; ++m_ColNum;


		Table.push_back(pRow);



		return true;
	}

	~GMAddItem ()
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