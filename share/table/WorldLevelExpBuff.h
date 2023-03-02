#ifndef __WorldLevelExpBuff_H__
#define __WorldLevelExpBuff_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/31 15:57:13


class WorldLevelExpBuff : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	WorldLevel;
		Sequence<uint, 2>	Level;
		double	ExpBuff;
		double	BackExpBuff;
		uint	BackflowLevel;
		Sequence<uint, 2>	BackBattleBuff;

		void CopyFrom(RowData *pRowData)
		{
			this->WorldLevel=pRowData->WorldLevel;
			this->Level=pRowData->Level;
			this->ExpBuff=pRowData->ExpBuff;
			this->BackExpBuff=pRowData->BackExpBuff;
			this->BackflowLevel=pRowData->BackflowLevel;
			this->BackBattleBuff=pRowData->BackBattleBuff;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "WorldLevel", "Level", "ExpBuff", "BackExpBuff", "BackflowLevel", "BackBattleBuff",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(WorldLevelExpBuff  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.WorldLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ExpBuff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.BackExpBuff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.BackflowLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.BackBattleBuff)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~WorldLevelExpBuff ()
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