#ifndef __DargonReward_H__
#define __DargonReward_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/13 15:00:51


class DargonReward : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ID;
		string	Achievement;
		string	Explanation;
		vector<int>	CompleteValue;
		vector<Sequence<int, 2>>	Reward;
		string	ICON;
		string	DesignationName;
		int	SortID;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Achievement=pRowData->Achievement;
			this->Explanation=pRowData->Explanation;
			this->CompleteValue=pRowData->CompleteValue;
			this->Reward=pRowData->Reward;
			this->ICON=pRowData->ICON;
			this->DesignationName=pRowData->DesignationName;
			this->SortID=pRowData->SortID;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "Achievement", "Explanation", "CompleteValue", "Reward", "ICON", "DesignationName", "SortID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DargonReward  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.Achievement)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Explanation)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.CompleteValue)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.ICON)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.DesignationName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.SortID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~DargonReward ()
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