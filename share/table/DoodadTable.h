#ifndef __DoodadTable_H__
#define __DoodadTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class DoodadTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	DoodadID;
		int	DoodadType;
		int	ItemID;
		int	ItemCount;
		int	DropProb;

		void CopyFrom(RowData *pRowData)
		{
			this->DoodadID=pRowData->DoodadID;
			this->DoodadType=pRowData->DoodadType;
			this->ItemID=pRowData->ItemID;
			this->ItemCount=pRowData->ItemCount;
			this->DropProb=pRowData->DropProb;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "DoodadID", "DoodadType", "ItemID", "ItemCount", "DropProb",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DoodadTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.DoodadID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.DoodadType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ItemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.ItemCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.DropProb)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~DoodadTable ()
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