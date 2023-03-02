#ifndef __SkyCraftRewardTable_H__
#define __SkyCraftRewardTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/12 20:28:58


class SkyCraftRewardTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	type;
		vector<Sequence<uint, 2>>	reward;
		uint	mailid;

		void CopyFrom(RowData *pRowData)
		{
			this->type=pRowData->type;
			this->reward=pRowData->reward;
			this->mailid=pRowData->mailid;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "type", "reward", "mailid",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SkyCraftRewardTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.mailid)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~SkyCraftRewardTable ()
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