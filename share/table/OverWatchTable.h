#ifndef __OverWatchTable_H__
#define __OverWatchTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/9/18 17:11:48


class OverWatchTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	HeroID;
		vector<uint>	StatisticsID;
		vector<uint>	BuffID;
		Sequence<uint, 2>	Price;
		string	Name;
		string	Icon;
		string	Description;
		string	CutSceneAniamtion;
		string	CutSceneIdleAni;
		uint	weight;
		int	SortID;
		string	Motto;
		string	MiniMapIcon;
		vector<float>	MobaAttributes;
		string	SelectAnim;
		vector<string>	SelectFx;
		string	HeroUseTips;
		string	MobaUseTips;
		string	IconAtlas;

		void CopyFrom(RowData *pRowData)
		{
			this->HeroID=pRowData->HeroID;
			this->StatisticsID=pRowData->StatisticsID;
			this->BuffID=pRowData->BuffID;
			this->Price=pRowData->Price;
			this->Name=pRowData->Name;
			this->Icon=pRowData->Icon;
			this->Description=pRowData->Description;
			this->CutSceneAniamtion=pRowData->CutSceneAniamtion;
			this->CutSceneIdleAni=pRowData->CutSceneIdleAni;
			this->weight=pRowData->weight;
			this->SortID=pRowData->SortID;
			this->Motto=pRowData->Motto;
			this->MiniMapIcon=pRowData->MiniMapIcon;
			this->MobaAttributes=pRowData->MobaAttributes;
			this->SelectAnim=pRowData->SelectAnim;
			this->SelectFx=pRowData->SelectFx;
			this->HeroUseTips=pRowData->HeroUseTips;
			this->MobaUseTips=pRowData->MobaUseTips;
			this->IconAtlas=pRowData->IconAtlas;

		}
	};

	std::unordered_map<uint,int> HeroID_Index;


    RowData *GetByHeroID(const uint &key)
    {
        auto i = HeroID_Index.find(key);
        return (i == HeroID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "HeroID", "StatisticsID", "BuffID", "Price", "Name", "Icon", "Description", "CutSceneAniamtion", "CutSceneIdleAni", "weight", "SortID", "Motto", "MiniMapIcon", "MobaAttributes", "SelectAnim", "SelectFx", "HeroUseTips", "MobaUseTips", "IconAtlas",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(OverWatchTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByHeroID(pSrcRow->HeroID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                HeroID_Index[pNewRow->HeroID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.HeroID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.StatisticsID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.BuffID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Price)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Description)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.CutSceneAniamtion)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.CutSceneIdleAni)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.weight)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.SortID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.Motto)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.MiniMapIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.MobaAttributes)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.SelectAnim)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.SelectFx)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.HeroUseTips)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.MobaUseTips)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.IconAtlas)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		HeroID_Index[row.HeroID] = Table.size()-1;


		return true;
	}

	~OverWatchTable ()
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

		HeroID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif