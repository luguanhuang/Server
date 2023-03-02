#ifndef __LevelSeal_H__
#define __LevelSeal_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class LevelSeal : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	Type;
		uint	BeginLevel;
		uint	EndLevel;
		float	ExtraBuff;

		void CopyFrom(RowData *pRowData)
		{
			this->Type=pRowData->Type;
			this->BeginLevel=pRowData->BeginLevel;
			this->EndLevel=pRowData->EndLevel;
			this->ExtraBuff=pRowData->ExtraBuff;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Type", "BeginLevel", "EndLevel", "ExtraBuff",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(LevelSeal  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.BeginLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.EndLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.ExtraBuff)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~LevelSeal ()
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