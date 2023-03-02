#ifndef __SpriteTable_H__
#define __SpriteTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class SpriteTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	SpriteID;
		string	SpriteName;
		uint	SpriteQuality;
		string	SpriteIcon;
		uint	SpriteModelID;
		string	SpriteDescription;
		string	ShowEffect;
		uint	SpriteSkillID;
		uint	PassiveSkillsID;
		uint	PassiveSkillMax;
		uint	AttrID1;
		uint	AttrID2;
		uint	AttrID3;
		uint	AttrID4;
		uint	AttrID5;
		uint	BaseAttr1;
		uint	BaseAttr2;
		uint	BaseAttr3;
		uint	BaseAttr4;
		uint	BaseAttr5;
		Sequence<uint, 2>	InitialRange1;
		Sequence<uint, 2>	InitialRange2;
		Sequence<uint, 2>	InitialRange3;
		Sequence<uint, 2>	InitialRange4;
		Sequence<uint, 2>	InitialRange5;
		Sequence<uint, 2>	Range1;
		Sequence<uint, 2>	Range2;
		Sequence<uint, 2>	Range3;
		Sequence<uint, 2>	Range4;
		Sequence<uint, 2>	Range5;
		vector<Sequence<int, 3>>	SmeltAttr1;
		vector<Sequence<int, 3>>	SmeltAttr2;
		vector<Sequence<int, 3>>	SmeltAttr3;
		vector<Sequence<int, 3>>	SmeltAttr4;
		vector<Sequence<int, 3>>	SmeltAttr5;
		uint	AttrMin1;
		uint	AttrMax1;
		uint	AttrMin2;
		uint	AttrMax2;
		uint	AttrMin3;
		uint	AttrMax3;
		uint	AttrMin4;
		uint	AttrMax4;
		uint	AttrMin5;
		uint	AttrMax5;
		vector<uint>	DropID;
		vector<int>	SpriteShow;
		uint	PresentID;
		string	Color;
		string	DeathAnim;
		string	ReviveAnim;
		int	IllustrationShow;

		void CopyFrom(RowData *pRowData)
		{
			this->SpriteID=pRowData->SpriteID;
			this->SpriteName=pRowData->SpriteName;
			this->SpriteQuality=pRowData->SpriteQuality;
			this->SpriteIcon=pRowData->SpriteIcon;
			this->SpriteModelID=pRowData->SpriteModelID;
			this->SpriteDescription=pRowData->SpriteDescription;
			this->ShowEffect=pRowData->ShowEffect;
			this->SpriteSkillID=pRowData->SpriteSkillID;
			this->PassiveSkillsID=pRowData->PassiveSkillsID;
			this->PassiveSkillMax=pRowData->PassiveSkillMax;
			this->AttrID1=pRowData->AttrID1;
			this->AttrID2=pRowData->AttrID2;
			this->AttrID3=pRowData->AttrID3;
			this->AttrID4=pRowData->AttrID4;
			this->AttrID5=pRowData->AttrID5;
			this->BaseAttr1=pRowData->BaseAttr1;
			this->BaseAttr2=pRowData->BaseAttr2;
			this->BaseAttr3=pRowData->BaseAttr3;
			this->BaseAttr4=pRowData->BaseAttr4;
			this->BaseAttr5=pRowData->BaseAttr5;
			this->InitialRange1=pRowData->InitialRange1;
			this->InitialRange2=pRowData->InitialRange2;
			this->InitialRange3=pRowData->InitialRange3;
			this->InitialRange4=pRowData->InitialRange4;
			this->InitialRange5=pRowData->InitialRange5;
			this->Range1=pRowData->Range1;
			this->Range2=pRowData->Range2;
			this->Range3=pRowData->Range3;
			this->Range4=pRowData->Range4;
			this->Range5=pRowData->Range5;
			this->SmeltAttr1=pRowData->SmeltAttr1;
			this->SmeltAttr2=pRowData->SmeltAttr2;
			this->SmeltAttr3=pRowData->SmeltAttr3;
			this->SmeltAttr4=pRowData->SmeltAttr4;
			this->SmeltAttr5=pRowData->SmeltAttr5;
			this->AttrMin1=pRowData->AttrMin1;
			this->AttrMax1=pRowData->AttrMax1;
			this->AttrMin2=pRowData->AttrMin2;
			this->AttrMax2=pRowData->AttrMax2;
			this->AttrMin3=pRowData->AttrMin3;
			this->AttrMax3=pRowData->AttrMax3;
			this->AttrMin4=pRowData->AttrMin4;
			this->AttrMax4=pRowData->AttrMax4;
			this->AttrMin5=pRowData->AttrMin5;
			this->AttrMax5=pRowData->AttrMax5;
			this->DropID=pRowData->DropID;
			this->SpriteShow=pRowData->SpriteShow;
			this->PresentID=pRowData->PresentID;
			this->Color=pRowData->Color;
			this->DeathAnim=pRowData->DeathAnim;
			this->ReviveAnim=pRowData->ReviveAnim;
			this->IllustrationShow=pRowData->IllustrationShow;

		}
	};

	std::unordered_map<uint,int> SpriteID_Index;
	std::unordered_map<uint,int> PresentID_Index;


    RowData *GetBySpriteID(const uint &key)
    {
        auto i = SpriteID_Index.find(key);
        return (i == SpriteID_Index.end()) ? NULL : Table[i->second];
    }
    
    RowData *GetByPresentID(const uint &key)
    {
        auto i = PresentID_Index.find(key);
        return (i == PresentID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "SpriteID", "SpriteName", "SpriteQuality", "SpriteIcon", "SpriteModelID", "SpriteDescription", "ShowEffect", "SpriteSkillID", "PassiveSkillsID", "PassiveSkillMax", "AttrID1", "AttrID2", "AttrID3", "AttrID4", "AttrID5", "BaseAttr1", "BaseAttr2", "BaseAttr3", "BaseAttr4", "BaseAttr5", "InitialRange1", "InitialRange2", "InitialRange3", "InitialRange4", "InitialRange5", "Range1", "Range2", "Range3", "Range4", "Range5", "SmeltAttr1", "SmeltAttr2", "SmeltAttr3", "SmeltAttr4", "SmeltAttr5", "AttrMin1", "AttrMax1", "AttrMin2", "AttrMax2", "AttrMin3", "AttrMax3", "AttrMin4", "AttrMax4", "AttrMin5", "AttrMax5", "DropID", "SpriteShow", "PresentID", "Color", "DeathAnim", "ReviveAnim", "IllustrationShow",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(SpriteTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetBySpriteID(pSrcRow->SpriteID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                SpriteID_Index[pNewRow->SpriteID]=Table.size()-1;
                PresentID_Index[pNewRow->PresentID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.SpriteID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.SpriteName)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.SpriteQuality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.SpriteIcon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.SpriteModelID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.SpriteDescription)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.ShowEffect)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.SpriteSkillID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.PassiveSkillsID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.PassiveSkillMax)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.AttrID1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.AttrID2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.AttrID3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.AttrID4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.AttrID5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.BaseAttr1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.BaseAttr2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.BaseAttr3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.BaseAttr4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.BaseAttr5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.InitialRange1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.InitialRange2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.InitialRange3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.InitialRange4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.InitialRange5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[25]], row.Range1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[26]], row.Range2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[27]], row.Range3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[28]], row.Range4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[29]], row.Range5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[30]], row.SmeltAttr1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[31]], row.SmeltAttr2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[32]], row.SmeltAttr3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[33]], row.SmeltAttr4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[34]], row.SmeltAttr5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[35]], row.AttrMin1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[36]], row.AttrMax1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[37]], row.AttrMin2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[38]], row.AttrMax2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[39]], row.AttrMin3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[40]], row.AttrMax3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[41]], row.AttrMin4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[42]], row.AttrMax4)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[43]], row.AttrMin5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[44]], row.AttrMax5)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[45]], row.DropID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[46]], row.SpriteShow)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[47]], row.PresentID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[48]], row.Color)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[49]], row.DeathAnim)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[50]], row.ReviveAnim)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[51]], row.IllustrationShow)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		SpriteID_Index[row.SpriteID] = Table.size()-1;
		PresentID_Index[row.PresentID] = Table.size()-1;


		return true;
	}

	~SpriteTable ()
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

		SpriteID_Index.clear();
		PresentID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif