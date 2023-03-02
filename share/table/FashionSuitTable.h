#ifndef __FashionSuitTable_H__
#define __FashionSuitTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/25 10:50:01


class FashionSuitTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	SuitID;
		string	SuitName;
		int	SuitQuality;
		vector<string>	SuitIcon;
		vector<uint>	FashionID;
		vector<Sequence<uint, 2>>	Effect2;
		vector<Sequence<uint, 2>>	Effect3;
		vector<Sequence<uint, 2>>	Effect4;
		vector<Sequence<uint, 2>>	Effect5;
		vector<Sequence<uint, 2>>	Effect6;
		vector<Sequence<uint, 2>>	Effect7;
		vector<Sequence<uint, 2>>	All1;
		vector<Sequence<uint, 2>>	All2;
		vector<Sequence<uint, 2>>	All3;
		vector<Sequence<uint, 2>>	All4;
		string	SuitAtlas;
		int	All0SP;
		int	All1SP;
		int	All2SP;
		int	All3SP;
		int	All4SP;
		bool	NoSale;
		int	ShowLevel;
		int	OverAll;
		int	CraftedItemQuality;

		void CopyFrom(RowData *pRowData)
		{
			this->SuitID=pRowData->SuitID;
			this->SuitName=pRowData->SuitName;
			this->SuitQuality=pRowData->SuitQuality;
			this->SuitIcon=pRowData->SuitIcon;
			this->FashionID=pRowData->FashionID;
			this->Effect2=pRowData->Effect2;
			this->Effect3=pRowData->Effect3;
			this->Effect4=pRowData->Effect4;
			this->Effect5=pRowData->Effect5;
			this->Effect6=pRowData->Effect6;
			this->Effect7=pRowData->Effect7;
			this->All1=pRowData->All1;
			this->All2=pRowData->All2;
			this->All3=pRowData->All3;
			this->All4=pRowData->All4;
			this->SuitAtlas=pRowData->SuitAtlas;
			this->All0SP=pRowData->All0SP;
			this->All1SP=pRowData->All1SP;
			this->All2SP=pRowData->All2SP;
			this->All3SP=pRowData->All3SP;
			this->All4SP=pRowData->All4SP;
			this->NoSale=pRowData->NoSale;
			this->ShowLevel=pRowData->ShowLevel;
			this->OverAll=pRowData->OverAll;
			this->CraftedItemQuality=pRowData->CraftedItemQuality;

		}
	};

	std::unordered_map<int,int> SuitID_Index;


    RowData *GetBySuitID(const int &key)
    {
        auto i = SuitID_Index.find(key);
        return (i == SuitID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SuitID", "SuitName", "SuitQuality", "SuitIcon", "FashionID", "Effect2", "Effect3", "Effect4", "Effect5", "Effect6", "Effect7", "All1", "All2", "All3", "All4", "SuitAtlas", "All0SP", "All1SP", "All2SP", "All3SP", "All4SP", "NoSale", "ShowLevel", "OverAll", "CraftedItemQuality",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(FashionSuitTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySuitID(pSrcRow->SuitID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SuitID_Index[pNewRow->SuitID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.SuitID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SuitName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SuitQuality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SuitIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.FashionID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Effect2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Effect3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Effect4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Effect5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Effect6)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.Effect7)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.All1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.All2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.All3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.All4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.SuitAtlas)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.All0SP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.All1SP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.All2SP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.All3SP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.All4SP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.NoSale)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.ShowLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.OverAll)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.CraftedItemQuality)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SuitID_Index[row.SuitID] = Table.size()-1;


		return true;
	}

	~FashionSuitTable ()
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

		SuitID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif