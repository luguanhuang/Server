#ifndef __NpcFeelingSpecial_H__
#define __NpcFeelingSpecial_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/14 16:16:18


class NpcFeelingSpecial : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	npcId;
		vector<Sequence<uint, 2>>	enhanceReduce;

		void CopyFrom(RowData *pRowData)
		{
			this->npcId=pRowData->npcId;
			this->enhanceReduce=pRowData->enhanceReduce;

		}
	};

	std::unordered_map<uint,int> npcId_Index;


    RowData *GetBynpcId(const uint &key)
    {
        auto i = npcId_Index.find(key);
        return (i == npcId_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "npcId", "enhanceReduce",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(NpcFeelingSpecial  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBynpcId(pSrcRow->npcId);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                npcId_Index[pNewRow->npcId]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.npcId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.enhanceReduce)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		npcId_Index[row.npcId] = Table.size()-1;


		return true;
	}

	~NpcFeelingSpecial ()
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

		npcId_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif