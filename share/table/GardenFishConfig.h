#ifndef __GardenFishConfig_H__
#define __GardenFishConfig_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GardenFishConfig : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	FishLeve;
		uint	Experience;
		vector<Sequence<int, 2>>	FishWeight;
		uint	SuccessRate;

		void CopyFrom(RowData *pRowData)
		{
			this->FishLeve=pRowData->FishLeve;
			this->Experience=pRowData->Experience;
			this->FishWeight=pRowData->FishWeight;
			this->SuccessRate=pRowData->SuccessRate;

		}
	};

	std::unordered_map<uint,int> FishLeve_Index;


    RowData *GetByFishLeve(const uint &key)
    {
        auto i = FishLeve_Index.find(key);
        return (i == FishLeve_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "FishLeve", "Experience", "FishWeight", "SuccessRate",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GardenFishConfig  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByFishLeve(pSrcRow->FishLeve);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                FishLeve_Index[pNewRow->FishLeve]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.FishLeve)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Experience)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.FishWeight)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SuccessRate)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		FishLeve_Index[row.FishLeve] = Table.size()-1;


		return true;
	}

	~GardenFishConfig ()
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

		FishLeve_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif