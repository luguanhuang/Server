#ifndef __GuildMineralBattle_H__
#define __GuildMineralBattle_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/7/4 16:26:00


class GuildMineralBattle : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ID;
		uint	Level;
		uint	Type;
		uint	Mineralcounts;
		int	DifficultLevel;
		uint	BossID;
		uint	Percent;
		float	atkpercent;
		float	defpercent;
		float	lifepercent;
		Sequence<int, 2>	ragebuff;
		uint	MineralReward;
		vector<Sequence<uint, 2>>	MineralBuffReward;
		vector<uint>	MineralDrop;
		uint	ragetime;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Level=pRowData->Level;
			this->Type=pRowData->Type;
			this->Mineralcounts=pRowData->Mineralcounts;
			this->DifficultLevel=pRowData->DifficultLevel;
			this->BossID=pRowData->BossID;
			this->Percent=pRowData->Percent;
			this->atkpercent=pRowData->atkpercent;
			this->defpercent=pRowData->defpercent;
			this->lifepercent=pRowData->lifepercent;
			this->ragebuff=pRowData->ragebuff;
			this->MineralReward=pRowData->MineralReward;
			this->MineralBuffReward=pRowData->MineralBuffReward;
			this->MineralDrop=pRowData->MineralDrop;
			this->ragetime=pRowData->ragetime;

		}
	};

	std::unordered_map<uint,int> ID_Index;


    RowData *GetByID(const uint &key)
    {
        auto i = ID_Index.find(key);
        return (i == ID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "Level", "Type", "Mineralcounts", "DifficultLevel", "BossID", "Percent", "atkpercent", "defpercent", "lifepercent", "ragebuff", "MineralReward", "MineralBuffReward", "MineralDrop", "ragetime",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildMineralBattle  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByID(pSrcRow->ID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ID_Index[pNewRow->ID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Mineralcounts)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.DifficultLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.BossID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Percent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.atkpercent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.defpercent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.lifepercent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.ragebuff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.MineralReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.MineralBuffReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.MineralDrop)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.ragetime)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~GuildMineralBattle ()
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

		ID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif