#ifndef __LiveTable_H__
#define __LiveTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/9 15:39:40


class LiveTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	Type;
		string	Name;
		int	DNExpType;
		int	SceneType;
		int	TimeSpan;
		int	LiveNum;
		int	MaxWatchNum;
		int	ShowWatch;
		int	ShowPraise;
		int	NoticeWatchNum;
		int	Page;
		int	SettingType;

		void CopyFrom(RowData *pRowData)
		{
			this->Type=pRowData->Type;
			this->Name=pRowData->Name;
			this->DNExpType=pRowData->DNExpType;
			this->SceneType=pRowData->SceneType;
			this->TimeSpan=pRowData->TimeSpan;
			this->LiveNum=pRowData->LiveNum;
			this->MaxWatchNum=pRowData->MaxWatchNum;
			this->ShowWatch=pRowData->ShowWatch;
			this->ShowPraise=pRowData->ShowPraise;
			this->NoticeWatchNum=pRowData->NoticeWatchNum;
			this->Page=pRowData->Page;
			this->SettingType=pRowData->SettingType;

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
		const char *Headers[] = { "Type", "Name", "DNExpType", "SceneType", "TimeSpan", "LiveNum", "MaxWatchNum", "ShowWatch", "ShowPraise", "NoticeWatchNum", "Page", "SettingType",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(LiveTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.DNExpType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SceneType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.TimeSpan)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.LiveNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.MaxWatchNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.ShowWatch)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.ShowPraise)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.NoticeWatchNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.Page)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.SettingType)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SceneType_Index[row.SceneType] = Table.size()-1;


		return true;
	}

	~LiveTable ()
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