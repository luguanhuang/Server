#ifndef __GuildBossRewardTable_H__
#define __GuildBossRewardTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildBossRewardTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	rank;
		vector<Sequence<uint, 2>>	reward;
		vector<Sequence<uint, 2>>	rewardMin;
		vector<Sequence<uint, 2>>	guildreward;
		vector<Sequence<uint, 2>>	guildexp;

		void CopyFrom(RowData *pRowData)
		{
			this->rank=pRowData->rank;
			this->reward=pRowData->reward;
			this->rewardMin=pRowData->rewardMin;
			this->guildreward=pRowData->guildreward;
			this->guildexp=pRowData->guildexp;

		}
	};

	std::unordered_map<uint,int> rank_Index;


    RowData *GetByrank(const uint &key)
    {
        auto i = rank_Index.find(key);
        return (i == rank_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "rank", "reward", "rewardMin", "guildreward", "guildexp",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildBossRewardTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByrank(pSrcRow->rank);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                rank_Index[pNewRow->rank]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.rank)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.rewardMin)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.guildreward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.guildexp)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		rank_Index[row.rank] = Table.size()-1;


		return true;
	}

	~GuildBossRewardTable ()
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

		rank_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif