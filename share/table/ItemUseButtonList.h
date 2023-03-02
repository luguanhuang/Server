#ifndef __ItemUseButtonList_H__
#define __ItemUseButtonList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class ItemUseButtonList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ItemID;
		string	ItemName;
		string	ButtonName;
		uint	SystemID;
		uint	TypeID;

		void CopyFrom(RowData *pRowData)
		{
			this->ItemID=pRowData->ItemID;
			this->ItemName=pRowData->ItemName;
			this->ButtonName=pRowData->ButtonName;
			this->SystemID=pRowData->SystemID;
			this->TypeID=pRowData->TypeID;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ItemID", "ItemName", "ButtonName", "SystemID", "TypeID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ItemUseButtonList  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ItemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ItemName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ButtonName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SystemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.TypeID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~ItemUseButtonList ()
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