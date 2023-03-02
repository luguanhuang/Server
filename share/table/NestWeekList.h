﻿#ifndef __NestWeekList_H__
#define __NestWeekList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class NestWeekList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	SealType;
		vector<int>	ExpID;
		int	WeekCount;

		void CopyFrom(RowData *pRowData)
		{
			this->SealType=pRowData->SealType;
			this->ExpID=pRowData->ExpID;
			this->WeekCount=pRowData->WeekCount;

		}
	};

	std::unordered_map<int,int> SealType_Index;


    RowData *GetBySealType(const int &key)
    {
        auto i = SealType_Index.find(key);
        return (i == SealType_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SealType", "ExpID", "WeekCount",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(NestWeekList  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySealType(pSrcRow->SealType);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SealType_Index[pNewRow->SealType]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.SealType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ExpID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.WeekCount)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SealType_Index[row.SealType] = Table.size()-1;


		return true;
	}

	~NestWeekList ()
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

		SealType_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif