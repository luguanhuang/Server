#ifndef __FightGroupTable_H__
#define __FightGroupTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class FightGroupTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	group;
		string	enemy;
		string	role;
		string	neutral;
		string	hostility;
		string	enemygod;
		string	rolegod;
		string	other;

		void CopyFrom(RowData *pRowData)
		{
			this->group=pRowData->group;
			this->enemy=pRowData->enemy;
			this->role=pRowData->role;
			this->neutral=pRowData->neutral;
			this->hostility=pRowData->hostility;
			this->enemygod=pRowData->enemygod;
			this->rolegod=pRowData->rolegod;
			this->other=pRowData->other;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "group", "enemy", "role", "neutral", "hostility", "enemygod", "rolegod", "other",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FightGroupTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.group)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.enemy)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.role)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.neutral)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.hostility)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.enemygod)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.rolegod)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.other)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~FightGroupTable ()
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