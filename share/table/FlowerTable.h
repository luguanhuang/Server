#ifndef __FlowerTable_H__
#define __FlowerTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class FlowerTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	count;
		Sequence<uint, 2>	cost;
		vector<Sequence<uint, 2>>	reward;

		void CopyFrom(RowData *pRowData)
		{
			this->count=pRowData->count;
			this->cost=pRowData->cost;
			this->reward=pRowData->reward;

		}
	};

	std::unordered_map<uint,int> count_Index;


    RowData *GetBycount(const uint &key)
    {
        auto i = count_Index.find(key);
        return (i == count_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "count", "cost", "reward",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FlowerTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBycount(pSrcRow->count);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                count_Index[pNewRow->count]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.count)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.cost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.reward)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		count_Index[row.count] = Table.size()-1;


		return true;
	}

	~FlowerTable ()
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

		count_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif