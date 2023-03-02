#ifndef __RiskMapFile_H__
#define __RiskMapFile_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class RiskMapFile : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	MapID;
		int	NeedLevel;
		string	FileName;
		int	StepSizeX;
		int	StepSizeY;
		int	StartUIX;
		int	StartUIY;
		vector<Sequence<int, 3>>	BoxPreview;
		string	MapBgName;
		string	MapTittleName;
		string	MapGridBg;

		void CopyFrom(RowData *pRowData)
		{
			this->MapID=pRowData->MapID;
			this->NeedLevel=pRowData->NeedLevel;
			this->FileName=pRowData->FileName;
			this->StepSizeX=pRowData->StepSizeX;
			this->StepSizeY=pRowData->StepSizeY;
			this->StartUIX=pRowData->StartUIX;
			this->StartUIY=pRowData->StartUIY;
			this->BoxPreview=pRowData->BoxPreview;
			this->MapBgName=pRowData->MapBgName;
			this->MapTittleName=pRowData->MapTittleName;
			this->MapGridBg=pRowData->MapGridBg;

		}
	};

	std::unordered_map<int,int> MapID_Index;


    RowData *GetByMapID(const int &key)
    {
        auto i = MapID_Index.find(key);
        return (i == MapID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "MapID", "NeedLevel", "FileName", "StepSizeX", "StepSizeY", "StartUIX", "StartUIY", "BoxPreview", "MapBgName", "MapTittleName", "MapGridBg",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(RiskMapFile  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByMapID(pSrcRow->MapID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                MapID_Index[pNewRow->MapID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.MapID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.NeedLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.FileName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.StepSizeX)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.StepSizeY)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.StartUIX)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.StartUIY)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.BoxPreview)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.MapBgName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.MapTittleName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.MapGridBg)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		MapID_Index[row.MapID] = Table.size()-1;


		return true;
	}

	~RiskMapFile ()
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

		MapID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif