#ifndef __CookingFoodInfo_H__
#define __CookingFoodInfo_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class CookingFoodInfo : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	FoodID;
		string	FoodName;
		string	Desc;
		uint	AddExp;
		vector<Sequence<uint, 2>>	Ingredients;
		uint	BuffID;
		uint	Duration;
		uint	Frequency;
		uint	Level;
		uint	CookBookID;
		uint	NeededExp;
		vector<uint>	Profession;

		void CopyFrom(RowData *pRowData)
		{
			this->FoodID=pRowData->FoodID;
			this->FoodName=pRowData->FoodName;
			this->Desc=pRowData->Desc;
			this->AddExp=pRowData->AddExp;
			this->Ingredients=pRowData->Ingredients;
			this->BuffID=pRowData->BuffID;
			this->Duration=pRowData->Duration;
			this->Frequency=pRowData->Frequency;
			this->Level=pRowData->Level;
			this->CookBookID=pRowData->CookBookID;
			this->NeededExp=pRowData->NeededExp;
			this->Profession=pRowData->Profession;

		}
	};

	std::unordered_map<uint,int> FoodID_Index;


    RowData *GetByFoodID(const uint &key)
    {
        auto i = FoodID_Index.find(key);
        return (i == FoodID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "FoodID", "FoodName", "Desc", "AddExp", "Ingredients", "BuffID", "Duration", "Frequency", "Level", "CookBookID", "NeededExp", "Profession",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CookingFoodInfo  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByFoodID(pSrcRow->FoodID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                FoodID_Index[pNewRow->FoodID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.FoodID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.FoodName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Desc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.AddExp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Ingredients)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.BuffID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Duration)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Frequency)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.CookBookID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.NeededExp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.Profession)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		FoodID_Index[row.FoodID] = Table.size()-1;


		return true;
	}

	~CookingFoodInfo ()
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

		FoodID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif