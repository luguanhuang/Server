#ifndef __NestListTable_H__
#define __NestListTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/13 21:10:11


class NestListTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	NestID;
		int	Type;
		int	Difficulty;
		vector<Sequence<int, 2>>	FindBackItems;

		void CopyFrom(RowData *pRowData)
		{
			this->NestID=pRowData->NestID;
			this->Type=pRowData->Type;
			this->Difficulty=pRowData->Difficulty;
			this->FindBackItems=pRowData->FindBackItems;

		}
	};

	std::unordered_map<int,int> NestID_Index;


    RowData *GetByNestID(const int &key)
    {
        auto i = NestID_Index.find(key);
        return (i == NestID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "NestID", "Type", "Difficulty", "FindBackItems",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(NestListTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByNestID(pSrcRow->NestID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                NestID_Index[pNewRow->NestID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.NestID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Difficulty)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.FindBackItems)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		NestID_Index[row.NestID] = Table.size()-1;


		return true;
	}

	~NestListTable ()
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

		NestID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif