#ifndef __ShopTable_H__
#define __ShopTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/5/13 16:10:05


class ShopTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ID;
		uint	ItemId;
		Sequence<uint, 2>	ConsumeItem;
		uint	Type;
		uint	LevelCondition;
		uint	PowerpointCondition;
		uint	DailyCountCondition;
		uint	CountCondition;
		Sequence<uint, 2>	Benefit;
		vector<Sequence<uint, 2>>	Price;
		Sequence<uint, 2>	LevelShow;
		uint	VIPCondition;
		uint	ArenaCondition;
		uint	PKCondition;
		uint	GuildLevel;
		Sequence<uint, 2>	Slot;
		bool	IsNotBind;
		uint	CookLevel;
		int	ShopItemType;
		int	WeekCountCondition;
		vector<int>	Profession;

		void CopyFrom(RowData *pRowData)
		{
			this->ID=pRowData->ID;
			this->ItemId=pRowData->ItemId;
			this->ConsumeItem=pRowData->ConsumeItem;
			this->Type=pRowData->Type;
			this->LevelCondition=pRowData->LevelCondition;
			this->PowerpointCondition=pRowData->PowerpointCondition;
			this->DailyCountCondition=pRowData->DailyCountCondition;
			this->CountCondition=pRowData->CountCondition;
			this->Benefit=pRowData->Benefit;
			this->Price=pRowData->Price;
			this->LevelShow=pRowData->LevelShow;
			this->VIPCondition=pRowData->VIPCondition;
			this->ArenaCondition=pRowData->ArenaCondition;
			this->PKCondition=pRowData->PKCondition;
			this->GuildLevel=pRowData->GuildLevel;
			this->Slot=pRowData->Slot;
			this->IsNotBind=pRowData->IsNotBind;
			this->CookLevel=pRowData->CookLevel;
			this->ShopItemType=pRowData->ShopItemType;
			this->WeekCountCondition=pRowData->WeekCountCondition;
			this->Profession=pRowData->Profession;

		}
	};

	std::unordered_map<uint,int> ID_Index;


    RowData *GetByID(const uint &key)
    {
        auto i = ID_Index.find(key);
        return (i == ID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ID", "ItemId", "ConsumeItem", "Type", "LevelCondition", "PowerpointCondition", "DailyCountCondition", "CountCondition", "Benefit", "Price", "LevelShow", "VIPCondition", "ArenaCondition", "PKCondition", "GuildLevel", "Slot", "IsNotBind", "CookLevel", "ShopItemType", "WeekCountCondition", "Profession",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ShopTable  *pSrcTable)
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

		if (!Parse(Fields[m_ColMap[0]], row.ID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.ItemId)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ConsumeItem)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Type)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.LevelCondition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.PowerpointCondition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.DailyCountCondition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.CountCondition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.Benefit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Price)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.LevelShow)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.VIPCondition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.ArenaCondition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.PKCondition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.GuildLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.Slot)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.IsNotBind)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.CookLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.ShopItemType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.WeekCountCondition)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.Profession)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ID_Index[row.ID] = Table.size()-1;


		return true;
	}

	~ShopTable ()
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