#ifndef __GuildInheritTable_H__
#define __GuildInheritTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class GuildInheritTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		Sequence<uint, 2>	Level;
		uint	StudentExp;
		uint	TeacherExp;

		void CopyFrom(RowData *pRowData)
		{
			this->Level=pRowData->Level;
			this->StudentExp=pRowData->StudentExp;
			this->TeacherExp=pRowData->TeacherExp;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Level", "StudentExp", "TeacherExp",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(GuildInheritTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.StudentExp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.TeacherExp)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~GuildInheritTable ()
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