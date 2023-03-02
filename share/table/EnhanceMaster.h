﻿#ifndef __EnhanceMaster_H__
#define __EnhanceMaster_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class EnhanceMaster : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ProfessionId;
		uint	TotalEnhanceLevel;
		vector<Sequence<uint, 2>>	Attribute;

		void CopyFrom(RowData *pRowData)
		{
			this->ProfessionId=pRowData->ProfessionId;
			this->TotalEnhanceLevel=pRowData->TotalEnhanceLevel;
			this->Attribute=pRowData->Attribute;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ProfessionId", "TotalEnhanceLevel", "Attribute",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EnhanceMaster  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ProfessionId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.TotalEnhanceLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Attribute)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~EnhanceMaster ()
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