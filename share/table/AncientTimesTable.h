#ifndef __AncientTimesTable_H__
#define __AncientTimesTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/7/27 20:37:48


class AncientTimesTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ID;
		Sequence<uint, 2>	nPoints;
		vector<Sequence<uint, 2>>	Items;
		string	Title;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->nPoints=pRowData->nPoints;
			this->Items=pRowData->Items;
			this->Title=pRowData->Title;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "nPoints", "Items", "Title",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(AncientTimesTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.nPoints)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Items)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Title)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~AncientTimesTable ()
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