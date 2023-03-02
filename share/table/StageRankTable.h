#ifndef __StageRankTable_H__
#define __StageRankTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class StageRankTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	scendid;
		Sequence<uint, 3>	star2;
		Sequence<uint, 3>	star3;

		void CopyFrom(RowData *pRowData)
		{
			this->scendid=pRowData->scendid;
			this->star2=pRowData->star2;
			this->star3=pRowData->star3;

		}
	};

	std::unordered_map<uint,int> scendid_Index;


    RowData *GetByscendid(const uint &key)
    {
        auto i = scendid_Index.find(key);
        return (i == scendid_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "scendid", "star2", "star3",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(StageRankTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByscendid(pSrcRow->scendid);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                scendid_Index[pNewRow->scendid]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.scendid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.star2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.star3)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		scendid_Index[row.scendid] = Table.size()-1;


		return true;
	}

	~StageRankTable ()
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

		scendid_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif