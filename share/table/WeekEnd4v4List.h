#ifndef __WeekEnd4v4List_H__
#define __WeekEnd4v4List_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/17 16:13:23


class WeekEnd4v4List : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ID;
		uint	Index;
		vector<Sequence<uint, 2>>	DropItems;
		string	Name;
		string	Rule;
		uint	SceneID;
		string	TexturePath;
		uint	ReviveSeconds;
		uint	Ratio;
		vector<Sequence<uint, 2>>	RankPoint;
		vector<Sequence<uint, 2>>	LoseDrop;
		uint	GoalScore;
		uint	MaxTime;
		uint	RewardTimes;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Index=pRowData->Index;
			this->DropItems=pRowData->DropItems;
			this->Name=pRowData->Name;
			this->Rule=pRowData->Rule;
			this->SceneID=pRowData->SceneID;
			this->TexturePath=pRowData->TexturePath;
			this->ReviveSeconds=pRowData->ReviveSeconds;
			this->Ratio=pRowData->Ratio;
			this->RankPoint=pRowData->RankPoint;
			this->LoseDrop=pRowData->LoseDrop;
			this->GoalScore=pRowData->GoalScore;
			this->MaxTime=pRowData->MaxTime;
			this->RewardTimes=pRowData->RewardTimes;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "Index", "DropItems", "Name", "Rule", "SceneID", "TexturePath", "ReviveSeconds", "Ratio", "RankPoint", "LoseDrop", "GoalScore", "MaxTime", "RewardTimes",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(WeekEnd4v4List  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Index)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.DropItems)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Rule)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.TexturePath)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.ReviveSeconds)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Ratio)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.RankPoint)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.LoseDrop)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.GoalScore)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.MaxTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.RewardTimes)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~WeekEnd4v4List ()
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