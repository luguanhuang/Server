#ifndef __PVPActivityList_H__
#define __PVPActivityList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class PVPActivityList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SysID;
		string	Name;
		string	Description;
		string	Icon;
		bool	FairMode;

		void CopyFrom(RowData *pRowData)
		{
			this->SysID=pRowData->SysID;
			this->Name=pRowData->Name;
			this->Description=pRowData->Description;
			this->Icon=pRowData->Icon;
			this->FairMode=pRowData->FairMode;

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
		const char *Headers[] = { "SysID", "Name", "Description", "Icon", "FairMode",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PVPActivityList  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Description)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.FairMode)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SysID_Index[row.SysID] = Table.size()-1;


		return true;
	}

	~PVPActivityList ()
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