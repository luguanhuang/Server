#ifndef __HeroBattleExperienceHero_H__
#define __HeroBattleExperienceHero_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/4/14 20:19:25


class HeroBattleExperienceHero : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	ItemID;
		uint	HeroID;
		uint	LastTime;
		vector<Sequence<uint, 2>>	Reward;
		string	ShowTime;

		void CopyFrom(RowData *pRowData)
		{
			this->ItemID=pRowData->ItemID;
			this->HeroID=pRowData->HeroID;
			this->LastTime=pRowData->LastTime;
			this->Reward=pRowData->Reward;
			this->ShowTime=pRowData->ShowTime;

		}
	};

	std::unordered_map<uint,int> ItemID_Index;


    RowData *GetByItemID(const uint &key)
    {
        auto i = ItemID_Index.find(key);
        return (i == ItemID_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "ItemID", "HeroID", "LastTime", "Reward", "ShowTime",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(HeroBattleExperienceHero  *pSrcTable)
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
		if (!Parse(Fields[m_ColMap[1]], row.HeroID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.LastTime)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.Reward)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.ShowTime)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		ItemID_Index[row.ItemID] = Table.size()-1;


		return true;
	}

	~HeroBattleExperienceHero ()
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