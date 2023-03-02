#ifndef __TeamTowerRewardTable_H__
#define __TeamTowerRewardTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class TeamTowerRewardTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	TowerHardLevel;
		int	TowerFloor;
		vector<Sequence<int, 2>>	Reward;
		string	Name;
		int	DragonCoinFindBackCost;
		int	GoldCoinCost;
		vector<Sequence<int, 2>>	GoldCoinFindBackItem;
		int	SceneID;
		vector<Sequence<int, 2>>	FirstPassReward;
		int	preward;

		void CopyFrom(RowData *pRowData)
		{
			this->TowerHardLevel=pRowData->TowerHardLevel;
			this->TowerFloor=pRowData->TowerFloor;
			this->Reward=pRowData->Reward;
			this->Name=pRowData->Name;
			this->DragonCoinFindBackCost=pRowData->DragonCoinFindBackCost;
			this->GoldCoinCost=pRowData->GoldCoinCost;
			this->GoldCoinFindBackItem=pRowData->GoldCoinFindBackItem;
			this->SceneID=pRowData->SceneID;
			this->FirstPassReward=pRowData->FirstPassReward;
			this->preward=pRowData->preward;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "TowerHardLevel", "TowerFloor", "Reward", "Name", "DragonCoinFindBackCost", "GoldCoinCost", "GoldCoinFindBackItem", "SceneID", "FirstPassReward", "preward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(TeamTowerRewardTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.TowerHardLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.TowerFloor)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.DragonCoinFindBackCost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.GoldCoinCost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.GoldCoinFindBackItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.FirstPassReward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.preward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~TeamTowerRewardTable ()
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