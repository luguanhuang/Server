#ifndef __EquipList_H__
#define __EquipList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/11/3 11:55:30


class EquipList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ItemID;
		int	EquipPos;
		vector<Sequence<int, 2>>	Attributes;
		vector<uint>	DecomposeDropID;
		Sequence<uint, 3>	Jade;
		uint	MaxEnhanceLevel;
		vector<Sequence<uint, 2>>	SmeltNeedItem;
		uint	SmeltNeedMoney;
		vector<Sequence<uint, 2>>	ForgeNeedItem;
		Sequence<uint, 2>	ForgeSpecialItem;
		uint	ForgeLowRate;
		uint	ForgeHighRate;
		vector<Sequence<uint, 2>>	ForgeNeedItemAfter;
		Sequence<uint, 2>	ForgeSpecialItemAfter;
		uint	ForgeLowRateAfter;
		uint	ForgeHighRateAfter;
		bool	IsCanSmelt;
		uint	ReturnSmeltStoneRate;
		uint	CanForge;
		vector<Sequence<uint, 2>>	UpgradeNeedMaterials;
		uint	UpgadeTargetID;
		vector<uint>	FuseCoreItems;
		uint	FuseCanBreakNum;
		uint	EquipType;
		vector<Sequence<uint, 2>>	ForgeCompensation;

		void CopyFrom(RowData *pRowData)
		{
			this->ItemID=pRowData->ItemID;
			this->EquipPos=pRowData->EquipPos;
			this->Attributes=pRowData->Attributes;
			this->DecomposeDropID=pRowData->DecomposeDropID;
			this->Jade=pRowData->Jade;
			this->MaxEnhanceLevel=pRowData->MaxEnhanceLevel;
			this->SmeltNeedItem=pRowData->SmeltNeedItem;
			this->SmeltNeedMoney=pRowData->SmeltNeedMoney;
			this->ForgeNeedItem=pRowData->ForgeNeedItem;
			this->ForgeSpecialItem=pRowData->ForgeSpecialItem;
			this->ForgeLowRate=pRowData->ForgeLowRate;
			this->ForgeHighRate=pRowData->ForgeHighRate;
			this->ForgeNeedItemAfter=pRowData->ForgeNeedItemAfter;
			this->ForgeSpecialItemAfter=pRowData->ForgeSpecialItemAfter;
			this->ForgeLowRateAfter=pRowData->ForgeLowRateAfter;
			this->ForgeHighRateAfter=pRowData->ForgeHighRateAfter;
			this->IsCanSmelt=pRowData->IsCanSmelt;
			this->ReturnSmeltStoneRate=pRowData->ReturnSmeltStoneRate;
			this->CanForge=pRowData->CanForge;
			this->UpgradeNeedMaterials=pRowData->UpgradeNeedMaterials;
			this->UpgadeTargetID=pRowData->UpgadeTargetID;
			this->FuseCoreItems=pRowData->FuseCoreItems;
			this->FuseCanBreakNum=pRowData->FuseCanBreakNum;
			this->EquipType=pRowData->EquipType;
			this->ForgeCompensation=pRowData->ForgeCompensation;

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
		const char *Headers[] = { "ItemID", "EquipPos", "Attributes", "DecomposeDropID", "Jade", "MaxEnhanceLevel", "SmeltNeedItem", "SmeltNeedMoney", "ForgeNeedItem", "ForgeSpecialItem", "ForgeLowRate", "ForgeHighRate", "ForgeNeedItemAfter", "ForgeSpecialItemAfter", "ForgeLowRateAfter", "ForgeHighRateAfter", "IsCanSmelt", "ReturnSmeltStoneRate", "CanForge", "UpgradeNeedMaterials", "UpgadeTargetID", "FuseCoreItems", "FuseCanBreakNum", "EquipType", "ForgeCompensation",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(EquipList  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.EquipPos)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Attributes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.DecomposeDropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Jade)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.MaxEnhanceLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.SmeltNeedItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.SmeltNeedMoney)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.ForgeNeedItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.ForgeSpecialItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.ForgeLowRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.ForgeHighRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.ForgeNeedItemAfter)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.ForgeSpecialItemAfter)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.ForgeLowRateAfter)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.ForgeHighRateAfter)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.IsCanSmelt)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.ReturnSmeltStoneRate)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.CanForge)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.UpgradeNeedMaterials)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.UpgadeTargetID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.FuseCoreItems)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.FuseCanBreakNum)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.EquipType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.ForgeCompensation)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ItemID_Index[row.ItemID] = Table.size()-1;


		return true;
	}

	~EquipList ()
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