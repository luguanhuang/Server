#ifndef __KillMonsterScoreTable_H__
#define __KillMonsterScoreTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class KillMonsterScoreTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	sceneID;
		vector<Sequence<int, 2>>	monsterScore;

		void CopyFrom(RowData *pRowData)
		{
			this->sceneID=pRowData->sceneID;
			this->monsterScore=pRowData->monsterScore;

		}
	};

	std::unordered_map<uint,int> sceneID_Index;


    RowData *GetBysceneID(const uint &key)
    {
        auto i = sceneID_Index.find(key);
        return (i == sceneID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "sceneID", "monsterScore",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(KillMonsterScoreTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBysceneID(pSrcRow->sceneID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                sceneID_Index[pNewRow->sceneID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.sceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.monsterScore)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		sceneID_Index[row.sceneID] = Table.size()-1;


		return true;
	}

	~KillMonsterScoreTable ()
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

		sceneID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif