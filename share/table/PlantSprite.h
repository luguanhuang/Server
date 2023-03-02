#ifndef __PlantSprite_H__
#define __PlantSprite_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PlantSprite : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SpriteID;
		uint	OccurrenceWeitght;
		uint	ReduceGrowth;
		uint	EffectGrowRate;
		uint	Duration;
		uint	DamageRate;
		vector<Sequence<uint, 2>>	AwardItems;
		vector<string>	Dialogues;
		vector<uint>	Plants;

		void CopyFrom(RowData *pRowData)
		{
			this->SpriteID=pRowData->SpriteID;
			this->OccurrenceWeitght=pRowData->OccurrenceWeitght;
			this->ReduceGrowth=pRowData->ReduceGrowth;
			this->EffectGrowRate=pRowData->EffectGrowRate;
			this->Duration=pRowData->Duration;
			this->DamageRate=pRowData->DamageRate;
			this->AwardItems=pRowData->AwardItems;
			this->Dialogues=pRowData->Dialogues;
			this->Plants=pRowData->Plants;

		}
	};

	std::unordered_map<uint,int> SpriteID_Index;


    RowData *GetBySpriteID(const uint &key)
    {
        auto i = SpriteID_Index.find(key);
        return (i == SpriteID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SpriteID", "OccurrenceWeitght", "ReduceGrowth", "EffectGrowRate", "Duration", "DamageRate", "AwardItems", "Dialogues", "Plants",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PlantSprite  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySpriteID(pSrcRow->SpriteID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SpriteID_Index[pNewRow->SpriteID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.SpriteID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.OccurrenceWeitght)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ReduceGrowth)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.EffectGrowRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Duration)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.DamageRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.AwardItems)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Dialogues)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Plants)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SpriteID_Index[row.SpriteID] = Table.size()-1;


		return true;
	}

	~PlantSprite ()
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

		SpriteID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif