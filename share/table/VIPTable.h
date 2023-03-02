#ifndef __VIPTable_H__
#define __VIPTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/8/29 10:32:11


class VIPTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	VIP;
		uint	RMB;
		int	BuyGoldTimes;
		int	BuyFatigueTimes;
		int	NestScoreBonus;
		uint	AuctionOnSaleMax;
		int	BuyDragonCoinTimes;
		int	BossRushCount;
		uint	WorldChannelChatTimes;
		int	ItemID;
		vector<string>	VIPTips;
		uint	GoldClickTimes;
		uint	EquipMax;
		uint	EmblemMax;
		uint	BagMax;
		uint	ArtifactMax;
		uint	InscriptionMax;

		void CopyFrom(RowData *pRowData)
		{
			this->VIP=pRowData->VIP;
			this->RMB=pRowData->RMB;
			this->BuyGoldTimes=pRowData->BuyGoldTimes;
			this->BuyFatigueTimes=pRowData->BuyFatigueTimes;
			this->NestScoreBonus=pRowData->NestScoreBonus;
			this->AuctionOnSaleMax=pRowData->AuctionOnSaleMax;
			this->BuyDragonCoinTimes=pRowData->BuyDragonCoinTimes;
			this->BossRushCount=pRowData->BossRushCount;
			this->WorldChannelChatTimes=pRowData->WorldChannelChatTimes;
			this->ItemID=pRowData->ItemID;
			this->VIPTips=pRowData->VIPTips;
			this->GoldClickTimes=pRowData->GoldClickTimes;
			this->EquipMax=pRowData->EquipMax;
			this->EmblemMax=pRowData->EmblemMax;
			this->BagMax=pRowData->BagMax;
			this->ArtifactMax=pRowData->ArtifactMax;
			this->InscriptionMax=pRowData->InscriptionMax;

		}
	};

	std::unordered_map<int,int> VIP_Index;


    RowData *GetByVIP(const int &key)
    {
        auto i = VIP_Index.find(key);
        return (i == VIP_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "VIP", "RMB", "BuyGoldTimes", "BuyFatigueTimes", "NestScoreBonus", "AuctionOnSaleMax", "BuyDragonCoinTimes", "BossRushCount", "WorldChannelChatTimes", "ItemID", "VIPTips", "GoldClickTimes", "EquipMax", "EmblemMax", "BagMax", "ArtifactMax", "InscriptionMax",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(VIPTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByVIP(pSrcRow->VIP);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                VIP_Index[pNewRow->VIP]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.VIP)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.RMB)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.BuyGoldTimes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.BuyFatigueTimes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.NestScoreBonus)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.AuctionOnSaleMax)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.BuyDragonCoinTimes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.BossRushCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.WorldChannelChatTimes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.ItemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.VIPTips)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.GoldClickTimes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.EquipMax)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.EmblemMax)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.BagMax)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.ArtifactMax)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.InscriptionMax)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		VIP_Index[row.VIP] = Table.size()-1;


		return true;
	}

	~VIPTable ()
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

		VIP_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif