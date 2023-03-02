#ifndef __FashionEnhanceFx_H__
#define __FashionEnhanceFx_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/15 16:07:54


class FashionEnhanceFx : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ItemID;
		vector<string>	Fx1;
		vector<string>	Fx2;
		vector<string>	Fx3;
		vector<string>	Fx4;
		vector<string>	Fx5;
		vector<string>	Fx6;
		vector<string>	Fx7;
		vector<string>	Fx8;

		void CopyFrom(RowData *pRowData)
		{
			this->ItemID=pRowData->ItemID;
			this->Fx1=pRowData->Fx1;
			this->Fx2=pRowData->Fx2;
			this->Fx3=pRowData->Fx3;
			this->Fx4=pRowData->Fx4;
			this->Fx5=pRowData->Fx5;
			this->Fx6=pRowData->Fx6;
			this->Fx7=pRowData->Fx7;
			this->Fx8=pRowData->Fx8;

		}
	};

	std::unordered_map<int,int> ItemID_Index;


    RowData *GetByItemID(const int &key)
    {
        auto i = ItemID_Index.find(key);
        return (i == ItemID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ItemID", "Fx1", "Fx2", "Fx3", "Fx4", "Fx5", "Fx6", "Fx7", "Fx8",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FashionEnhanceFx  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByItemID(pSrcRow->ItemID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ItemID_Index[pNewRow->ItemID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ItemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Fx1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Fx2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Fx3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Fx4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Fx5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Fx6)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Fx7)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Fx8)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ItemID_Index[row.ItemID] = Table.size()-1;


		return true;
	}

	~FashionEnhanceFx ()
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

		ItemID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif