#ifndef __XQTEStatusTable_H__
#define __XQTEStatusTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class XQTEStatusTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	Name;
		uint	Value;

		void CopyFrom(RowData *pRowData)
		{
			this->Name=pRowData->Name;
			this->Value=pRowData->Value;

		}
	};

	std::unordered_map<uint,int> Value_Index;


    RowData *GetByValue(const uint &key)
    {
        auto i = Value_Index.find(key);
        return (i == Value_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Name", "Value",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(XQTEStatusTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByValue(pSrcRow->Value);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                Value_Index[pNewRow->Value]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Value)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Value_Index[row.Value] = Table.size()-1;


		return true;
	}

	~XQTEStatusTable ()
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

		Value_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif