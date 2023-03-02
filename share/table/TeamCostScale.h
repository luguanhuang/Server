#ifndef __TeamCostScale_H__
#define __TeamCostScale_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/14 20:30:13


class TeamCostScale : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	seallevel;
		uint	level;

		void CopyFrom(RowData *pRowData)
		{
			this->seallevel=pRowData->seallevel;
			this->level=pRowData->level;

		}
	};

	std::unordered_map<uint,int> seallevel_Index;


    RowData *GetByseallevel(const uint &key)
    {
        auto i = seallevel_Index.find(key);
        return (i == seallevel_Index.end()) ? NULL : Table[i->second];
    }


	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "seallevel", "level",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(TeamCostScale  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByseallevel(pSrcRow->seallevel);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                seallevel_Index[pNewRow->seallevel]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.seallevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.level)) return false; ++m_ColNum;


		Table.push_back(pRow);

		seallevel_Index[row.seallevel] = Table.size()-1;


		return true;
	}

	~TeamCostScale ()
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

		seallevel_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif