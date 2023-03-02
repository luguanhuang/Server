#ifndef __DefaultEquip_H__
#define __DefaultEquip_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/11/3 17:58:33


class DefaultEquip : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		int	ProfID;
		string	Helmet;
		string	Face;
		string	Body;
		string	Leg;
		string	Boots;
		string	Glove;
		string	Weapon;
		vector<string>	WeaponPoint;
		string	WingPoint;
		string	SecondWeapon;
		string	Wing;
		string	Tail;
		string	Decal;
		string	Hair;
		string	TailPoint;
		string	FishingPoint;
		vector<string>	SideWeaponPoint;
		string	RootPoint;
		int	id;

		void CopyFrom(RowData *pRowData)
		{
			this->ProfID=pRowData->ProfID;
			this->Helmet=pRowData->Helmet;
			this->Face=pRowData->Face;
			this->Body=pRowData->Body;
			this->Leg=pRowData->Leg;
			this->Boots=pRowData->Boots;
			this->Glove=pRowData->Glove;
			this->Weapon=pRowData->Weapon;
			this->WeaponPoint=pRowData->WeaponPoint;
			this->WingPoint=pRowData->WingPoint;
			this->SecondWeapon=pRowData->SecondWeapon;
			this->Wing=pRowData->Wing;
			this->Tail=pRowData->Tail;
			this->Decal=pRowData->Decal;
			this->Hair=pRowData->Hair;
			this->TailPoint=pRowData->TailPoint;
			this->FishingPoint=pRowData->FishingPoint;
			this->SideWeaponPoint=pRowData->SideWeaponPoint;
			this->RootPoint=pRowData->RootPoint;
			this->id=pRowData->id;

		}
	};

	std::unordered_map<int,int> ProfID_Index;


    RowData *GetByProfID(const int &key)
    {
        auto i = ProfID_Index.find(key);
        return (i == ProfID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ProfID", "Helmet", "Face", "Body", "Leg", "Boots", "Glove", "Weapon", "WeaponPoint", "WingPoint", "SecondWeapon", "Wing", "Tail", "Decal", "Hair", "TailPoint", "FishingPoint", "SideWeaponPoint", "RootPoint", "ID",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(DefaultEquip  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByProfID(pSrcRow->ProfID);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                ProfID_Index[pNewRow->ProfID]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.ProfID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.Helmet)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Face)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Body)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.Leg)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.Boots)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.Glove)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.Weapon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.WeaponPoint)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.WingPoint)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.SecondWeapon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.Wing)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.Tail)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.Decal)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.Hair)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.TailPoint)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.FishingPoint)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.SideWeaponPoint)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.RootPoint)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.id)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ProfID_Index[row.ProfID] = Table.size()-1;


		return true;
	}

	~DefaultEquip ()
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

		ProfID_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif