#ifndef __FriendSysConfigTable_H__
#define __FriendSysConfigTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class FriendSysConfigTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	TabID;
		bool	IsOpen;
		string	TabName;
		string	Icon;
		string	NumLabel;
		string	RButtonLabel;
		string	LButtonLabel;
		int	NumLimit;

		void CopyFrom(RowData *pRowData)
		{
			this->TabID=pRowData->TabID;
			this->IsOpen=pRowData->IsOpen;
			this->TabName=pRowData->TabName;
			this->Icon=pRowData->Icon;
			this->NumLabel=pRowData->NumLabel;
			this->RButtonLabel=pRowData->RButtonLabel;
			this->LButtonLabel=pRowData->LButtonLabel;
			this->NumLimit=pRowData->NumLimit;

		}
	};

	std::unordered_map<int,int> TabID_Index;


    RowData *GetByTabID(const int &key)
    {
        auto i = TabID_Index.find(key);
        return (i == TabID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "TabID", "IsOpen", "TabName", "Icon", "NumLabel", "RButtonLabel", "LButtonLabel", "NumLimit",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FriendSysConfigTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByTabID(pSrcRow->TabID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                TabID_Index[pNewRow->TabID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.TabID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.IsOpen)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.TabName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.NumLabel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.RButtonLabel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.LButtonLabel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.NumLimit)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		TabID_Index[row.TabID] = Table.size()-1;


		return true;
	}

	~FriendSysConfigTable ()
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

		TabID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif