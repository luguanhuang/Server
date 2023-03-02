#ifndef __HeroBattleMapCenter_H__
#define __HeroBattleMapCenter_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class HeroBattleMapCenter : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SceneID;
		uint	CenterType;
		Sequence<float, 3>	Center;
		vector<float>	Param;
		float	ClientFxScalse;
		vector<string>	OccupantFx;
		vector<string>	MiniMapFx;
		vector<string>	OccSuccessFx;
		vector<string>	OccWinFx;
		string	MVPCutScene;
		vector<float>	MVPPos;

		void CopyFrom(RowData *pRowData)
		{
			this->SceneID=pRowData->SceneID;
			this->CenterType=pRowData->CenterType;
			this->Center=pRowData->Center;
			this->Param=pRowData->Param;
			this->ClientFxScalse=pRowData->ClientFxScalse;
			this->OccupantFx=pRowData->OccupantFx;
			this->MiniMapFx=pRowData->MiniMapFx;
			this->OccSuccessFx=pRowData->OccSuccessFx;
			this->OccWinFx=pRowData->OccWinFx;
			this->MVPCutScene=pRowData->MVPCutScene;
			this->MVPPos=pRowData->MVPPos;

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
		const char *Headers[] = { "SceneID", "CenterType", "Center", "Param", "ClientFxScalse", "OccupantFx", "MiniMapFx", "OccSuccessFx", "OccWinFx", "MVPCutScene", "MVPPos",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(HeroBattleMapCenter  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.CenterType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Center)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Param)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.ClientFxScalse)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.OccupantFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.MiniMapFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.OccSuccessFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.OccWinFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.MVPCutScene)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.MVPPos)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SceneID_Index[row.SceneID] = Table.size()-1;


		return true;
	}

	~HeroBattleMapCenter ()
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