#ifndef __MobaMobTable_H__
#define __MobaMobTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/7 20:15:30


class MobaMobTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	level;
		vector<Sequence<uint, 2>>	moba;
		vector<Sequence<uint, 2>>	mobb;

		void CopyFrom(RowData *pRowData)
		{
			this->level=pRowData->level;
			this->moba=pRowData->moba;
			this->mobb=pRowData->mobb;

		}
	};




	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "level", "moba", "mobb",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(MobaMobTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.moba)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.mobb)) return false; ++m_ColNum;

		
		Table.push_back(pRow);



		return true;
	}

	~MobaMobTable ()
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