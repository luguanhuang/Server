﻿#ifndef __PartnerTable_H__
#define __PartnerTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PartnerTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	level;
		uint	degree;
		Sequence<int, 2>	buf;

		void CopyFrom(RowData *pRowData)
		{
			this->level=pRowData->level;
			this->degree=pRowData->degree;
			this->buf=pRowData->buf;

		}
	};

	std::unordered_map<uint,int> level_Index;


    RowData *GetBylevel(const uint &key)
    {
        auto i = level_Index.find(key);
        return (i == level_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "level", "degree", "buf",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PartnerTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBylevel(pSrcRow->level);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                level_Index[pNewRow->level]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.degree)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.buf)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		level_Index[row.level] = Table.size()-1;


		return true;
	}

	~PartnerTable ()
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

		level_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif