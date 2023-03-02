#ifndef __HeroBattleAncientPower_H__
#define __HeroBattleAncientPower_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/7/15 21:59:14


class HeroBattleAncientPower : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	HeroID;
		vector<uint>	Buff1;
		vector<uint>	Buff2;
		vector<uint>	Buff3;
		double	AddPowerPerSecond;
		double	AddPowerInCircle;
		double	AddPowerBeHit;

		void CopyFrom(RowData *pRowData)
		{
			this->HeroID=pRowData->HeroID;
			this->Buff1=pRowData->Buff1;
			this->Buff2=pRowData->Buff2;
			this->Buff3=pRowData->Buff3;
			this->AddPowerPerSecond=pRowData->AddPowerPerSecond;
			this->AddPowerInCircle=pRowData->AddPowerInCircle;
			this->AddPowerBeHit=pRowData->AddPowerBeHit;

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
		const char *Headers[] = { "HeroID", "Buff1", "Buff2", "Buff3", "AddPowerPerSecond", "AddPowerInCircle", "AddPowerBeHit",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(HeroBattleAncientPower  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.Buff1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.Buff2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Buff3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.AddPowerPerSecond)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.AddPowerInCircle)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.AddPowerBeHit)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		HeroID_Index[row.HeroID] = Table.size()-1;


		return true;
	}

	~HeroBattleAncientPower ()
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