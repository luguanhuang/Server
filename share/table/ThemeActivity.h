#ifndef __ThemeActivity_H__
#define __ThemeActivity_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/7/27 16:23:28


class ThemeActivity : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	OrderId;
		string	SysEnum;
		uint	SysID;
		string	TabName;
		uint	Level;
		uint	OpenTime;
		uint	EndTime;
		string	TabIcon;

		void CopyFrom(RowData *pRowData)
		{
			this->OrderId=pRowData->OrderId;
			this->SysEnum=pRowData->SysEnum;
			this->SysID=pRowData->SysID;
			this->TabName=pRowData->TabName;
			this->Level=pRowData->Level;
			this->OpenTime=pRowData->OpenTime;
			this->EndTime=pRowData->EndTime;
			this->TabIcon=pRowData->TabIcon;

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
		const char *Headers[] = { "OrderId", "SysEnum", "SysID", "TabName", "Level", "OpenTime", "EndTime", "TabIcon",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ThemeActivity  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.OrderId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SysEnum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SysID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.TabName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.OpenTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.EndTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.TabIcon)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SysID_Index[row.SysID] = Table.size()-1;


		return true;
	}

	~ThemeActivity ()
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