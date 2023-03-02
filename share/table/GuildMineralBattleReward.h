#ifndef __GuildMineralBattleReward_H__
#define __GuildMineralBattleReward_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/1 16:34:19


class GuildMineralBattleReward : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Rank;
		vector<Sequence<uint, 2>>	AdministratorReward1;
		vector<Sequence<uint, 2>>	AdministratorReward2;
		vector<Sequence<uint, 2>>	MembersReward;
		uint	GuildEXP;
		uint	GguildPrestige;
		vector<Sequence<int, 2>>	RewardShow;
		uint	LevelSeal;

		void CopyFrom(RowData *pRowData)
		{
			this->Rank=pRowData->Rank;
			this->AdministratorReward1=pRowData->AdministratorReward1;
			this->AdministratorReward2=pRowData->AdministratorReward2;
			this->MembersReward=pRowData->MembersReward;
			this->GuildEXP=pRowData->GuildEXP;
			this->GguildPrestige=pRowData->GguildPrestige;
			this->RewardShow=pRowData->RewardShow;
			this->LevelSeal=pRowData->LevelSeal;

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
		const char *Headers[] = { "Rank", "AdministratorReward1", "AdministratorReward2", "MembersReward", "GuildEXP", "GguildPrestige", "RewardShow", "LevelSeal",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildMineralBattleReward  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.AdministratorReward1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.AdministratorReward2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.MembersReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.GuildEXP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.GguildPrestige)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.RewardShow)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.LevelSeal)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Rank_Index[row.Rank] = Table.size()-1;


		return true;
	}

	~GuildMineralBattleReward ()
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