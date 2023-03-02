#ifndef __ItemList_H__
#define __ItemList_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/6/19 16:10:24


class ItemList : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ItemID;
		vector<string>	ItemName;
		string	ItemDescription;
		vector<string>	ItemIcon;
		int	ItemType;
		int	ItemQuality;
		int	ReqLevel;
		int	SellPrize;
		int	SortID;
		vector<string>	ItemAtlas;
		string	DoodadFx;
		vector<Sequence<int, 2>>	Access;
		int	ShowTips;
		vector<string>	ItemAtlas1;
		vector<string>	ItemIcon1;
		bool	CanTrade;
		vector<uint>	AuctionType;
		int	OverCnt;
		uint	AuctPriceRecommend;
		uint	Profession;
		uint	Overlap;
		string	NumberName;
		uint	TimeLimit;
		vector<Sequence<uint, 2>>	Decompose;
		string	ItemEffect;
		int	ReqRoll;
		uint	AuctionGroup;
		uint	IsNeedShowTipsPanel;
		vector<float>	IconTransform;
		Sequence<float, 2>	AuctionRange;
		uint	IsCanRecycle;
		Sequence<uint, 2>	Sell;
		uint	BagType;
		int	AuctionUpperLimit;
		float	DecomposeGold;

		void CopyFrom(RowData *pRowData)
		{
			this->ItemID=pRowData->ItemID;
			this->ItemName=pRowData->ItemName;
			this->ItemDescription=pRowData->ItemDescription;
			this->ItemIcon=pRowData->ItemIcon;
			this->ItemType=pRowData->ItemType;
			this->ItemQuality=pRowData->ItemQuality;
			this->ReqLevel=pRowData->ReqLevel;
			this->SellPrize=pRowData->SellPrize;
			this->SortID=pRowData->SortID;
			this->ItemAtlas=pRowData->ItemAtlas;
			this->DoodadFx=pRowData->DoodadFx;
			this->Access=pRowData->Access;
			this->ShowTips=pRowData->ShowTips;
			this->ItemAtlas1=pRowData->ItemAtlas1;
			this->ItemIcon1=pRowData->ItemIcon1;
			this->CanTrade=pRowData->CanTrade;
			this->AuctionType=pRowData->AuctionType;
			this->OverCnt=pRowData->OverCnt;
			this->AuctPriceRecommend=pRowData->AuctPriceRecommend;
			this->Profession=pRowData->Profession;
			this->Overlap=pRowData->Overlap;
			this->NumberName=pRowData->NumberName;
			this->TimeLimit=pRowData->TimeLimit;
			this->Decompose=pRowData->Decompose;
			this->ItemEffect=pRowData->ItemEffect;
			this->ReqRoll=pRowData->ReqRoll;
			this->AuctionGroup=pRowData->AuctionGroup;
			this->IsNeedShowTipsPanel=pRowData->IsNeedShowTipsPanel;
			this->IconTransform=pRowData->IconTransform;
			this->AuctionRange=pRowData->AuctionRange;
			this->IsCanRecycle=pRowData->IsCanRecycle;
			this->Sell=pRowData->Sell;
			this->BagType=pRowData->BagType;
			this->AuctionUpperLimit=pRowData->AuctionUpperLimit;
			this->DecomposeGold=pRowData->DecomposeGold;

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
		const char *Headers[] = { "ItemID", "ItemName", "ItemDescription", "ItemIcon", "ItemType", "ItemQuality", "ReqLevel", "SellPrize", "SortID", "ItemAtlas", "DoodadFx", "Access", "ShowTips", "ItemAtlas1", "ItemIcon1", "CanTrade", "AuctionType", "OverCnt", "AuctPriceRecommend", "Profession", "Overlap", "NumberName", "TimeLimit", "Decompose", "ItemEffect", "ReqRoll", "AuctionGroup", "IsNeedShowTipsPanel", "IconTransform", "AuctionRange", "IsCanRecycle", "Sell", "BagType", "AuctionUpperLimit", "DecomposeGold",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(ItemList  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.ItemName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.ItemDescription)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.ItemIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.ItemType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.ItemQuality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.ReqLevel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.SellPrize)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.SortID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.ItemAtlas)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.DoodadFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.Access)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.ShowTips)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.ItemAtlas1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.ItemIcon1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.CanTrade)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.AuctionType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.OverCnt)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.AuctPriceRecommend)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.Profession)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.Overlap)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.NumberName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.TimeLimit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.Decompose)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.ItemEffect)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[25]], row.ReqRoll)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[26]], row.AuctionGroup)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[27]], row.IsNeedShowTipsPanel)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[28]], row.IconTransform)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[29]], row.AuctionRange)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[30]], row.IsCanRecycle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[31]], row.Sell)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[32]], row.BagType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[33]], row.AuctionUpperLimit)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[34]], row.DecomposeGold)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ItemID_Index[row.ItemID] = Table.size()-1;


		return true;
	}

	~ItemList ()
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