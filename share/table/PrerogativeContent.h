#ifndef __PrerogativeContent_H__
#define __PrerogativeContent_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/21 14:00:16


class PrerogativeContent : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ID;
		uint	Type;
		string	Content;
		uint	Normal;
		string	Icon;
		Sequence<uint, 2>	Item;
		string	Name;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->Type=pRowData->Type;
			this->Content=pRowData->Content;
			this->Normal=pRowData->Normal;
			this->Icon=pRowData->Icon;
			this->Item=pRowData->Item;
			this->Name=pRowData->Name;

		}
	};

	std::unordered_map<uint,int> ID_Index;


    RowData *GetByID(const uint &key)
    {
        auto i = ID_Index.find(key);
        return (i == ID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "Type", "Content", "Normal", "Icon", "Item", "Name",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PrerogativeContent  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Content)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Normal)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Item)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Name)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~PrerogativeContent ()
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