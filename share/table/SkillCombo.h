#ifndef __SkillCombo_H__
#define __SkillCombo_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class SkillCombo : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	skillname;
		string	nextskill0;
		string	nextskill1;
		string	nextskill2;
		string	nextskill3;
		string	nextskill4;

		void CopyFrom(RowData *pRowData)
		{
			this->skillname=pRowData->skillname;
			this->nextskill0=pRowData->nextskill0;
			this->nextskill1=pRowData->nextskill1;
			this->nextskill2=pRowData->nextskill2;
			this->nextskill3=pRowData->nextskill3;
			this->nextskill4=pRowData->nextskill4;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "skillname", "nextskill0", "nextskill1", "nextskill2", "nextskill3", "nextskill4",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SkillCombo  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.skillname)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.nextskill0)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.nextskill1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.nextskill2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.nextskill3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.nextskill4)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~SkillCombo ()
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