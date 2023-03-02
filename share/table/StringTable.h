#ifndef __StringTable_H__
#define __StringTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class StringTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	Enum;
		string	Text;

		void CopyFrom(RowData *pRowData)
		{
			this->Enum=pRowData->Enum;
			this->Text=pRowData->Text;

		}
	};

	std::unordered_map<string,int> Enum_Index;


    RowData *GetByEnum(const string &key)
    {
        auto i = Enum_Index.find(key);
        return (i == Enum_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "Enum", "Text",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(StringTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByEnum(pSrcRow->Enum);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                Enum_Index[pNewRow->Enum]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.Enum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Text)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		Enum_Index[row.Enum] = Table.size()-1;


		return true;
	}

	~StringTable ()
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

		Enum_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif