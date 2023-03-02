#ifndef __GuildSkill_H__
#define __GuildSkill_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildSkill : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	skillid;
		string	name;
		uint	level;
		Sequence<uint, 2>	need;
		uint	rexp;
		uint	glevel;
		uint	attribute;

		void CopyFrom(RowData *pRowData)
		{
			this->skillid=pRowData->skillid;
			this->name=pRowData->name;
			this->level=pRowData->level;
			this->need=pRowData->need;
			this->rexp=pRowData->rexp;
			this->glevel=pRowData->glevel;
			this->attribute=pRowData->attribute;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "skillid", "name", "level", "need", "rexp", "glevel", "attribute",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildSkill  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.skillid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.need)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.rexp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.glevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.attribute)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GuildSkill ()
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