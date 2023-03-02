#ifndef __PayMemberTable_H__
#define __PayMemberTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/13 15:54:23


class PayMemberTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		string	ParamID;
		int	ID;
		string	Name;
		int	Price;
		int	Days;
		int	DragonCoin;
		Sequence<int, 2>	Flower;
		int	ChatCount;
		int	FatigueLimit;
		int	AbyssCount;
		int	ReviveCount;
		Sequence<int, 2>	PetFeedAdd;
		int	BossRushCount;
		int	BuyGreenAgateLimit;
		vector<int>	CheckinDoubleDays;
		Sequence<int, 2>	SpriteFeedAdd;
		int	SuperRiskCount;
		int	SystemID;
		string	Icon;
		string	Desc;
		string	Tip;
		int	ExpireMailID;
		string	Detail;
		string	BuyNtf;
		vector<int>	CheckinDoubleEvenDays;
		string	ServiceCode;
		Sequence<uint, 2>	Value;
		int	ExpireSoonMailID;
		int	AuctionCount;
		Sequence<uint, 2>	worldBossbuffid;
		Sequence<uint, 2>	guildBossBuffid;
		int	HeroBattleFree;
		uint	NpcFeeling;

		void CopyFrom(RowData *pRowData)
		{
			this->ParamID=pRowData->ParamID;
			this->ID=pRowData->ID;
			this->Name=pRowData->Name;
			this->Price=pRowData->Price;
			this->Days=pRowData->Days;
			this->DragonCoin=pRowData->DragonCoin;
			this->Flower=pRowData->Flower;
			this->ChatCount=pRowData->ChatCount;
			this->FatigueLimit=pRowData->FatigueLimit;
			this->AbyssCount=pRowData->AbyssCount;
			this->ReviveCount=pRowData->ReviveCount;
			this->PetFeedAdd=pRowData->PetFeedAdd;
			this->BossRushCount=pRowData->BossRushCount;
			this->BuyGreenAgateLimit=pRowData->BuyGreenAgateLimit;
			this->CheckinDoubleDays=pRowData->CheckinDoubleDays;
			this->SpriteFeedAdd=pRowData->SpriteFeedAdd;
			this->SuperRiskCount=pRowData->SuperRiskCount;
			this->SystemID=pRowData->SystemID;
			this->Icon=pRowData->Icon;
			this->Desc=pRowData->Desc;
			this->Tip=pRowData->Tip;
			this->ExpireMailID=pRowData->ExpireMailID;
			this->Detail=pRowData->Detail;
			this->BuyNtf=pRowData->BuyNtf;
			this->CheckinDoubleEvenDays=pRowData->CheckinDoubleEvenDays;
			this->ServiceCode=pRowData->ServiceCode;
			this->Value=pRowData->Value;
			this->ExpireSoonMailID=pRowData->ExpireSoonMailID;
			this->AuctionCount=pRowData->AuctionCount;
			this->worldBossbuffid=pRowData->worldBossbuffid;
			this->guildBossBuffid=pRowData->guildBossBuffid;
			this->HeroBattleFree=pRowData->HeroBattleFree;
			this->NpcFeeling=pRowData->NpcFeeling;

		}
	};

	std::unordered_map<string,int> ParamID_Index;


    RowData *GetByParamID(const string &key)
    {
        auto i = ParamID_Index.find(key);
        return (i == ParamID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ParamID", "ID", "Name", "Price", "Days", "DragonCoin", "Flower", "ChatCount", "FatigueLimit", "AbyssCount", "ReviveCount", "PetFeedAdd", "BossRushCount", "BuyGreenAgateLimit", "CheckinDoubleDays", "SpriteFeedAdd", "SuperRiskCount", "SystemID", "Icon", "Desc", "Tip", "ExpireMailID", "Detail", "BuyNtf", "CheckinDoubleEvenDays", "ServiceCode", "Value", "ExpireSoonMailID", "AuctionCount", "worldBossbuffid", "guildBossBuffid", "HeroBattleFree", "NpcFeeling",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PayMemberTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByParamID(pSrcRow->ParamID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ParamID_Index[pNewRow->ParamID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ParamID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Price)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Days)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.DragonCoin)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Flower)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.ChatCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.FatigueLimit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.AbyssCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.ReviveCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.PetFeedAdd)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.BossRushCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.BuyGreenAgateLimit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.CheckinDoubleDays)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.SpriteFeedAdd)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.SuperRiskCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.SystemID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.Desc)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.Tip)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.ExpireMailID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.Detail)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.BuyNtf)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.CheckinDoubleEvenDays)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[25]], row.ServiceCode)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[26]], row.Value)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[27]], row.ExpireSoonMailID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[28]], row.AuctionCount)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[29]], row.worldBossbuffid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[30]], row.guildBossBuffid)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[31]], row.HeroBattleFree)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[32]], row.NpcFeeling)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ParamID_Index[row.ParamID] = Table.size()-1;


		return true;
	}

	~PayMemberTable ()
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

		ParamID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif