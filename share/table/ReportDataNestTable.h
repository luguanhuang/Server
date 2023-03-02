#ifndef __ReportDataNestTable_H__
#define __ReportDataNestTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/8 10:43:11


class ReportDataNestTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	SceneID;
		string	Comment;
		int	HardLevel;
		int	Type;
		int	PlayReportID;
		int	FirstPassReportID;
		int	WinReportID;

		void CopyFrom(RowData *pRowData)
		{
			this->SceneID=pRowData->SceneID;
			this->Comment=pRowData->Comment;
			this->HardLevel=pRowData->HardLevel;
			this->Type=pRowData->Type;
			this->PlayReportID=pRowData->PlayReportID;
			this->FirstPassReportID=pRowData->FirstPassReportID;
			this->WinReportID=pRowData->WinReportID;

		}
	};

	std::unordered_map<int,int> SceneID_Index;


    RowData *GetBySceneID(const int &key)
    {
        auto i = SceneID_Index.find(key);
        return (i == SceneID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SceneID", "Comment", "HardLevel", "Type", "PlayReportID", "FirstPassReportID", "WinReportID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ReportDataNestTable  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.Comment)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.HardLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.PlayReportID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.FirstPassReportID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.WinReportID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SceneID_Index[row.SceneID] = Table.size()-1;


		return true;
	}

	~ReportDataNestTable ()
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