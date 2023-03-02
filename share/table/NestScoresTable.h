#ifndef __NestScoresTable_H__
#define __NestScoresTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class NestScoresTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	SceneID;
		int	MaxTimeScores;
		int	Time;
		int	MaxSurviveScores;
		int	DeadScores;
		vector<int>	DamageScores;
		vector<int>	FightScores;
		vector<int>	ProfessionScores;
		vector<int>	Standard;
		vector<int>	TreatScores;

		void CopyFrom(RowData *pRowData)
		{
			this->SceneID=pRowData->SceneID;
			this->MaxTimeScores=pRowData->MaxTimeScores;
			this->Time=pRowData->Time;
			this->MaxSurviveScores=pRowData->MaxSurviveScores;
			this->DeadScores=pRowData->DeadScores;
			this->DamageScores=pRowData->DamageScores;
			this->FightScores=pRowData->FightScores;
			this->ProfessionScores=pRowData->ProfessionScores;
			this->Standard=pRowData->Standard;
			this->TreatScores=pRowData->TreatScores;

		}
	};

	std::unordered_map<int,int> SceneID_Index;


    RowData *GetBySceneID(const int &key)
    {
        auto i = SceneID_Index.find(key);
        return (i == SceneID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SceneID", "MaxTimeScores", "Time", "MaxSurviveScores", "DeadScores", "DamageScores", "FightScores", "ProfessionScores", "Standard", "TreatScores",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(NestScoresTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySceneID(pSrcRow->SceneID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SceneID_Index[pNewRow->SceneID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.MaxTimeScores)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Time)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.MaxSurviveScores)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.DeadScores)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.DamageScores)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.FightScores)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.ProfessionScores)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Standard)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.TreatScores)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SceneID_Index[row.SceneID] = Table.size()-1;


		return true;
	}

	~NestScoresTable ()
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

		SceneID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif