#ifndef __CompeteDragonTable_H__
#define __CompeteDragonTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/7 15:09:04


class CompeteDragonTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	LevelSealType;
		vector<int>	ExpIDs;
		int	MCount;
		vector<Sequence<int, 2>>	Reward;

		void CopyFrom(RowData *pRowData)
		{
			this->LevelSealType=pRowData->LevelSealType;
			this->ExpIDs=pRowData->ExpIDs;
			this->MCount=pRowData->MCount;
			this->Reward=pRowData->Reward;

		}
	};

	std::unordered_map<int,int> LevelSealType_Index;


    RowData *GetByLevelSealType(const int &key)
    {
        auto i = LevelSealType_Index.find(key);
        return (i == LevelSealType_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "LevelSealType", "ExpIDs", "MCount", "Reward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(CompeteDragonTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByLevelSealType(pSrcRow->LevelSealType);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                LevelSealType_Index[pNewRow->LevelSealType]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.LevelSealType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ExpIDs)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.MCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Reward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		LevelSealType_Index[row.LevelSealType] = Table.size()-1;


		return true;
	}

	~CompeteDragonTable ()
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

		LevelSealType_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif