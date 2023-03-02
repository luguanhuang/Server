#ifndef __GaoJiGuTShowTable_H__
#define __GaoJiGuTShowTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GaoJiGuTShowTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	score;
		vector<Sequence<int, 2>>	output;
		int	exp;

		void CopyFrom(RowData *pRowData)
		{
			this->score=pRowData->score;
			this->output=pRowData->output;
			this->exp=pRowData->exp;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "score", "output", "exp",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GaoJiGuTShowTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.score)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.output)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.exp)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GaoJiGuTShowTable ()
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