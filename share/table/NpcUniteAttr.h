#ifndef __NpcUniteAttr_H__
#define __NpcUniteAttr_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/15 10:15:14


class NpcUniteAttr : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		vector<uint>	npcId;
		uint	level;
		vector<Sequence<uint, 2>>	Attr;
		string	Name;
		string	Icon;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->npcId=pRowData->npcId;
			this->level=pRowData->level;
			this->Attr=pRowData->Attr;
			this->Name=pRowData->Name;
			this->Icon=pRowData->Icon;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "npcId", "level", "Attr", "Name", "Icon",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(NpcUniteAttr  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.npcId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Attr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Icon)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~NpcUniteAttr ()
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