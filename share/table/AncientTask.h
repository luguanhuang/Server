#ifndef __AncientTask_H__
#define __AncientTask_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/7/27 11:42:13


class AncientTask : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ID;
		string	title;
		string	content;
		string	time;
		vector<Sequence<uint, 2>>	rewards;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->title=pRowData->title;
			this->content=pRowData->content;
			this->time=pRowData->time;
			this->rewards=pRowData->rewards;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "title", "content", "time", "rewards",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(AncientTask  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.title)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.content)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.time)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.rewards)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~AncientTask ()
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