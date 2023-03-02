#ifndef __SmeltingTable_H__
#define __SmeltingTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class SmeltingTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	level;
		uint	quality;
		Sequence<uint, 2>	physicalattack;
		Sequence<uint, 2>	physicaldefence;
		Sequence<uint, 2>	magicattack;
		Sequence<uint, 2>	magicdefence;
		Sequence<uint, 2>	hp;
		vector<Sequence<uint, 2>>	coin;
		Sequence<uint, 2>	transfer;

		void CopyFrom(RowData *pRowData)
		{
			this->level=pRowData->level;
			this->quality=pRowData->quality;
			this->physicalattack=pRowData->physicalattack;
			this->physicaldefence=pRowData->physicaldefence;
			this->magicattack=pRowData->magicattack;
			this->magicdefence=pRowData->magicdefence;
			this->hp=pRowData->hp;
			this->coin=pRowData->coin;
			this->transfer=pRowData->transfer;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "level", "quality", "physicalattack", "physicaldefence", "magicattack", "magicdefence", "hp", "coin", "transfer",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SmeltingTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.quality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.physicalattack)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.physicaldefence)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.magicattack)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.magicdefence)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.hp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.coin)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.transfer)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~SmeltingTable ()
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