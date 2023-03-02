#ifndef __SPRITE_H__
#define __SPRITE_H__


namespace KKSG
{
	class SpriteInfo;
}

struct SpriteTLogVar
{
	SpriteTLogVar()
	{
		m_iOperType = -1;
		m_iChangeValue = 0;	
		m_iReason = -1;
		m_iSubReason = -1;
	}	
	int m_iOperType;
	int m_iChangeValue;
	int m_iReason;
	int m_iSubReason;

};

class Role;
class CombatAttribute;
class TSpriteFlow;

class Sprite
{
public:
	Sprite(Role* pRole);
	Sprite(UINT64 id, UINT32 spriteid, Role* pRole);
	~Sprite();

	void KKSGToSprite(const KKSG::SpriteInfo& infoData);
	void SpriteToKKSG(KKSG::SpriteInfo* infoData);	

	void ApplyEffect(CombatAttribute* combatAttr);

	void RecalculateAttr();

	KKSG::ErrorCode CheckAwake();
	void AwakeRandom();

	KKSG::ErrorCode AddExp(UINT32 itemID);

	KKSG::ErrorCode Train(const std::vector<UINT32>& notToChoose, std::vector<std::pair<UINT32, UINT32>>& addAttr);
	KKSG::ErrorCode ResetTrain(UINT32 chooseid, bool needCost = true);
	KKSG::ErrorCode Rebirth(bool needCost = true);

	KKSG::ErrorCode EvolutionLevelUp();
	KKSG::ErrorCode GetEvolutionLevelUpPPT(UINT32& ppt);
	
	inline UINT64 Getuid() const { return m_uid; }
	inline UINT32 GetSpriteID() const { return m_spriteID; }
	inline UINT32 GetSkillID() const { return m_skillID; }
	inline UINT32 GetLevel() const { return m_level; }
	inline UINT32 GetEvolutionLevel() const { return m_evolutionLevel; }
	inline UINT32 GetExp() const { return m_exp; }	
	inline UINT32 GetPowerPoint() const { return m_powerpoint; }
	inline UINT32 GetTrainExp() const { return m_trainExp; }

	void SetLevel(UINT32 level) { m_level = level; }

	void NotifyChangeSprite();

	void PrintTLog(const SpriteTLogVar& var, const std::string& trans="");
	void PrintTLogOut(const std::string& trans);

private:
	void NewSprite(UINT64 id, UINT32 spriteid);	

private:
	Role* m_pRole;
	UINT64 m_uid;
	UINT32 m_spriteID;
	UINT32 m_skillID;	
	UINT32 m_level;
	UINT32 m_evolutionLevel;
	UINT32 m_exp;
	UINT32 m_powerpoint;
	UINT32 m_trainExp;
	std::vector<UINT32> m_passiveSkillID;
	std::unordered_map<UINT32, double> m_addAttr;
	std::unordered_map<UINT32, double> m_attr;
	std::unordered_map<UINT32, double> m_evoAttr;
	std::unordered_map<UINT32, double> m_thisLevelEvoAttr;
};


#endif