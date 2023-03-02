#ifndef __PkOpenTable_H__
#define __PkOpenTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PkOpenTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		Sequence<uint, 3>	date;
		Sequence<uint, 3>	timebegin;
		Sequence<uint, 3>	timeend;
		uint	id;

		void CopyFrom(RowData *pRowData)
		{
			this->date=pRowData->date;
			this->timebegin=pRowData->timebegin;
			this->timeend=pRowData->timeend;
			this->id=pRowData->id;

		}
	};

	std::unordered_map<uint,int> id_Index;


    RowData *GetByid(const uint &key)
    {
        auto i = id_Index.find(key);
        return (i == id_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "date", "timebegin", "timeend", "id",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PkOpenTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByid(pSrcRow->id);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                id_Index[pNewRow->id]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.date)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.timebegin)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.timeend)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.id)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		id_Index[row.id] = Table.size()-1;


		return true;
	}

	~PkOpenTable ()
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

		id_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif