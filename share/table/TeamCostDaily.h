﻿#ifndef __TeamCostDaily_H__
#define __TeamCostDaily_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class TeamCostDaily : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		Sequence<uint, 2>	Level;
		uint	Activity;
		uint	Possibility;
		uint	DropId;
		uint	Time;

		void CopyFrom(RowData *pRowData)
		{
			this->Level=pRowData->Level;
			this->Activity=pRowData->Activity;
			this->Possibility=pRowData->Possibility;
			this->DropId=pRowData->DropId;
			this->Time=pRowData->Time;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Level", "Activity", "Possibility", "DropId", "Time",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(TeamCostDaily  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Activity)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Possibility)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.DropId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Time)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~TeamCostDaily ()
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