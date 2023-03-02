#ifndef __ProfessionGrowthPercentTable_H__
#define __ProfessionGrowthPercentTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class ProfessionGrowthPercentTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ProfessionID;
		int	Strength;
		int	Agility;
		int	Intelligence;
		int	Vitality;
		string	ProfessionName;

		void CopyFrom(RowData *pRowData)
		{
			this->ProfessionID=pRowData->ProfessionID;
			this->Strength=pRowData->Strength;
			this->Agility=pRowData->Agility;
			this->Intelligence=pRowData->Intelligence;
			this->Vitality=pRowData->Vitality;
			this->ProfessionName=pRowData->ProfessionName;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ProfessionID", "Strength", "Agility", "Intelligence", "Vitality", "ProfessionName",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ProfessionGrowthPercentTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ProfessionID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Strength)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Agility)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Intelligence)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Vitality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.ProfessionName)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~ProfessionGrowthPercentTable ()
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