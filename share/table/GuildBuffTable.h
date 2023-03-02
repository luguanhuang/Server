#ifndef __GuildBuffTable_H__
#define __GuildBuffTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildBuffTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		string	name;
		uint	type;
		vector<uint>	param;
		uint	time;
		uint	self;
		uint	itemid;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->name=pRowData->name;
			this->type=pRowData->type;
			this->param=pRowData->param;
			this->time=pRowData->time;
			this->self=pRowData->self;
			this->itemid=pRowData->itemid;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "name", "type", "param", "time", "self", "itemid",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildBuffTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[2]], row.type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.param)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.time)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.self)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.itemid)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GuildBuffTable ()
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