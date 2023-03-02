﻿#ifndef __ItemBuffTable_H__
#define __ItemBuffTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class ItemBuffTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ItemId;
		vector<Sequence<uint, 2>>	Buffs;
		string	Name;
		uint	Type;

		void CopyFrom(RowData *pRowData)
		{
			this->ItemId=pRowData->ItemId;
			this->Buffs=pRowData->Buffs;
			this->Name=pRowData->Name;
			this->Type=pRowData->Type;

		}
	};

	std::unordered_map<uint,int> ItemId_Index;


    RowData *GetByItemId(const uint &key)
    {
        auto i = ItemId_Index.find(key);
        return (i == ItemId_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ItemId", "Buffs", "Name", "Type",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ItemBuffTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByItemId(pSrcRow->ItemId);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ItemId_Index[pNewRow->ItemId]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ItemId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Buffs)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Type)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ItemId_Index[row.ItemId] = Table.size()-1;


		return true;
	}

	~ItemBuffTable ()
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

		ItemId_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif