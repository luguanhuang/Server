﻿#ifndef __MainSubstance_H__
#define __MainSubstance_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/8/1 18:30:03


class MainSubstance : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	SystemID;
		vector<string>	Text;
		vector<string>	Icon;
		vector<string>	Effect;

		void CopyFrom(RowData *pRowData)
		{
			this->SystemID=pRowData->SystemID;
			this->Text=pRowData->Text;
			this->Icon=pRowData->Icon;
			this->Effect=pRowData->Effect;

		}
	};

	std::unordered_map<int,int> SystemID_Index;


    RowData *GetBySystemID(const int &key)
    {
        auto i = SystemID_Index.find(key);
        return (i == SystemID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SystemID", "Text", "Icon", "Effect",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(MainSubstance  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySystemID(pSrcRow->SystemID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SystemID_Index[pNewRow->SystemID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.SystemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Text)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Effect)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SystemID_Index[row.SystemID] = Table.size()-1;


		return true;
	}

	~MainSubstance ()
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

		SystemID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif