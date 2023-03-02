#ifndef __ShopTypeTable_H__
#define __ShopTypeTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/8 17:46:52


class ShopTypeTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ShopID;
		string	ShopIcon;
		uint	ShopLevel;
		vector<Sequence<uint, 2>>	RefreshCost;
		vector<uint>	ShopCycle;
		vector<Sequence<uint, 2>>	ShopOpen;
		string	ShopName;
		uint	SystemId;
		int	IsHasTab;

		void CopyFrom(RowData *pRowData)
		{
			this->ShopID=pRowData->ShopID;
			this->ShopIcon=pRowData->ShopIcon;
			this->ShopLevel=pRowData->ShopLevel;
			this->RefreshCost=pRowData->RefreshCost;
			this->ShopCycle=pRowData->ShopCycle;
			this->ShopOpen=pRowData->ShopOpen;
			this->ShopName=pRowData->ShopName;
			this->SystemId=pRowData->SystemId;
			this->IsHasTab=pRowData->IsHasTab;

		}
	};

	std::unordered_map<uint,int> ShopID_Index;
	std::unordered_map<uint,int> SystemId_Index;


    RowData *GetByShopID(const uint &key)
    {
        auto i = ShopID_Index.find(key);
        return (i == ShopID_Index.end()) ? NULL : Table[i->second];
    }
    
    RowData *GetBySystemId(const uint &key)
    {
        auto i = SystemId_Index.find(key);
        return (i == SystemId_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ShopID", "ShopIcon", "ShopLevel", "RefreshCost", "ShopCycle", "ShopOpen", "ShopName", "SystemId", "IsHasTab",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ShopTypeTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByShopID(pSrcRow->ShopID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ShopID_Index[pNewRow->ShopID]=Table.size()-1;
                SystemId_Index[pNewRow->SystemId]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ShopID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ShopIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ShopLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.RefreshCost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.ShopCycle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.ShopOpen)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.ShopName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.SystemId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.IsHasTab)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ShopID_Index[row.ShopID] = Table.size()-1;
		SystemId_Index[row.SystemId] = Table.size()-1;


		return true;
	}

	~ShopTypeTable ()
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

		ShopID_Index.clear();
		SystemId_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif