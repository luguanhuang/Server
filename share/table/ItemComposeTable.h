#ifndef __ItemComposeTable_H__
#define __ItemComposeTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/8 16:53:22


class ItemComposeTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ItemID;
		Sequence<int, 2>	SrcItem1;
		Sequence<int, 2>	SrcItem2;
		Sequence<int, 2>	SrcItem3;
		int	ID;
		int	Coin;
		int	Level;
		bool	IsBind;
		Sequence<int, 2>	SrcItem4;
		int	Type;
		vector<Sequence<uint, 2>>	FuseCompensation;

		void CopyFrom(RowData *pRowData)
		{
			this->ItemID=pRowData->ItemID;
			this->SrcItem1=pRowData->SrcItem1;
			this->SrcItem2=pRowData->SrcItem2;
			this->SrcItem3=pRowData->SrcItem3;
			this->ID=pRowData->ID;
			this->Coin=pRowData->Coin;
			this->Level=pRowData->Level;
			this->IsBind=pRowData->IsBind;
			this->SrcItem4=pRowData->SrcItem4;
			this->Type=pRowData->Type;
			this->FuseCompensation=pRowData->FuseCompensation;

		}
	};

	std::unordered_map<int,int> ID_Index;


    RowData *GetByID(const int &key)
    {
        auto i = ID_Index.find(key);
        return (i == ID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ItemID", "SrcItem1", "SrcItem2", "SrcItem3", "ID", "Coin", "Level", "IsBind", "SrcItem4", "Type", "FuseCompensation",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ItemComposeTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByID(pSrcRow->ID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ID_Index[pNewRow->ID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ItemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SrcItem1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SrcItem2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SrcItem3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Coin)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.IsBind)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.SrcItem4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.FuseCompensation)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~ItemComposeTable ()
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

		ID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif