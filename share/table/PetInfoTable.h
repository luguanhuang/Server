#ifndef __PetInfoTable_H__
#define __PetInfoTable_H__

#include "pch.h"
#include "cvsreader.h"
#include "config.h"
#include "noncopyable.h"

// generate by ProtoGen at date: 2017/10/25 20:26:30


class PetInfoTable : public CVSReader, public noncopyable
{
public:

	struct RowData
	{
		uint	id;
		string	name;
		uint	quality;
		uint	touchAttr;
		uint	touchHourMax;
		uint	touchDayMax;
		uint	followAttr;
		uint	followDayMax;
		int	hungryAuto;
		vector<uint>	Star;
		vector<uint>	LvRequire;
		vector<Sequence<uint, 2>>	skill1;
		vector<Sequence<uint, 2>>	skill2;
		vector<Sequence<uint, 2>>	skill3;
		uint	LevelupInsight;
		vector<Sequence<uint, 2>>	randSkills;
		uint	SpeedBuff;
		string	avatar;
		string	icon;
		uint	randSkillMax;
		uint	initHungry;
		uint	maxHungry;
		uint	initMood;
		uint	maxMood;
		uint	presentID;
		vector<Sequence<int, 2>>	hungryAttr;
		vector<Sequence<uint, 2>>	MinimumInsight;
		uint	Male;
		uint	PetType;
		uint	WithWings;
		string	Atlas;

		void CopyFrom(RowData *pRowData)
		{
			this->id=pRowData->id;
			this->name=pRowData->name;
			this->quality=pRowData->quality;
			this->touchAttr=pRowData->touchAttr;
			this->touchHourMax=pRowData->touchHourMax;
			this->touchDayMax=pRowData->touchDayMax;
			this->followAttr=pRowData->followAttr;
			this->followDayMax=pRowData->followDayMax;
			this->hungryAuto=pRowData->hungryAuto;
			this->Star=pRowData->Star;
			this->LvRequire=pRowData->LvRequire;
			this->skill1=pRowData->skill1;
			this->skill2=pRowData->skill2;
			this->skill3=pRowData->skill3;
			this->LevelupInsight=pRowData->LevelupInsight;
			this->randSkills=pRowData->randSkills;
			this->SpeedBuff=pRowData->SpeedBuff;
			this->avatar=pRowData->avatar;
			this->icon=pRowData->icon;
			this->randSkillMax=pRowData->randSkillMax;
			this->initHungry=pRowData->initHungry;
			this->maxHungry=pRowData->maxHungry;
			this->initMood=pRowData->initMood;
			this->maxMood=pRowData->maxMood;
			this->presentID=pRowData->presentID;
			this->hungryAttr=pRowData->hungryAttr;
			this->MinimumInsight=pRowData->MinimumInsight;
			this->Male=pRowData->Male;
			this->PetType=pRowData->PetType;
			this->WithWings=pRowData->WithWings;
			this->Atlas=pRowData->Atlas;

		}
	};

	std::unordered_map<uint,int> id_Index;


    RowData *GetByid(const uint &key)
    {
        auto i = id_Index.find(key);
        return (i == id_Index.end()) ? NULL : Table[i->second];
    }
    

	virtual bool OnHeaderLine(char **Fields, int FieldCount)
	{
		const char *Headers[] = { "id", "name", "quality", "touchAttr", "touchHourMax", "touchDayMax", "followAttr", "followDayMax", "hungryAuto", "Star", "LvRequire", "skill1", "skill2", "skill3", "LevelupInsight", "randSkills", "SpeedBuff", "avatar", "icon", "randSkillMax", "initHungry", "maxHungry", "initMood", "maxMood", "presentID", "hungryAttr", "MinimumInsight", "Male", "PetType", "WingActive", "Atlas",  };
		const int HeadersCount = sizeof(Headers)/sizeof(Headers[0]);
		//assert(FieldCount == HeadersCount);
		return MapColHeader(Headers, HeadersCount, Fields, FieldCount);
	}

	virtual bool OnCommentLine(char **Fields, int FieldCount)
	{
		return true;
	}

	void CopyFrom(PetInfoTable  *pSrcTable)
	{
		for (UINT32 i = 0; i< pSrcTable->Table.size(); i++)
		{
			RowData* pSrcRow = pSrcTable->Table[i];
            RowData *pDstRow = GetByid(pSrcRow->id);
            if(pDstRow != NULL)
            {
                pDstRow->CopyFrom(pSrcRow);
            }
            else
            {
                RowData* pNewRow = new RowData();
                pNewRow->CopyFrom(pSrcRow);
                Table.push_back(pNewRow);
                id_Index[pNewRow->id]=Table.size()-1;
            }

		}
	}

	virtual bool OnLine(char **Fields, int FieldCount)
	{
		RowData *pRow = new RowData();
		RowData &row = *pRow;
		m_ColNum = 0;

		if (!Parse(Fields[m_ColMap[0]], row.id)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[1]], row.name)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[2]], row.quality)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[3]], row.touchAttr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[4]], row.touchHourMax)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[5]], row.touchDayMax)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[6]], row.followAttr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[7]], row.followDayMax)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[8]], row.hungryAuto)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[9]], row.Star)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[10]], row.LvRequire)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[11]], row.skill1)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[12]], row.skill2)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[13]], row.skill3)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[14]], row.LevelupInsight)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[15]], row.randSkills)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[16]], row.SpeedBuff)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[17]], row.avatar)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[18]], row.icon)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[19]], row.randSkillMax)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[20]], row.initHungry)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[21]], row.maxHungry)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[22]], row.initMood)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[23]], row.maxMood)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[24]], row.presentID)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[25]], row.hungryAttr)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[26]], row.MinimumInsight)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[27]], row.Male)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[28]], row.PetType)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[29]], row.WithWings)) return false; ++m_ColNum;
		if (!Parse(Fields[m_ColMap[30]], row.Atlas)) return false; ++m_ColNum;

		
		Table.push_back(pRow);

		id_Index[row.id] = Table.size()-1;


		return true;
	}

	~PetInfoTable ()
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

		id_Index.clear();

	}

	std::vector<RowData*> Table;
};

#endif