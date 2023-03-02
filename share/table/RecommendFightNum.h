#ifndef __RecommendFightNum_H__
#define __RecommendFightNum_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class RecommendFightNum : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Level;
		uint	SystemID;
		uint	Total;
		uint	Point;

		void CopyFrom(RowData *pRowData)
		{
			this->Level=pRowData->Level;
			this->SystemID=pRowData->SystemID;
			this->Total=pRowData->Total;
			this->Point=pRowData->Point;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Level", "SystemID", "Total", "Point",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(RecommendFightNum  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.SystemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Total)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Point)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~RecommendFightNum ()
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