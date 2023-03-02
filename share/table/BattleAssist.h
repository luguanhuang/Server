#ifndef __BattleAssist_H__
#define __BattleAssist_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class BattleAssist : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SceneId;
		Sequence<double, 2>	Time;
		double	Revive;
		vector<Sequence<double, 2>>	Hp;
		vector<Sequence<double, 2>>	PPT1;
		vector<Sequence<double, 2>>	PPT2;
		Sequence<double, 2>	Stage;

		void CopyFrom(RowData *pRowData)
		{
			this->SceneId=pRowData->SceneId;
			this->Time=pRowData->Time;
			this->Revive=pRowData->Revive;
			this->Hp=pRowData->Hp;
			this->PPT1=pRowData->PPT1;
			this->PPT2=pRowData->PPT2;
			this->Stage=pRowData->Stage;

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
		const char *Headers[] = { "SceneId", "Time", "Revive", "Hp", "PPT1", "PPT2", "Stage",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(BattleAssist  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[2]], row.Revive)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Hp)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.PPT1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.PPT2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Stage)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SceneId_Index[row.SceneId] = Table.size()-1;


		return true;
	}

	~BattleAssist ()
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