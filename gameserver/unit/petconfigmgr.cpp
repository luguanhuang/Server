#include "pch.h"
#include "petconfigmgr.h"
#include "util/XRandom.h"
#include "gamelogic/itemsuffix.h"
#include "foreach.h"

INSTANCE_SINGLETON(PetConfigMgr);

PetConfigMgr::PetConfigMgr()
{

}

PetConfigMgr::~PetConfigMgr()
{

}

bool PetConfigMgr::Init()
{
	return LoadFile();
}

void PetConfigMgr::Uninit()
{

}

static bool HandleTableData(PetInfoTable& table, PetLevelTable& leveltable, PetMoodTipsTable& moodtable,
					PetPassiveSkillTable& skilltable, PetSkillBook& skillbook)
{
	if (!table.LoadFile("table/PetInfo.txt"))
	{
		SSWarn<<"load file table/PetInfo.txt failed"<<END;
		return false;
	}
	if (!leveltable.LoadFile("table/PetLevel.txt"))
	{
		SSWarn<<"load file table/PetLevel.txt failed"<<END;
		return false;
	}
	if (!skilltable.LoadFile("table/PetPassiveSkill.txt"))
	{
		SSWarn<<"load file table/PetPassiveSkill.txt failed"<<END;
		return false;
	}
	if (!moodtable.LoadFile("table/PetMoodTips.txt"))
	{
		SSWarn<<"load file table/PetMoodTips.txt failed"<<END;
		return false;
	}
	if (!skillbook.LoadFile("table/PetSkillBook.txt"))
	{
		SSWarn<<"load file table/PetSkillBook.txt failed"<<END;
		return false;
	}
	return true;
}

bool PetConfigMgr::CheckFile()
{
	PetInfoTable table;
	PetLevelTable leveltable;
	PetMoodTipsTable moodtable;
	PetPassiveSkillTable skilltable;
	PetSkillBook skillbook;
	return HandleTableData(table, leveltable, moodtable, skilltable, skillbook);
}

bool PetConfigMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	HandleTableData(m_table, m_leveltable, m_moodtable, m_skilltable, m_skillbook);

	return true;
}

void PetConfigMgr::ClearFile()
{
	m_table.Clear();
	m_leveltable.Clear();
	m_moodtable.Clear();
	m_skilltable.Clear();
	m_skillbook.Clear();
}

bool PetConfigMgr::CanLevelUp(UINT32 petId, UINT32 level, UINT32& exp)
{
	foreach (i in m_leveltable.Table)
	{
		if ((*i)->PetsID == petId && (*i)->level == level + 1)
		{
			if (exp >= (*i)->exp)
			{
				exp -= (*i)->exp;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}
/*
UINT32 PetConfigMgr::RandomPassiveSkill(UINT32 petid)
{

PetInfoTable::RowData* data = m_table.GetByid(petid);
UINT32 quality = 0;
UINT32 random = (UINT32)XRandom::randInt(1, 101);
UINT32 sum = 0;
for (auto i = data->passiveskills.begin(); i != data->passiveskills.end(); ++i)
{
sum += i->seq[1];
if (random <= sum)
{
quality = i->seq[0];
break;
}
}
auto j = m_randomskills.find(quality);
if (j == m_randomskills.end())
{
return 0;
}
random = (UINT32)XRandom::randInt(1, 101);
sum = 0;
for (auto k = j->second.begin(); k != j->second.end(); ++k)
{
sum += k->prob;
if (random <= sum)
{
return k->skillid;
}
}
return 0;

}*/
/*
std::vector<ItemChangeAttr>* PetConfigMgr::GetPassiveSkillAttrs(UINT32 skillid, UINT32 level)
{

auto i = m_skillattrs.find((skillid << 4) + level);
if (i == m_skillattrs.end())
{
return NULL;
}
return &i->second;

}
*/
/*
bool PetConfigMgr::IsSkillLevelLimit(UINT32 skillid, UINT32 level)
{

auto i = m_skilllevels.find(skillid);
if (i == m_skilllevels.end())
{
return false;
}
return level >= i->second;

}

int PetConfigMgr::RandomMood(UINT32 mood)
{
PetMoodTipsTable::RowData* rowdata = GetMood(mood);
if (NULL == rowdata)
{
return 0;
}
int offset = 0;
if (rowdata->hourchange[0] < 0)
{
offset = -rowdata->hourchange[0];
}
int random = (int)XRandom::randInt(rowdata->hourchange[0] + offset, rowdata->hourchange[1] + offset);
return random - offset;
}
*/

int PetConfigMgr::GetRandomMood(UINT32 mood)
{
	PetMoodTipsTable::RowData* rowdata = GetMood(mood);
	if (NULL == rowdata)
	{
		return 0;
	}

	UINT32 num = rowdata->hourchange[0] > rowdata->hourchange[1] ? (rowdata->hourchange[0] - rowdata->hourchange[1] + 1) :(rowdata->hourchange[1] - rowdata->hourchange[0] + 1);
	return rowdata->hourchange[0] > rowdata->hourchange[1] ? (rowdata->hourchange[1] + XRandom::randInt(0, num)):(rowdata->hourchange[0] + XRandom::randInt(0, num)); 
}

UINT32 PetConfigMgr::GetComprehend(UINT32 mood)
{
	PetMoodTipsTable::RowData* rowdata = GetMood(mood);
	if (NULL == rowdata)
	{
		return 0;
	}

	return rowdata->comprehend;
}

bool PetConfigMgr::IsMoodHit(UINT32 mood)
{
	PetMoodTipsTable::RowData* rowdata = GetMood(mood);
	if (NULL == rowdata)
	{
		return false;
	}
	UINT32 random = XRandom::randInt(1, 101);
	return random <= rowdata->comprehend;
}

PetMoodTipsTable::RowData* PetConfigMgr::GetMood(UINT32 mood)
{
	for (auto i = m_moodtable.Table.rbegin(); i != m_moodtable.Table.rend(); ++i)
	{
		PetMoodTipsTable::RowData* rowdata = *i;
		if ((int)mood >= rowdata->value)
		{
			return rowdata;
		}
	}
	return NULL;
}

void PetConfigMgr::GetAllAttrs(UINT32 petId, UINT32 petLvl, const std::vector<UINT32>& skillIdVec, std::vector<ItemChangeAttr>& attrsVec)
{
	std::map<UINT32, UINT32> attrsMap;
	for (UINT32 cnt = 1; cnt <= petLvl; ++cnt)
	{
		PetLevelTable::RowData* pRowData = _GetPetLvlRowData(petId, cnt);
		if (pRowData != NULL)
		{
			foreach (i in pRowData->PetsAttributes)
			{
				attrsMap[(*i)[0]] += (*i)[1];
			}
		}
	}

	foreach (j in skillIdVec)
	{
		PetPassiveSkillTable::RowData* pData = _GetSkillRowData(*j);
		if (pData != NULL)
		{
			foreach (k in pData->attrs)
			{
				attrsMap[(*k)[0]] += (*k)[1];
			}

			foreach (x in pData->attrlvl)
			{
				attrsMap[(*x)[0]] += (*x)[1] * petLvl;
			}
		}
	}

	foreach (x in attrsMap)
	{
		ItemChangeAttr attr(x->first, x->second);
		attrsVec.push_back(attr);
	}
}

bool PetConfigMgr::ExistPetId(UINT32 petId)
{
	PetInfoTable::RowData* data = m_table.GetByid(petId);
	return data == NULL ? false : true;
}

PetLevelTable::RowData* PetConfigMgr::_GetPetLvlRowData(UINT32 petId, UINT32 petLvl)
{
	foreach (i in m_leveltable.Table)
	{
		if ((*i)->PetsID == petId && (*i)->level == petLvl)
		{
			return (*i);
		}
	}
	return NULL;
}

PetPassiveSkillTable::RowData* PetConfigMgr::_GetSkillRowData(UINT32 skillId)
{
	return m_skilltable.GetByid(skillId);
}

PetInfoTable::RowData* PetConfigMgr::GetPetInfoData(UINT32 petId)
{
	return m_table.GetByid(petId);
}

PetSkillBook::RowData* PetConfigMgr::GetPetBookData(UINT32 bookId)
{
	return m_skillbook.GetByitemid(bookId);
}

UINT32 PetConfigMgr::GetInitMood(UINT32 petId)
{
	PetInfoTable::RowData* pRowData = GetPetInfoData(petId);
	if (pRowData == NULL)
	{
		return 0;
	}
	return pRowData->initMood;
}

UINT32 PetConfigMgr::GetInitHungry(UINT32 petId)
{
	PetInfoTable::RowData* pRowData = GetPetInfoData(petId);
	if (pRowData == NULL)
	{
		return 0;
	}
	return pRowData->initHungry;
}

UINT32 PetConfigMgr::GetRandSkillMax(UINT32 petId)
{
	PetInfoTable::RowData* pRowData = GetPetInfoData(petId);
	if (pRowData == NULL)
	{
		return 0;
	}
	return pRowData->randSkillMax;
}

UINT32 PetConfigMgr::GetRandSkillRet(UINT32 petId)
{
	PetInfoTable::RowData* pRowData = GetPetInfoData(petId);
	if (pRowData == NULL)
	{
		return 0;
	}
	return pRowData->LevelupInsight;
}

UINT32 PetConfigMgr::GetRandSkillQuality(UINT32 petId)
{
	PetInfoTable::RowData* pRowData = GetPetInfoData(petId);
	if (pRowData == NULL)
	{
		return 0;
	}

	UINT32 wight = 0;
	foreach (i in pRowData->randSkills)
	{
		wight += i->seq[1];
	}

	UINT32 randNum = XRandom::randInt(0, wight);

	wight = 0;
	foreach (i in pRowData->randSkills)
	{
		wight += i->seq[1];
		if (randNum < wight)
		{
			return i->seq[0];
		}
	}

	return 0;
}

void PetConfigMgr::GetQualitySkill(UINT32 petId, UINT32 mood, std::map<UINT32, UINT32>& skillsMap, bool isMast)
{
	skillsMap.clear();

	if (!isMast)
	{
		UINT32 randNum = XRandom::randInt(0, 10000);
		if (randNum > GetRandSkillRet(petId) * GetComprehend (mood))
		{
			return ;
		}
	}

	UINT32 quality = GetRandSkillQuality(petId); 
	std::set<UINT32> fixedSkillSet;
	GetFixedSkills(petId, fixedSkillSet);

	foreach (i in m_skilltable.Table)
	{
		if ((*i)->quality == quality && fixedSkillSet.find((*i)->id) == fixedSkillSet.end())
		{
			skillsMap[(*i)->id] = (*i)->probability;
		}
	}
}

void PetConfigMgr::GetFixedSkills(UINT32 petId, std::set<UINT32>& skillSet)
{
	PetInfoTable::RowData* data = m_table.GetByid(petId);
	if (data == NULL) return;

	foreach (j in data->skill1)
	{
		skillSet.insert(j->seq[0]);
	}
	foreach (k in data->skill2)
	{
		skillSet.insert(k->seq[0]);
	}
	foreach (n in data->skill3)
	{
		skillSet.insert(n->seq[0]);
	}
}

UINT32 PetConfigMgr::GetBuffId(UINT32 petId)
{
	PetInfoTable::RowData* pRowData = GetPetInfoData(petId);
	if (pRowData == NULL)
	{
		return 0;
	}
	return pRowData->SpeedBuff;
}

UINT32 PetConfigMgr::GetFixedSkillId(UINT32 petId, UINT32 lvl)
{
	PetInfoTable::RowData* pRowData = GetPetInfoData(petId);
	if (pRowData == NULL)
	{
		return 0;
	}

	UINT32 cnt = 0;
	foreach (i in pRowData->LvRequire)
	{
		++cnt;
		if (lvl == *i)
		{
			vector<Sequence<uint, 2>>* pData;
			if (cnt == 1)
			{
				pData = &pRowData->skill1;
			}
			else if (cnt == 2)
			{
				pData = &pRowData->skill2;
			}
			else
			{
				pData = &pRowData->skill3;
			}

			UINT32 wight = 0;
			foreach (j in (*pData))
			{
				wight += j->seq[1];
			}

			if (wight == 0)
			{
				return 0;
			}
			UINT32 randNum = XRandom::randInt(0, wight);
			wight = 0;
			foreach (x in (*pData))
			{
				wight += x->seq[1];
				if (randNum < wight)
				{
					return x->seq[0];
				}
			}
		}
	}

	return 0;
}

int PetConfigMgr::GetHungryAttr(UINT32 petId, UINT32 hungry)
{
	PetInfoTable::RowData* data = m_table.GetByid(petId);
	if (data == NULL) return 0;

	foreach (j in data->hungryAttr)
	{
		if (hungry <= j->seq[0])
		{
			return j->seq[1];
		}
	}
	return 0;
}


UINT32 PetConfigMgr::GetExpByLevel(const UINT32 pet_id,const UINT32 level)
{
	UINT32 total_exp = 0;
	auto it = m_leveltable.Table.begin();
	auto it_end = m_leveltable.Table.end();
	for (; it != it_end; ++it)
	{
		if ((*it)->PetsID == pet_id &&
			(*it)->level <= level)
		{
			total_exp += (*it)->exp;
		}	
	}

	return total_exp;
}

UINT32 PetConfigMgr::GetMale(UINT32 petId)
{
	PetInfoTable::RowData* pRowData = GetPetInfoData(petId);
	if (pRowData != NULL)
	{
		return pRowData->Male;
	}
	return 0;
}

bool PetConfigMgr::CanPairRide(UINT32 petid)
{
	PetInfoTable::RowData* pRowData = GetPetInfoData(petid);
	if (pRowData != NULL)
	{
		return pRowData->PetType == 1;
	}
	return false;
}
