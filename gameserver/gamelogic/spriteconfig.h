#ifndef __SPIRITCONFIG_H__
#define __SPIRITCONFIG_H__

#include "table/SpriteTable.h"
#include "table/SpriteLevel.h"
#include "table/SpriteEvolution.h"
#include "table/SpriteSkill.h"
#include "table/SpritePassiveSkillBank.h"

typedef SpriteTable::RowData SpriteConf;

#define SpriteBasicAttrNum 5


struct SpriteLevelInfo
{
	SpriteLevelInfo(){}
	SpriteLevelInfo(UINT32 _level, UINT32 _exp, double _ratioSum):level(_level), exp(_exp), ratioSum(_ratioSum){}

	bool operator < (const SpriteLevelInfo& info) const
	{
		return level < info.level;
	}

	UINT32 level;
	UINT32 exp;
	UINT32 expSum;
	double ratioSum;
};

struct SpriteEvolutionInfo
{	
	SpriteEvolutionInfo(){}
	SpriteEvolutionInfo(UINT32 _evolutionLevel, UINT32 _levelLimit, Sequence<UINT32, 2>& _cost, Sequence<UINT32, 2>& _trainExp, 
		std::vector<Sequence<UINT32, 2>>& _resetTrainCost, std::vector<Sequence<UINT32, 2>>& _attr, std::vector<Sequence<UINT32, 3>> _trainAttr)
		:evolutionLevel(_evolutionLevel), levelLimit(_levelLimit), cost(_cost), trainExp(_trainExp), resetTrainCost(_resetTrainCost), 
		attr(_attr), trainAttr(_trainAttr){}

	bool operator < (const SpriteEvolutionInfo& info) const
	{
		return evolutionLevel < info.evolutionLevel;
	}

	UINT32 evolutionLevel;
	UINT32 levelLimit;
	Sequence<UINT32, 2> cost;
	Sequence<UINT32, 2> trainExp;
	std::vector<Sequence<UINT32, 2>> resetTrainCost;
	std::vector<Sequence<UINT32, 2>> attr;
	std::vector<Sequence<UINT32, 3>> trainAttr;
};

struct SpriteRandomPassiveSkill
{
	SpriteRandomPassiveSkill(){}
	SpriteRandomPassiveSkill(UINT32 _skillEffectID, UINT32 _skillID, UINT32 _weight):skillEffectID(_skillEffectID), skillID(_skillID), 
		weight(_weight){}

	UINT32 skillEffectID;
	UINT32 skillID;
	UINT32 weight;
};

struct SpriteSkillInfo
{
	SpriteSkillInfo(){}
	SpriteSkillInfo(UINT32 _skillID, UINT32 _skillQuality, UINT32 _passiveSkillType, std::string _skillName, std::string _noticeDetail, Sequence<int, 2> _bShowNotice, 
		std::vector<Sequence<UINT32, 2>>& _attr, std::vector<Sequence<UINT32,2>>& _baseAttr, std::vector<Sequence<UINT32, 2>>& _buff):
			skillID(_skillID), skillQuality(_skillQuality), passiveSkillType(_passiveSkillType), skillName(_skillName), noticeDetail(_noticeDetail), 
			attr(_attr), baseAttr(_baseAttr), buff(_buff)
			{
				bShowNotice = (_bShowNotice[0] != 0);
			}

	UINT32 skillID;
	UINT32 skillQuality;
	UINT32 passiveSkillType;
	std::string skillName;
	std::string noticeDetail;
	std::vector<Sequence<UINT32,2>> attr;
	std::vector<Sequence<UINT32,2>> baseAttr;
	std::vector<Sequence<UINT32,2>> buff;
	bool bShowNotice;
};

class SpriteTableData
{
public:
	SpriteTableData();
	~SpriteTableData();

	bool HandlerTableData();

protected:
	SpriteTable m_oSpriteTable;
	SpriteLevel m_oSpriteLevelTable;
	SpriteEvolution m_oSpriteEvolution;

	SpriteSkill m_oSpriteSkillTable;
	SpritePassiveSkillBank m_oSpritePassiveSkillBank;

	std::unordered_map<UINT32, UINT32> m_id2index;
	std::unordered_map<UINT32, std::vector<SpriteLevelInfo>> m_level; // key:quality, vector size is level
	std::unordered_map<UINT32, std::vector<SpriteEvolutionInfo>> m_evolution; // key:spriteid, vector size is evolution_level
	std::unordered_map<UINT32, std::vector<SpriteRandomPassiveSkill>> m_passiveSkill;
	std::unordered_map<UINT32, std::vector<SpriteSkillInfo>> m_skill;
};

class SpriteConfig : public SpriteTableData
{
	SpriteConfig();
	~SpriteConfig();
	DECLARE_SINGLETON(SpriteConfig)
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	UINT32 GetSpriteIndex(UINT32 spriteID);

	UINT32 GetSpriteSize(){ return m_oSpriteTable.Table.size(); };

	UINT32 GetSpriteFoodExp(UINT32 itemID);

	UINT32 GetSpriteQuality(UINT32 spriteID);

	const SpriteConf* GetSpriteConf(UINT32 spriteID){ return m_oSpriteTable.GetBySpriteID(spriteID); }

	const SpriteLevelInfo* GetSpriteLevelInfo(UINT32 quality, UINT32 level);

	const SpriteEvolutionInfo* GetSpriteEvolutionInfo(UINT32 spriteID, UINT32 evolutionLevel);

	const SpriteSkillInfo* GetSpriteSkillInfo(UINT32 skillID, UINT32 evoLevel);

	void GetRandomPassiveSkillID(UINT32 skillBankID, UINT32 num, std::vector<UINT32>& skills);

	void GetAllEvolutionCost(UINT32 spriteID, UINT32 evolutionLevel, UINT32 trainExp, std::map<UINT32, UINT32>& allcost);

	void GetAllExpCost(UINT32 quality, UINT32 level, UINT32 exp, std::map<UINT32, UINT32>& allcost);
	
};


#endif