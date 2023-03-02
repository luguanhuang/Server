#ifndef __PlantSeed_H__
#define __PlantSeed_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PlantSeed : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SeedID;
		string	SeedName;
		Sequence<int, 2>	PlantID;
		string	PlantName;
		string	Desc;
		Sequence<uint, 2>	FirstGrowSection;
		Sequence<uint, 2>	SecondGrowSection;
		Sequence<uint, 2>	RipeSection;
		uint	GrowUpAmount;
		uint	GrowUpAmountPerMinute;
		uint	PredictGrowUpTime;
		uint	HarvestRate;
		Sequence<uint, 2>	HarvestPlant;
		string	HarvestPlantName;
		uint	ExtraDropRate;
		Sequence<uint, 2>	ExtraDropItem;
		string	ExtraDropItemName;
		uint	IncreaseGrowUpRate;
		uint	ReduceGrowUpRate;
		uint	PlantStateCD;
		uint	MatureDuration;
		uint	BadStateGrowUpRate;
		Sequence<int, 2>	StealAward;
		Sequence<int, 2>	TrainAward;
		uint	FirstGrowModelSpriteId;
		uint	SecondGrowSpriteId;
		uint	RipeSpriteID;
		uint	CanStealMaxTimes;
		uint	InitUpAmount;

		void CopyFrom(RowData *pRowData)
		{
			this->SeedID=pRowData->SeedID;
			this->SeedName=pRowData->SeedName;
			this->PlantID=pRowData->PlantID;
			this->PlantName=pRowData->PlantName;
			this->Desc=pRowData->Desc;
			this->FirstGrowSection=pRowData->FirstGrowSection;
			this->SecondGrowSection=pRowData->SecondGrowSection;
			this->RipeSection=pRowData->RipeSection;
			this->GrowUpAmount=pRowData->GrowUpAmount;
			this->GrowUpAmountPerMinute=pRowData->GrowUpAmountPerMinute;
			this->PredictGrowUpTime=pRowData->PredictGrowUpTime;
			this->HarvestRate=pRowData->HarvestRate;
			this->HarvestPlant=pRowData->HarvestPlant;
			this->HarvestPlantName=pRowData->HarvestPlantName;
			this->ExtraDropRate=pRowData->ExtraDropRate;
			this->ExtraDropItem=pRowData->ExtraDropItem;
			this->ExtraDropItemName=pRowData->ExtraDropItemName;
			this->IncreaseGrowUpRate=pRowData->IncreaseGrowUpRate;
			this->ReduceGrowUpRate=pRowData->ReduceGrowUpRate;
			this->PlantStateCD=pRowData->PlantStateCD;
			this->MatureDuration=pRowData->MatureDuration;
			this->BadStateGrowUpRate=pRowData->BadStateGrowUpRate;
			this->StealAward=pRowData->StealAward;
			this->TrainAward=pRowData->TrainAward;
			this->FirstGrowModelSpriteId=pRowData->FirstGrowModelSpriteId;
			this->SecondGrowSpriteId=pRowData->SecondGrowSpriteId;
			this->RipeSpriteID=pRowData->RipeSpriteID;
			this->CanStealMaxTimes=pRowData->CanStealMaxTimes;
			this->InitUpAmount=pRowData->InitUpAmount;

		}
	};

	std::unordered_map<uint,int> SeedID_Index;


    RowData *GetBySeedID(const uint &key)
    {
        auto i = SeedID_Index.find(key);
        return (i == SeedID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SeedID", "SeedName", "PlantID", "PlantName", "Desc", "FirstGrowSection", "SecondGrowSection", "RipeSection", "GrowUpAmount", "GrowUpAmountPerMinute", "PredictGrowUpTime", "HarvestRate", "HarvestPlant", "HarvestPlantName", "ExtraDropRate", "ExtraDropItem", "ExtraDropItemName", "IncreaseGrowUpRate", "ReduceGrowUpRate", "PlantStateCD", "MatureDuration", "BadStateGrowUpRate", "StealAward", "TrainAward", "FirstGrowModelSpriteId", "SecondGrowSpriteId", "RipeSpriteID", "CanStealMaxTimes", "InitUpAmount",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PlantSeed  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySeedID(pSrcRow->SeedID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SeedID_Index[pNewRow->SeedID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.SeedID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SeedName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.PlantID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.PlantName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Desc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.FirstGrowSection)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.SecondGrowSection)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.RipeSection)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.GrowUpAmount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.GrowUpAmountPerMinute)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.PredictGrowUpTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.HarvestRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.HarvestPlant)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.HarvestPlantName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.ExtraDropRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.ExtraDropItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.ExtraDropItemName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.IncreaseGrowUpRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.ReduceGrowUpRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.PlantStateCD)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.MatureDuration)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.BadStateGrowUpRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.StealAward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.TrainAward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.FirstGrowModelSpriteId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[25]], row.SecondGrowSpriteId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[26]], row.RipeSpriteID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[27]], row.CanStealMaxTimes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[28]], row.InitUpAmount)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SeedID_Index[row.SeedID] = Table.size()-1;


		return true;
	}

	~PlantSeed ()
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

		SeedID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif