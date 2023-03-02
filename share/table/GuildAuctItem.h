﻿#ifndef __GuildAuctItem_H__
#define __GuildAuctItem_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildAuctItem : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ItemID;
		string	ItemName;
		int	ItemType;
		uint	GuildBasePrice;
		uint	GuildMaxPrice;
		uint	WorldBasePrice;
		uint	WorldMaxPrice;

		void CopyFrom(RowData *pRowData)
		{
			this->ItemID=pRowData->ItemID;
			this->ItemName=pRowData->ItemName;
			this->ItemType=pRowData->ItemType;
			this->GuildBasePrice=pRowData->GuildBasePrice;
			this->GuildMaxPrice=pRowData->GuildMaxPrice;
			this->WorldBasePrice=pRowData->WorldBasePrice;
			this->WorldMaxPrice=pRowData->WorldMaxPrice;

		}
	};

	std::unordered_map<uint,int> ItemID_Index;


    RowData *GetByItemID(const uint &key)
    {
        auto i = ItemID_Index.find(key);
        return (i == ItemID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ItemID", "ItemName", "ItemType", "GuildBasePrice", "GuildMaxPrice", "WorldBasePrice", "WorldMaxPrice",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildAuctItem  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByItemID(pSrcRow->ItemID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ItemID_Index[pNewRow->ItemID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ItemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ItemName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ItemType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.GuildBasePrice)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.GuildMaxPrice)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.WorldBasePrice)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.WorldMaxPrice)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ItemID_Index[row.ItemID] = Table.size()-1;


		return true;
	}

	~GuildAuctItem ()
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

		ItemID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif