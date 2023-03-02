#ifndef __BagExpandItemListTable_H__
#define __BagExpandItemListTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/13 19:36:36


class BagExpandItemListTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ItemId;
		uint	Type;
		vector<Sequence<uint, 2>>	NeedAndOpen;

		void CopyFrom(RowData *pRowData)
		{
			this->ItemId=pRowData->ItemId;
			this->Type=pRowData->Type;
			this->NeedAndOpen=pRowData->NeedAndOpen;

		}
	};

	std::unordered_map<uint,int> ItemId_Index;
	std::unordered_map<uint,int> Type_Index;


    RowData *GetByItemId(const uint &key)
    {
        auto i = ItemId_Index.find(key);
        return (i == ItemId_Index.end()) ? NULL : Table[i->second];
    }
    
    RowData *GetByType(const uint &key)
    {
        auto i = Type_Index.find(key);
        return (i == Type_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ItemId", "Type", "NeedAndOpen",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(BagExpandItemListTable  *pSrcTable)
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
                Type_Index[pNewRow->Type]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ItemId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.NeedAndOpen)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ItemId_Index[row.ItemId] = Table.size()-1;
		Type_Index[row.Type] = Table.size()-1;


		return true;
	}

	~BagExpandItemListTable ()
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
		Type_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif