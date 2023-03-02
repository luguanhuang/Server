#ifndef __RenameList_H__
#define __RenameList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class RenameList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	id;
		int	cost;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->cost=pRowData->cost;

		}
	};

	std::unordered_map<int,int> id_Index;


    RowData *GetByid(const int &key)
    {
        auto i = id_Index.find(key);
        return (i == id_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "cost",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(RenameList  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.id)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.cost)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		id_Index[row.id] = Table.size()-1;


		return true;
	}

	~RenameList ()
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