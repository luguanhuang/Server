#ifndef __ActivityListTable_H__
#define __ActivityListTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/26 17:05:11


class ActivityListTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SysID;
		string	Tittle;
		vector<string>	TagNames;
		string	Icon;
		vector<string>	TagName;
		bool	HadShop;
		vector<Sequence<uint, 2>>	DropItems;
		string	Describe;
		uint	SortIndex;
		string	AtlasPath;

		void CopyFrom(RowData *pRowData)
		{
			this->SysID=pRowData->SysID;
			this->Tittle=pRowData->Tittle;
			this->TagNames=pRowData->TagNames;
			this->Icon=pRowData->Icon;
			this->TagName=pRowData->TagName;
			this->HadShop=pRowData->HadShop;
			this->DropItems=pRowData->DropItems;
			this->Describe=pRowData->Describe;
			this->SortIndex=pRowData->SortIndex;
			this->AtlasPath=pRowData->AtlasPath;

		}
	};

	std::unordered_map<uint,int> SysID_Index;


    RowData *GetBySysID(const uint &key)
    {
        auto i = SysID_Index.find(key);
        return (i == SysID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SysID", "Tittle", "TagNames", "Icon", "TagName", "HadShop", "DropItems", "Describe", "SortIndex", "AtlasPath",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ActivityListTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySysID(pSrcRow->SysID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SysID_Index[pNewRow->SysID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.SysID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Tittle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.TagNames)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.TagName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.HadShop)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.DropItems)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Describe)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.SortIndex)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.AtlasPath)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SysID_Index[row.SysID] = Table.size()-1;


		return true;
	}

	~ActivityListTable ()
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

		SysID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif