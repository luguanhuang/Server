#ifndef __ForgeTransAttr_H__
#define __ForgeTransAttr_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class ForgeTransAttr : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ProfID;
		vector<uint>	AttrID;

		void CopyFrom(RowData *pRowData)
		{
			this->ProfID=pRowData->ProfID;
			this->AttrID=pRowData->AttrID;

		}
	};

	std::unordered_map<uint,int> ProfID_Index;


    RowData *GetByProfID(const uint &key)
    {
        auto i = ProfID_Index.find(key);
        return (i == ProfID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ProfID", "AttrID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ForgeTransAttr  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByProfID(pSrcRow->ProfID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ProfID_Index[pNewRow->ProfID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ProfID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.AttrID)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ProfID_Index[row.ProfID] = Table.size()-1;


		return true;
	}

	~ForgeTransAttr ()
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

		ProfID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif