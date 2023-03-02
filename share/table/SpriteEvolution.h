#ifndef __SpriteEvolution_H__
#define __SpriteEvolution_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/8 10:28:56


class SpriteEvolution : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SpriteID;
		uint	Quality;
		uint	EvolutionLevel;
		uint	LevelLimit;
		Sequence<uint, 2>	EvolutionCost;
		Sequence<uint, 2>	TrainExp;
		vector<Sequence<uint, 2>>	ResetTrainCost;
		vector<Sequence<uint, 2>>	AddAttr;
		vector<Sequence<uint, 2>>	AttrExtra;
		vector<Sequence<uint, 3>>	TrainAttr;

		void CopyFrom(RowData *pRowData)
		{
			this->SpriteID=pRowData->SpriteID;
			this->Quality=pRowData->Quality;
			this->EvolutionLevel=pRowData->EvolutionLevel;
			this->LevelLimit=pRowData->LevelLimit;
			this->EvolutionCost=pRowData->EvolutionCost;
			this->TrainExp=pRowData->TrainExp;
			this->ResetTrainCost=pRowData->ResetTrainCost;
			this->AddAttr=pRowData->AddAttr;
			this->AttrExtra=pRowData->AttrExtra;
			this->TrainAttr=pRowData->TrainAttr;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SpriteID", "Quality", "EvolutionLevel", "LevelLimit", "EvolutionCost", "TrainExp", "ResetTrainCost", "AddAttr", "AttrExtra", "TrainAttr",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SpriteEvolution  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.SpriteID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Quality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.EvolutionLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.LevelLimit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.EvolutionCost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.TrainExp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.ResetTrainCost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.AddAttr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.AttrExtra)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.TrainAttr)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~SpriteEvolution ()
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