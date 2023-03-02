#ifndef __BackflowShop_H__
#define __BackflowShop_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/27 11:29:50


class BackflowShop : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	GoodID;
		uint	ItemID;
		uint	ItemCount;
		uint	CostType;
		uint	CostNum;
		uint	Discount;
		Sequence<uint, 2>	LevelRange;
		Sequence<uint, 2>	LostDayRange;
		Sequence<uint, 2>	PayRange;
		uint	Weight;
		uint	Quality;

		void CopyFrom(RowData *pRowData)
		{
			this->GoodID=pRowData->GoodID;
			this->ItemID=pRowData->ItemID;
			this->ItemCount=pRowData->ItemCount;
			this->CostType=pRowData->CostType;
			this->CostNum=pRowData->CostNum;
			this->Discount=pRowData->Discount;
			this->LevelRange=pRowData->LevelRange;
			this->LostDayRange=pRowData->LostDayRange;
			this->PayRange=pRowData->PayRange;
			this->Weight=pRowData->Weight;
			this->Quality=pRowData->Quality;

		}
	};

	std::unordered_map<uint,int> GoodID_Index;


    RowData *GetByGoodID(const uint &key)
    {
        auto i = GoodID_Index.find(key);
        return (i == GoodID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "GoodID", "ItemID", "ItemCount", "CostType", "CostNum", "Discount", "LevelRange", "LostDayRange", "PayRange", "Weight", "Quality",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(BackflowShop  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByGoodID(pSrcRow->GoodID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                GoodID_Index[pNewRow->GoodID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.GoodID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ItemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ItemCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.CostType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.CostNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Discount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.LevelRange)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.LostDayRange)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.PayRange)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Weight)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.Quality)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		GoodID_Index[row.GoodID] = Table.size()-1;


		return true;
	}

	~BackflowShop ()
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

		GoodID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif