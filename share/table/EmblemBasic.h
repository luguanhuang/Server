#ifndef __EmblemBasic_H__
#define __EmblemBasic_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/13 21:36:37


class EmblemBasic : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	EmblemID;
		uint	Level;
		uint	ProfID;
		uint	EmblemType;
		uint	DragonCoinCost;
		uint	ThirdProb;
		vector<Sequence<uint, 2>>	SmeltNeedItem;
		uint	SmeltNeedMoney;
		uint	ReturnSmeltStoneRate;

		void CopyFrom(RowData *pRowData)
		{
			this->EmblemID=pRowData->EmblemID;
			this->Level=pRowData->Level;
			this->ProfID=pRowData->ProfID;
			this->EmblemType=pRowData->EmblemType;
			this->DragonCoinCost=pRowData->DragonCoinCost;
			this->ThirdProb=pRowData->ThirdProb;
			this->SmeltNeedItem=pRowData->SmeltNeedItem;
			this->SmeltNeedMoney=pRowData->SmeltNeedMoney;
			this->ReturnSmeltStoneRate=pRowData->ReturnSmeltStoneRate;

		}
	};

	std::unordered_map<uint,int> EmblemID_Index;


    RowData *GetByEmblemID(const uint &key)
    {
        auto i = EmblemID_Index.find(key);
        return (i == EmblemID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "EmblemID", "Level", "ProfID", "EmblemType", "DragonCoinCost", "ThirdProb", "SmeltNeedItem", "SmeltNeedMoney", "ReturnSmeltStoneRate",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EmblemBasic  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByEmblemID(pSrcRow->EmblemID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                EmblemID_Index[pNewRow->EmblemID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.EmblemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Level)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ProfID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.EmblemType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.DragonCoinCost)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.ThirdProb)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.SmeltNeedItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.SmeltNeedMoney)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.ReturnSmeltStoneRate)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		EmblemID_Index[row.EmblemID] = Table.size()-1;


		return true;
	}

	~EmblemBasic ()
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

		EmblemID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif