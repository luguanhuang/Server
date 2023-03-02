#ifndef __GuildPermissionTable_H__
#define __GuildPermissionTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildPermissionTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	GuildID;
		int	GPOS_LEADER;
		int	GPOS_VICELEADER;
		int	GPOS_OFFICER;
		int	GPOS_ELITEMEMBER;
		int	GPOS_MEMBER;

		void CopyFrom(RowData *pRowData)
		{
			this->GuildID=pRowData->GuildID;
			this->GPOS_LEADER=pRowData->GPOS_LEADER;
			this->GPOS_VICELEADER=pRowData->GPOS_VICELEADER;
			this->GPOS_OFFICER=pRowData->GPOS_OFFICER;
			this->GPOS_ELITEMEMBER=pRowData->GPOS_ELITEMEMBER;
			this->GPOS_MEMBER=pRowData->GPOS_MEMBER;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "GuildID", "GPOS_LEADER", "GPOS_VICELEADER", "GPOS_OFFICER", "GPOS_ELITEMEMBER", "GPOS_MEMBER",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildPermissionTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.GuildID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.GPOS_LEADER)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.GPOS_VICELEADER)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.GPOS_OFFICER)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.GPOS_ELITEMEMBER)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.GPOS_MEMBER)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GuildPermissionTable ()
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