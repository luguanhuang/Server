#ifndef __AINODESKILL_H__
#define __AINODESKILL_H__

#include "ainode.h"
#include "ainodeaction.h"
#include "aibehaviortree.h"
#include "component/XAIComponent.h"
#include "ainodecondition.h"

class XSkillCore;

class FilterSkill : public AINodeAction
{
public:
	FilterSkill(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

	bool BindSkill(XAIComponent* ai, const vector<XSkillCore*>& v);
	bool BindLinkedSkill(XAIComponent* ai, const vector<XSkillCore*>& v);
	bool ResetSkill(Unit* unit, AIBehaviorTree* tree, XAIComponent* ai, bool useinstall);
	bool FilterSkillByMP(Unit* unit, AIBehaviorTree* tree, XAIComponent* ai);
	bool FilterSkillByName(Unit* unit, AIBehaviorTree* tree, XAIComponent* ai, string name);
	bool FilterSkillByHP(Unit* unit, AIBehaviorTree* tree, XAIComponent* ai);
	bool FilterSkillByCoolDown(Unit* unit, AIBehaviorTree* tree, XAIComponent* ai);
	bool FilterSkillByAttackField(Unit* unit, AIBehaviorTree* tree, XAIComponent* ai, Unit* target);
	bool FilterSkillByCombo(Unit* unit, AIBehaviorTree* tree, XAIComponent* ai, int skillType);

private:
	std::string m_sharedTargetName;
	UINT32 m_targetIndex;
	bool m_useMP;
	bool m_useName;
	bool m_useHP;
	bool m_useCoolDown;
	bool m_useAttackField;
	bool m_useCombo;
	bool m_useInstall;
	int m_ComboSkillType;
	std::string m_skillName;
	bool m_useDetectAllInAttackField;
	int m_maxSkillNum;
};

class DoSelectSkillInOrder : public AINodeAction
{
public:
	DoSelectSkillInOrder(const tinyxml2::XMLElement* elem){};
	virtual bool Update(Unit* unit);
};

class DoSelectSkillRandom : public AINodeAction
{
public:
	DoSelectSkillRandom(const tinyxml2::XMLElement* elem){};
	virtual bool Update(Unit* unit);
};


class PhysicalAttack:public AINodeAction
{
public:
	PhysicalAttack(const tinyxml2::XMLElement *elem);

	virtual bool Update(Unit* unit);
	virtual void Print();

private:
	std::string m_sharedTargetName;
	UINT32 m_targetIndex;
};

class DoCastSkill : public AINodeAction
{
public:
	DoCastSkill(const tinyxml2::XMLElement* elem);

	virtual bool Update(Unit* unit);

private:
	std::string m_sharedTargetName;
	UINT32 m_targetIndex;
};

class IsSkillChoosed : public AINodeCondition
{
public:
	IsSkillChoosed(const tinyxml2::XMLElement* elem){};
	virtual bool Update(Unit* unit);
};

class TryCastQTE:public AINodeAction
{
public:
	TryCastQTE(const tinyxml2::XMLElement* elem){}

	virtual bool Update(Unit* unit);
};

class CastDash:public AINodeAction
{
public:
	CastDash(const tinyxml2::XMLElement* elem){}

	virtual bool Update(Unit* unit);
};

class StopCastingSkill : public AINodeAction
{
public: 
	StopCastingSkill(const tinyxml2::XMLElement* elem){};
	virtual bool Update(Unit* unit);
};

#endif

