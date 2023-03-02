#ifndef __DoodadItemSkillTable_H__
#define __DoodadItemSkillTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/23 16:24:18


class DoodadItemSkillTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	sceneType;
		uint	itemId;
		uint	overlap;

		void CopyFrom(RowData *pRowData)
		{
			this->sceneType=pRowData->sceneType;
			this->itemId=pRowData->itemId;
			this->overlap=pRowData->overlap;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "sceneType", "itemId", "overlap",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DoodadItemSkillTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.sceneType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.itemId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.overlap)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~DoodadItemSkillTable ()
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