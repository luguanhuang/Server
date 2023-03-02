#ifndef __SceneConsumeTable_H__
#define __SceneConsumeTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class SceneConsumeTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SceneType;
		int	SceneCoolDown;
		int	SceneCost;
		int	SceneCount;

		void CopyFrom(RowData *pRowData)
		{
			this->SceneType=pRowData->SceneType;
			this->SceneCoolDown=pRowData->SceneCoolDown;
			this->SceneCost=pRowData->SceneCost;
			this->SceneCount=pRowData->SceneCount;

		}
	};

	std::unordered_map<uint,int> SceneType_Index;


    RowData *GetBySceneType(const uint &key)
    {
        auto i = SceneType_Index.find(key);
        return (i == SceneType_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SceneType", "SceneCoolDown", "SceneCost", "SceneCount",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SceneConsumeTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySceneType(pSrcRow->SceneType);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SceneType_Index[pNewRow->SceneType]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.SceneType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SceneCoolDown)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SceneCost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SceneCount)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SceneType_Index[row.SceneType] = Table.size()-1;


		return true;
	}

	~SceneConsumeTable ()
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

		SceneType_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif