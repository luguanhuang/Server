#ifndef __GoalAwards_H__
#define __GoalAwards_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/25 22:55:10


class GoalAwards : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	GoalAwardsID;
		uint	AwardsIndex;
		uint	GKID;
		double	AwardsValue;
		vector<Sequence<uint, 2>>	Awards;
		uint	TitleID;
		uint	Type;
		string	Description;
		string	Explanation;
		uint	ShowLevel;
		uint	ShowType;

		void CopyFrom(RowData *pRowData)
		{
			this->GoalAwardsID=pRowData->GoalAwardsID;
			this->AwardsIndex=pRowData->AwardsIndex;
			this->GKID=pRowData->GKID;
			this->AwardsValue=pRowData->AwardsValue;
			this->Awards=pRowData->Awards;
			this->TitleID=pRowData->TitleID;
			this->Type=pRowData->Type;
			this->Description=pRowData->Description;
			this->Explanation=pRowData->Explanation;
			this->ShowLevel=pRowData->ShowLevel;
			this->ShowType=pRowData->ShowType;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "GoalAwardsID", "AwardsIndex", "GKID", "AwardsValue", "Awards", "TitleID", "Type", "Description", "Explanation", "ShowLevel", "ShowType",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GoalAwards  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.GoalAwardsID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.AwardsIndex)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.GKID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.AwardsValue)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Awards)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.TitleID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Description)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Explanation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.ShowLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.ShowType)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GoalAwards ()
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