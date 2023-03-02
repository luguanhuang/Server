#ifndef __CircleDrawTable_H__
#define __CircleDrawTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class CircleDrawTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	UseItemID;
		uint	GiveItemID;
		uint	GiveItemCount;
		uint	GiveItemProb;
		uint	GiveItemIndex;
		uint	Notice;

		void CopyFrom(RowData *pRowData)
		{
			this->UseItemID=pRowData->UseItemID;
			this->GiveItemID=pRowData->GiveItemID;
			this->GiveItemCount=pRowData->GiveItemCount;
			this->GiveItemProb=pRowData->GiveItemProb;
			this->GiveItemIndex=pRowData->GiveItemIndex;
			this->Notice=pRowData->Notice;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "UseItemID", "GiveItemID", "GiveItemCount", "GiveItemProb", "GiveItemIndex", "Notice",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CircleDrawTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.UseItemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.GiveItemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.GiveItemCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.GiveItemProb)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.GiveItemIndex)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Notice)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~CircleDrawTable ()
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