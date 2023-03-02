#ifndef __EmblemTable_H__
#define __EmblemTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class EmblemTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EmblemID;
		vector<Sequence<uint, 2>>	EmblemAttribute;
		uint	ExpOutput;

		void CopyFrom(RowData *pRowData)
		{
			this->EmblemID=pRowData->EmblemID;
			this->EmblemAttribute=pRowData->EmblemAttribute;
			this->ExpOutput=pRowData->ExpOutput;

		}
	};

	std::unordered_map<uint,int> EmblemID_Index;


    RowData *GetByEmblemID(const uint &key)
    {
        auto i = EmblemID_Index.find(key);
        return (i == EmblemID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EmblemID", "EmblemAttribute", "ExpOutput",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EmblemTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByEmblemID(pSrcRow->EmblemID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                EmblemID_Index[pNewRow->EmblemID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.EmblemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.EmblemAttribute)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ExpOutput)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		EmblemID_Index[row.EmblemID] = Table.size()-1;


		return true;
	}

	~EmblemTable ()
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

		EmblemID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif