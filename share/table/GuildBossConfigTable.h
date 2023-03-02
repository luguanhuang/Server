#ifndef __GuildBossConfigTable_H__
#define __GuildBossConfigTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildBossConfigTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	BossID;
		uint	GuildLevel;
		uint	EnemyID;
		float	AttackPercent;
		float	LifePercent;
		string	BossName;
		vector<Sequence<uint, 2>>	FirsttKillReward;
		string	FirstKillDesignation;
		vector<Sequence<uint, 2>>	JoinReward;
		vector<Sequence<uint, 2>>	KillReward;
		string	WinCutScene;

		void CopyFrom(RowData *pRowData)
		{
			this->BossID=pRowData->BossID;
			this->GuildLevel=pRowData->GuildLevel;
			this->EnemyID=pRowData->EnemyID;
			this->AttackPercent=pRowData->AttackPercent;
			this->LifePercent=pRowData->LifePercent;
			this->BossName=pRowData->BossName;
			this->FirsttKillReward=pRowData->FirsttKillReward;
			this->FirstKillDesignation=pRowData->FirstKillDesignation;
			this->JoinReward=pRowData->JoinReward;
			this->KillReward=pRowData->KillReward;
			this->WinCutScene=pRowData->WinCutScene;

		}
	};

	std::unordered_map<uint,int> BossID_Index;


    RowData *GetByBossID(const uint &key)
    {
        auto i = BossID_Index.find(key);
        return (i == BossID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "BossID", "GuildLevel", "EnemyID", "AttackPercent", "LifePercent", "BossName", "FirsttKillReward", "FirstKillDesignation", "JoinReward", "KillReward", "WinCutScene",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildBossConfigTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByBossID(pSrcRow->BossID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                BossID_Index[pNewRow->BossID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.BossID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.GuildLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.EnemyID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.AttackPercent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.LifePercent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.BossName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.FirsttKillReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.FirstKillDesignation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.JoinReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.KillReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.WinCutScene)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		BossID_Index[row.BossID] = Table.size()-1;


		return true;
	}

	~GuildBossConfigTable ()
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

		BossID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif