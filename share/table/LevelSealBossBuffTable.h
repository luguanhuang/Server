#ifndef __LevelSealBossBuffTable_H__
#define __LevelSealBossBuffTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class LevelSealBossBuffTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Type;
		uint	Days;
		uint	Buff;
		uint	SelfIncTime;
		uint	SelfIncCount;

		void CopyFrom(RowData *pRowData)
		{
			this->Type=pRowData->Type;
			this->Days=pRowData->Days;
			this->Buff=pRowData->Buff;
			this->SelfIncTime=pRowData->SelfIncTime;
			this->SelfIncCount=pRowData->SelfIncCount;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Type", "Days", "Buff", "SelfIncTime", "SelfIncCount",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(LevelSealBossBuffTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Days)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Buff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SelfIncTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.SelfIncCount)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~LevelSealBossBuffTable ()
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