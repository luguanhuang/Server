#ifndef __HorseRace_H__
#define __HorseRace_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/4 16:09:52


class HorseRace : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Rank;
		vector<Sequence<uint, 2>>	Reward;
		uint	sceneid;

		void CopyFrom(RowData *pRowData)
		{
			this->Rank=pRowData->Rank;
			this->Reward=pRowData->Reward;
			this->sceneid=pRowData->sceneid;

		}
	};

	std::unordered_map<uint,int> Rank_Index;


    RowData *GetByRank(const uint &key)
    {
        auto i = Rank_Index.find(key);
        return (i == Rank_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Rank", "Reward", "sceneid",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(HorseRace  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByRank(pSrcRow->Rank);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                Rank_Index[pNewRow->Rank]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.Rank)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.sceneid)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Rank_Index[row.Rank] = Table.size()-1;


		return true;
	}

	~HorseRace ()
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

		Rank_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif