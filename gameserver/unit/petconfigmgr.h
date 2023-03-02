#ifndef __PETCONFIGMGR_H__
#define __PETCONFIGMGR_H__

#include "table/PetInfoTable.h"
#include "table/PetLevelTable.h"
#include "table/PetPassiveSkillTable.h"
#include "table/PetMoodTipsTable.h"
#include "table/PetSkillBook.h"

struct RandomPetSkill
{
	UINT32 skillid;
	UINT32 prob;
};

struct ItemChangeAttr;
class PetConfigMgr
{
	PetConfigMgr();
	~PetConfigMgr();
	DECLARE_SINGLETON(PetConfigMgr);
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	bool CanLevelUp(UINT32 petId, UINT32 level, UINT32& exp);

	PetMoodTipsTable::RowData* GetMood(UINT32 mood);
	//int RandomMood(UINT32 mood);
	bool IsMoodHit(UINT32 mood);
	bool CanPairRide(UINT32 petid);

	//UINT32 RandomPassiveSkill(UINT32 petid);
	//bool IsSkillLevelLimit(UINT32 skillid, UINT32 level);
	//std::vector<ItemChangeAttr>* GetPassiveSkillAttrs(UINT32 skillid, UINT32 level);

	UINT32 GetExpByLevel(const UINT32 pet_id,const UINT32 level);
public:
	void GetAllAttrs(UINT32 petId, UINT32 petLvl, const std::vector<UINT32>& skillIdVec, std::vector<ItemChangeAttr>& attrsVec);
	bool ExistPetId(UINT32 petId);
	PetInfoTable::RowData* GetPetInfoData(UINT32 petId);
	PetSkillBook::RowData* GetPetBookData(UINT32 bookId); 
	UINT32 GetInitMood(UINT32 petId);
	UINT32 GetInitHungry(UINT32 petId);
	UINT32 GetRandSkillMax(UINT32 petId);
	UINT32 GetRandSkillRet(UINT32 petId);
	UINT32 GetRandSkillQuality(UINT32 petId);
	void GetQualitySkill(UINT32 petId, UINT32 mood, std::map<UINT32, UINT32>& skillsMap, bool isMast = false);
	UINT32 GetBuffId(UINT32 petId);
	UINT32 GetFixedSkillId(UINT32 petId, UINT32 lvl);
	int GetHungryAttr(UINT32 petId, UINT32 hungry);
	int GetRandomMood(UINT32 mood);
	UINT32 GetComprehend(UINT32 mood);
	UINT32 GetMale(UINT32 petId);
	void GetFixedSkills(UINT32 petId, std::set<UINT32>& skillSet);

private:
	PetLevelTable::RowData* _GetPetLvlRowData(UINT32 petId, UINT32 petLvl);
	PetPassiveSkillTable::RowData* _GetSkillRowData(UINT32 skillId);

private:
	PetInfoTable m_table;
	PetLevelTable m_leveltable;
	PetMoodTipsTable m_moodtable;
	PetPassiveSkillTable m_skilltable;
	PetSkillBook m_skillbook;


	/*std::map<UINT32, UINT32> m_skilllevels;
	std::map<UINT32, std::vector<ItemChangeAttr>> m_skillattrs;
	std::map<UINT32, std::vector<RandomPetSkill>> m_randomskills;*/
};

#endif