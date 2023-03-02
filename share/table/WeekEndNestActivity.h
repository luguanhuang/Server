#ifndef __WeekEndNestActivity_H__
#define __WeekEndNestActivity_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/8/24 20:34:18


class WeekEndNestActivity : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		Sequence<uint, 2>	OpenSvrDay;
		uint	ParentTaskId;
		string	BgTexName;

		void CopyFrom(RowData *pRowData)
		{
			this->OpenSvrDay=pRowData->OpenSvrDay;
			this->ParentTaskId=pRowData->ParentTaskId;
			this->BgTexName=pRowData->BgTexName;

		}
	};

	std::unordered_map<uint,int> ParentTaskId_Index;


    RowData *GetByParentTaskId(const uint &key)
    {
        auto i = ParentTaskId_Index.find(key);
        return (i == ParentTaskId_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "OpenSvrDay", "ParentTaskId", "BgTexName",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(WeekEndNestActivity  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByParentTaskId(pSrcRow->ParentTaskId);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ParentTaskId_Index[pNewRow->ParentTaskId]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.OpenSvrDay)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ParentTaskId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.BgTexName)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ParentTaskId_Index[row.ParentTaskId] = Table.size()-1;


		return true;
	}

	~WeekEndNestActivity ()
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

		ParentTaskId_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif