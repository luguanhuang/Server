#ifndef __NpcFeelingAttr_H__
#define __NpcFeelingAttr_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/15 10:13:16


class NpcFeelingAttr : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	npcId;
		uint	level;
		uint	needExp;
		vector<Sequence<uint, 2>>	Attr;
		string	RelicsDesc;

		void CopyFrom(RowData *pRowData)
		{
			this->npcId=pRowData->npcId;
			this->level=pRowData->level;
			this->needExp=pRowData->needExp;
			this->Attr=pRowData->Attr;
			this->RelicsDesc=pRowData->RelicsDesc;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "npcId", "level", "needExp", "Attr", "RelicsDesc",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(NpcFeelingAttr  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.npcId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.needExp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Attr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.RelicsDesc)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~NpcFeelingAttr ()
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