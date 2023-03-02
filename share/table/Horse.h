#ifndef __Horse_H__
#define __Horse_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class Horse : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	sceneid;
		Sequence<float, 3>	CenterPos;
		uint	Laps;
		uint	CollisionWallTime;
		uint	FinalWallIndex;
		uint	SprintWallIndex;
		Sequence<float, 3>	FinalPos;

		void CopyFrom(RowData *pRowData)
		{
			this->sceneid=pRowData->sceneid;
			this->CenterPos=pRowData->CenterPos;
			this->Laps=pRowData->Laps;
			this->CollisionWallTime=pRowData->CollisionWallTime;
			this->FinalWallIndex=pRowData->FinalWallIndex;
			this->SprintWallIndex=pRowData->SprintWallIndex;
			this->FinalPos=pRowData->FinalPos;

		}
	};

	std::unordered_map<uint,int> sceneid_Index;


    RowData *GetBysceneid(const uint &key)
    {
        auto i = sceneid_Index.find(key);
        return (i == sceneid_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "sceneid", "CenterPos", "Laps", "CollisionWallTime", "FinalWallIndex", "SprintWallIndex", "FinalPos",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(Horse  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBysceneid(pSrcRow->sceneid);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                sceneid_Index[pNewRow->sceneid]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.sceneid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.CenterPos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Laps)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.CollisionWallTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.FinalWallIndex)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.SprintWallIndex)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.FinalPos)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		sceneid_Index[row.sceneid] = Table.size()-1;


		return true;
	}

	~Horse ()
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

		sceneid_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif