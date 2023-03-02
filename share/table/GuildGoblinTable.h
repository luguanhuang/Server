#ifndef __GuildGoblinTable_H__
#define __GuildGoblinTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildGoblinTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	Level;
		int	UpgradeNum;
		float	HpAddPer;
		float	AttackAddPer;
		int	Contribute;
		int	BigGoblinContribute;
		Sequence<int, 2>	BonusContent;
		vector<Sequence<int, 2>>	DragonCoinFindBack;
		vector<Sequence<int, 2>>	GoldCoinFindBack;
		vector<Sequence<uint, 2>>	guildreward;

		void CopyFrom(RowData *pRowData)
		{
			this->Level=pRowData->Level;
			this->UpgradeNum=pRowData->UpgradeNum;
			this->HpAddPer=pRowData->HpAddPer;
			this->AttackAddPer=pRowData->AttackAddPer;
			this->Contribute=pRowData->Contribute;
			this->BigGoblinContribute=pRowData->BigGoblinContribute;
			this->BonusContent=pRowData->BonusContent;
			this->DragonCoinFindBack=pRowData->DragonCoinFindBack;
			this->GoldCoinFindBack=pRowData->GoldCoinFindBack;
			this->guildreward=pRowData->guildreward;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Level", "UpgradeNum", "HpAddPer", "AttackAddPer", "Contribute", "BigGoblinContribute", "BonusContent", "DragonCoinFindBack", "GoldCoinFindBack", "guildreward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildGoblinTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.UpgradeNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.HpAddPer)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.AttackAddPer)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Contribute)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.BigGoblinContribute)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.BonusContent)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.DragonCoinFindBack)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.GoldCoinFindBack)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.guildreward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GuildGoblinTable ()
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


	}

	std::vector<RowData*> Table;
};

#endif