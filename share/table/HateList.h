#ifndef __HateList_H__
#define __HateList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/9 15:01:13


class HateList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	TypeID;
		vector<Sequence<uint, 2>>	HateValue;

		void CopyFrom(RowData *pRowData)
		{
			this->TypeID=pRowData->TypeID;
			this->HateValue=pRowData->HateValue;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "TypeID", "HateValue",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(HateList  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.TypeID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.HateValue)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~HateList ()
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