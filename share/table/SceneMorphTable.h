#ifndef __SceneMorphTable_H__
#define __SceneMorphTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/22 21:37:42


class SceneMorphTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SceneID;
		vector<Sequence<int, 2>>	MorphCondition;
		uint	MorphTime;
		vector<int>	DropList;

		void CopyFrom(RowData *pRowData)
		{
			this->SceneID=pRowData->SceneID;
			this->MorphCondition=pRowData->MorphCondition;
			this->MorphTime=pRowData->MorphTime;
			this->DropList=pRowData->DropList;

		}
	};

	std::unordered_map<uint,int> SceneID_Index;


    RowData *GetBySceneID(const uint &key)
    {
        auto i = SceneID_Index.find(key);
        return (i == SceneID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SceneID", "MorphCondition", "MorphTime", "DropList",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SceneMorphTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySceneID(pSrcRow->SceneID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SceneID_Index[pNewRow->SceneID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.SceneID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.MorphCondition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.MorphTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.DropList)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SceneID_Index[row.SceneID] = Table.size()-1;


		return true;
	}

	~SceneMorphTable ()
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

		SceneID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif