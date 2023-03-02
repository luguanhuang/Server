#ifndef __DailyTaskLuck_H__
#define __DailyTaskLuck_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/30 14:08:48


class DailyTaskLuck : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		string	name;
		vector<Sequence<uint, 2>>	prob;
		uint	getProb;
		uint	backflowProb;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->name=pRowData->name;
			this->prob=pRowData->prob;
			this->getProb=pRowData->getProb;
			this->backflowProb=pRowData->backflowProb;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "name", "prob", "getProb", "backflowProb",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DailyTaskLuck  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.id)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.prob)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.getProb)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.backflowProb)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~DailyTaskLuck ()
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