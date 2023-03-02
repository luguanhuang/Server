#ifndef __EmblemLevelTable_H__
#define __EmblemLevelTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class EmblemLevelTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EmblemLevel;
		uint	EXP0;
		uint	EXP1;
		uint	EXP2;
		uint	EXP3;
		uint	EXP4;
		uint	EXP5;
		float	Advance;

		void CopyFrom(RowData *pRowData)
		{
			this->EmblemLevel=pRowData->EmblemLevel;
			this->EXP0=pRowData->EXP0;
			this->EXP1=pRowData->EXP1;
			this->EXP2=pRowData->EXP2;
			this->EXP3=pRowData->EXP3;
			this->EXP4=pRowData->EXP4;
			this->EXP5=pRowData->EXP5;
			this->Advance=pRowData->Advance;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EmblemLevel", "EXP0", "EXP1", "EXP2", "EXP3", "EXP4", "EXP5", "Advance",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EmblemLevelTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.EmblemLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.EXP0)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.EXP1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.EXP2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.EXP3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.EXP4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.EXP5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Advance)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~EmblemLevelTable ()
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