#ifndef __PeriodDesignation_H__
#define __PeriodDesignation_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/15 17:58:08


class PeriodDesignation : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	DesID;
		Sequence<string, 2>	Time;
		Sequence<uint, 3>	Param;

		void CopyFrom(RowData *pRowData)
		{
			this->DesID=pRowData->DesID;
			this->Time=pRowData->Time;
			this->Param=pRowData->Param;

		}
	};

	std::unordered_map<uint,int> DesID_Index;


    RowData *GetByDesID(const uint &key)
    {
        auto i = DesID_Index.find(key);
        return (i == DesID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "DesID", "Time", "Param",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PeriodDesignation  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByDesID(pSrcRow->DesID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                DesID_Index[pNewRow->DesID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.DesID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Time)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Param)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		DesID_Index[row.DesID] = Table.size()-1;


		return true;
	}

	~PeriodDesignation ()
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

		DesID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif