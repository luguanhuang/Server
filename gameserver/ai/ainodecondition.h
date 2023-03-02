#ifndef __AINODECONDITION_H__
#define __AINODECONDITION_H__

#include "ainode.h"

/*全局数据*/
class Unit;

typedef Unit* Transform;

class AINodeCondition:public AINodeBase
{
public:
	AINodeCondition(){m_type = NODE_TYPE_CONDITION;}
	virtual ~AINodeCondition(){}
};

class ValueTarget : public AINodeCondition
{
public:
	ValueTarget(const tinyxml2::XMLElement* elem);

	virtual bool Update(Unit* unit);
	virtual void Print();

private:
	std::string m_sharedTargetName;
};

class ValueDistance : public AINodeCondition
{
public:
	ValueDistance(const tinyxml2::XMLElement* elem);

	virtual bool Update(Unit* unit);
	virtual void Print();

private:
	std::string m_sharedTargetName;
	std::string m_sharedMaxDistanceName;
};

class ValueHP:public AINodeCondition
{
public:
	ValueHP(const tinyxml2::XMLElement* elem);

	virtual bool Update(Unit* unit);
	virtual void Print();

private:
	UINT32 m_maxPercent;
	UINT32 m_minPercent;
};

class ValueMP:public AINodeCondition
{
public:
	ValueMP(const tinyxml2::XMLElement* elem);

	virtual bool Update(Unit* unit);
	virtual void Print();

private:
	UINT32 m_maxPercent;
	UINT32 m_minPercent;
};

class ValueFP:public AINodeCondition
{
public:
	ValueFP(const tinyxml2::XMLElement* elem);

	virtual bool Update(Unit* unit);
	virtual void Print();

private:
	UINT32 m_maxFP;
	UINT32 m_minFP;
};

class StatusIdle:public AINodeCondition
{
public:
	StatusIdle(const tinyxml2::XMLElement* elem){};

	virtual bool Update(Unit* unit);
};

class StatusMove:public AINodeCondition
{
public:
	StatusMove(const tinyxml2::XMLElement* elem){};

	virtual bool Update(Unit* unit);
};

class StatusRotate:public AINodeCondition
{
public:
	StatusRotate(const tinyxml2::XMLElement* elem){};

	virtual bool Update(Unit* unit);
};

class StatusBehit:public AINodeCondition
{
public:
	StatusBehit(const tinyxml2::XMLElement* elem){};

	virtual bool Update(Unit* unit);
};

class StatusDeath:public AINodeCondition
{
public:
	StatusDeath(const tinyxml2::XMLElement* elem){};

	virtual bool Update(Unit* unit);
};

class StatusFreeze:public AINodeCondition
{
public:
	StatusFreeze(const tinyxml2::XMLElement* elem){};

	virtual bool Update(Unit* unit);
};

class StatusSkill:public AINodeCondition
{
public:
	StatusSkill(const tinyxml2::XMLElement* elem){};

	virtual bool Update(Unit* unit);
};

class StatusWoozy:public AINodeCondition
{
public:
	StatusWoozy(const tinyxml2::XMLElement* elem){};

	virtual bool Update(Unit* unit);
};

class IsOppoCastingSkill:public AINodeCondition
{
public:
	IsOppoCastingSkill(const tinyxml2::XMLElement* elem){}

	virtual bool Update(Unit* unit);
};

class IsHurtOppo:public AINodeCondition
{
public:
	IsHurtOppo(const tinyxml2::XMLElement* elem){}

	virtual bool Update(Unit* unit);
};

class IsFixedInCd:public AINodeCondition
{
public:
	IsFixedInCd(const tinyxml2::XMLElement* elem){}

	virtual bool Update(Unit* unit);
};

class IsWander:public AINodeCondition
{
public:
	IsWander(const tinyxml2::XMLElement* elem){}

	virtual bool Update(Unit* unit);
};

class IsCastingSkill:public AINodeCondition
{
public:
	IsCastingSkill(const tinyxml2::XMLElement* elem){}
	
	virtual bool Update(Unit* unit);
};

class IsFighting:public AINodeCondition
{
public:
	IsFighting(const tinyxml2::XMLElement* elem){}

	virtual bool Update(Unit* unit);
};

class IsQTEState:public AINodeCondition
{
public:
	IsQTEState(const tinyxml2::XMLElement* elem);

	virtual bool Update(Unit* unit);

private:
	int QTEState;
};

class TargetQTEState:public AINodeCondition
{
public:
	TargetQTEState(const tinyxml2::XMLElement* elem);

	virtual bool Update(Unit* unit);

private:
	std::string Shared_Target;
	UINT32 TargetIndex;
	int QTEState;
};

class ConditionMonsterNum : public AINodeCondition
{
public:
	ConditionMonsterNum(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	std::string NumString;
	UINT32 NumIndex;
	int MonsterId;
};

class ConditionPlayerNum : public AINodeCondition
{
public:
	ConditionPlayerNum(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	std::string NumString;
	UINT32 NumIndex;
	int BaseProf;
	int DetailProf;
	int Way;
	Vector3 Center;
	float SqrRadius;
};

class IsTargetImmortal : public AINodeCondition
{
public:
	IsTargetImmortal(const tinyxml2::XMLElement* elem);
	virtual bool Update(Unit* unit);

private:
	std::string TargetName;
	UINT32 TargetIndex;
};

#endif