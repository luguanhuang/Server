#ifndef __XOptions_H__
#define __XOptions_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class XOptions : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ID;
		vector<int>	Classify;
		int	Sort;
		string	Text;
		int	Type;
		string	Range;
		string	Default;
		vector<string>	OptionText;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Classify=pRowData->Classify;
			this->Sort=pRowData->Sort;
			this->Text=pRowData->Text;
			this->Type=pRowData->Type;
			this->Range=pRowData->Range;
			this->Default=pRowData->Default;
			this->OptionText=pRowData->OptionText;

		}
	};

	std::unordered_map<int,int> ID_Index;


    RowData *GetByID(const int &key)
    {
        auto i = ID_Index.find(key);
        return (i == ID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "Classify", "Sort", "Text", "Type", "Range", "Default", "OptionText",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(XOptions  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByID(pSrcRow->ID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ID_Index[pNewRow->ID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Classify)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Sort)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Text)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Range)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Default)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.OptionText)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~XOptions ()
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

		ID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif