#ifndef __SceneTrans_H__
#define __SceneTrans_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/16 11:45:10


class SceneTrans : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SceneId;
		Sequence<string, 2>	Time;
		vector<Sequence<uint, 4>>	TimeCycle;
		uint	Interval;
		vector<Sequence<uint, 3>>	Buff;

		void CopyFrom(RowData *pRowData)
		{
			this->SceneId=pRowData->SceneId;
			this->Time=pRowData->Time;
			this->TimeCycle=pRowData->TimeCycle;
			this->Interval=pRowData->Interval;
			this->Buff=pRowData->Buff;

		}
	};

	std::unordered_map<uint,int> SceneId_Index;


    RowData *GetBySceneId(const uint &key)
    {
        auto i = SceneId_Index.find(key);
        return (i == SceneId_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SceneId", "Time", "TimeCycle", "Interval", "Buff",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SceneTrans  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySceneId(pSrcRow->SceneId);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SceneId_Index[pNewRow->SceneId]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.SceneId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Time)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.TimeCycle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Interval)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Buff)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SceneId_Index[row.SceneId] = Table.size()-1;


		return true;
	}

	~SceneTrans ()
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

		SceneId_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif