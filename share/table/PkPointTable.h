#ifndef __PkPointTable_H__
#define __PkPointTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PkPointTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	point;
		vector<Sequence<uint, 2>>	reward;
		int	level;
		int	IconIndex;

		void CopyFrom(RowData *pRowData)
		{
			this->point=pRowData->point;
			this->reward=pRowData->reward;
			this->level=pRowData->level;
			this->IconIndex=pRowData->IconIndex;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "point", "reward", "level", "IconIndex",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PkPointTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.point)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.IconIndex)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~PkPointTable ()
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