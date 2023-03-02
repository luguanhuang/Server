#ifndef __FpStrengthNew_H__
#define __FpStrengthNew_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/27 20:04:19


class FpStrengthNew : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	BQID;
		int	Bqtype;
		int	BQSystem;
		string	BQTips;
		string	BQImageID;
		string	BQName;
		int	ShowLevel;
		int	StarNum;

		void CopyFrom(RowData *pRowData)
		{
			this->BQID=pRowData->BQID;
			this->Bqtype=pRowData->Bqtype;
			this->BQSystem=pRowData->BQSystem;
			this->BQTips=pRowData->BQTips;
			this->BQImageID=pRowData->BQImageID;
			this->BQName=pRowData->BQName;
			this->ShowLevel=pRowData->ShowLevel;
			this->StarNum=pRowData->StarNum;

		}
	};

	std::unordered_map<int,int> BQID_Index;


    RowData *GetByBQID(const int &key)
    {
        auto i = BQID_Index.find(key);
        return (i == BQID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "BQID", "Bqtype", "BQSystem", "BQTips", "BQImageID", "BQName", "ShowLevel", "StarNum",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FpStrengthNew  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByBQID(pSrcRow->BQID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                BQID_Index[pNewRow->BQID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.BQID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Bqtype)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.BQSystem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.BQTips)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.BQImageID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.BQName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.ShowLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.StarNum)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		BQID_Index[row.BQID] = Table.size()-1;


		return true;
	}

	~FpStrengthNew ()
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

		BQID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif