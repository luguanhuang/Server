#ifndef __GuildSkillTable_H__
#define __GuildSkillTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildSkillTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	skillid;
		string	name;
		uint	level;
		vector<Sequence<uint, 2>>	need;
		uint	rexp;
		uint	glevel;
		vector<Sequence<uint, 2>>	attribute;
		string	icon;
		string	atlas;
		string	currentLevelDescription;
		uint	roleLevel;
		vector<uint>	profecssion;

		void CopyFrom(RowData *pRowData)
		{
			this->skillid=pRowData->skillid;
			this->name=pRowData->name;
			this->level=pRowData->level;
			this->need=pRowData->need;
			this->rexp=pRowData->rexp;
			this->glevel=pRowData->glevel;
			this->attribute=pRowData->attribute;
			this->icon=pRowData->icon;
			this->atlas=pRowData->atlas;
			this->currentLevelDescription=pRowData->currentLevelDescription;
			this->roleLevel=pRowData->roleLevel;
			this->profecssion=pRowData->profecssion;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "skillid", "name", "level", "need", "rexp", "glevel", "attribute", "icon", "atlas", "currentLevelDescription", "roleLevel", "profecssion",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildSkillTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[7]], row.icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.atlas)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.currentLevelDescription)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.roleLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.profecssion)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GuildSkillTable ()
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