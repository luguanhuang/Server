#ifndef __AICONFIG_H__
#define __AICONFIG_H__

#include "table/SkillCombo.h"
#include "table/HateList.h"
#include "unit/unit.h"

class AIConfig
{
	AIConfig();
	~AIConfig();
	DECLARE_SINGLETON(AIConfig)


public:

	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	SkillCombo::RowData* GetRowData(UINT32 index);
	SkillCombo::RowData* GetRowData(string name);

	bool HasStartSkill(int profIndex);
	bool HasComboSkill(std::string skillname);
	std::vector<std::string>& GetStartSkill(int profIndex);
	std::vector<std::string>& GetStartSkillById(int profIndex);
	std::vector<std::string>& GetComboSkill(std::string skillname);
	int GetProfIndex(Unit* unit);
	UINT32 GetHateValue(UINT32 casterType, UINT32 receiverType);
private:
	std::map<int, std::vector<std::string>> StartSkills;
	std::map<std::string, std::vector<std::string>> NextSkills;
	std::vector<std::string>Skills;
	SkillCombo m_oTable;
	HateList m_oHateListTable;

	typedef std::map<UINT32, UINT32> HateInfo;
	struct HateData
	{
		HateInfo normalInfo;	///> 也许以后会不同场景有不同的仇恨值
	};
	std::map<UINT32, HateData> type2hateData;
};


class AISKillData
{
	AISKillData();
	~AISKillData();
	DECLARE_SINGLETON(AISKillData)


public:
	bool HasStartSkill(int profIndex);
	bool HasComboSkill(std::string skillname);
	std::vector<std::string>& GetStartSkill(int profIndex);
	std::vector<std::string>& GetComboSkill(std::string skillname);
	int GetProfIndex(Unit* unit);
};


#endif // __RANDOMTASKCONFIG_H__