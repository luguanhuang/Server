﻿#ifndef __SpectateLevelRewardConfig_H__
#define __SpectateLevelRewardConfig_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class SpectateLevelRewardConfig : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	SceneType;
		vector<int>	DataConfig;

		void CopyFrom(RowData *pRowData)
		{
			this->SceneType=pRowData->SceneType;
			this->DataConfig=pRowData->DataConfig;

		}
	};

	std::unordered_map<int,int> SceneType_Index;


    RowData *GetBySceneType(const int &key)
    {
        auto i = SceneType_Index.find(key);
        return (i == SceneType_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SceneType", "DataConfig",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SpectateLevelRewardConfig  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.DataConfig)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SceneType_Index[row.SceneType] = Table.size()-1;


		return true;
	}

	~SpectateLevelRewardConfig ()
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