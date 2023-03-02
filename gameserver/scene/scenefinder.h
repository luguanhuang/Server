#ifndef __SCENEFINDER_H__
#define __SCENEFINDER_H__

#include "math/XVector3.h"

class Unit;
class Role;

class IFindUnitCondition
{
public:
	virtual ~IFindUnitCondition() {}
	virtual bool operator()(Unit *pUnit) = 0;
};
///> 逻辑条件
class TrueCondition : public IFindUnitCondition
{
public:
	virtual bool operator()(Unit *pUnit) { return true; }
};
///> &&
class LogicAndCondition : public IFindUnitCondition
{
public:
	LogicAndCondition(IFindUnitCondition *cond1, IFindUnitCondition *cond2);
	virtual bool operator()(Unit *pUnit);

private:
	IFindUnitCondition *m_Cond[2];
};
///> ||
class LogicOrCondition : public IFindUnitCondition
{
public:
	LogicOrCondition(IFindUnitCondition *cond1, IFindUnitCondition *cond2);
	virtual bool operator()(Unit *pUnit);

private:
	IFindUnitCondition *m_Cond[2];
};
///> ~
class LogicNotCondition : public IFindUnitCondition
{
public:
	LogicNotCondition(IFindUnitCondition *cond);
	virtual bool operator()(Unit *pUnit);

private:
	IFindUnitCondition *m_Cond;
};

///> same fight group
class FindAllyCondition : public IFindUnitCondition
{
public:
	FindAllyCondition(Unit *pUnit):m_pUnit(pUnit){}
	virtual bool operator()(Unit *pUnit);

	Unit *m_pUnit;
};
/////> in group
//class FindInGroupCondition : public IFindUnitCondition 
//{
//public:
//	FindInGroupCondition(std::set<UINT32>& fightgroups){ m_fightgroups = fightgroups; }
//	virtual bool operator()(Unit *pUnit);
//private:
//	std::set<UINT32> m_fightgroups;
//};
/////> in group in range
//class FindInGroupInRangeCondition : public IFindUnitCondition 
//{
//public:
//	FindInGroupInRangeCondition(const Vector3 &position, float raidus, std::set<UINT32>& fightgroups)
//	{ 
//		m_raidus = raidus;
//		m_position = position;
//		m_fightgroups = fightgroups;
//	}
//	virtual bool operator()(Unit *pUnit);
//private:
//	float m_raidus;
//	Vector3 m_position;
//	std::set<UINT32> m_fightgroups;
//};
///> dummyrole same group
class FindDummyRoleFightGroupCondition : public IFindUnitCondition 
{
public:
	FindDummyRoleFightGroupCondition(UINT32 fightGroup):m_fightGroup(fightGroup){}
	virtual bool operator()(Unit *pUnit);
private:
	UINT32 m_fightGroup;
};

///> 怪物条件
///> is enemy
class FindEnemyCondition : public IFindUnitCondition
{
public:
	virtual bool operator()(Unit *pUnit);
};
///> enemy id
class FindEnemyIDCondition : public IFindUnitCondition
{
public:
	FindEnemyIDCondition(UINT32 enemyID):m_enemyID(enemyID){}
	virtual bool operator()(Unit *pUnit);
private:
	UINT32 m_enemyID;
};
///> enemy wave id
class FindEnemyWaveIDCondition : public IFindUnitCondition
{
public:
	FindEnemyWaveIDCondition(int waveID):m_waveID(waveID){}
	virtual bool operator()(Unit *pUnit);
private:
	int m_waveID;
};
///> enemy id not died
class FindEnemyTemplateIdCondition : public IFindUnitCondition
{
public:
	FindEnemyTemplateIdCondition(UINT32 templateId):m_templateID(templateId){}
	virtual bool operator()(Unit *pUnit);

private:
	UINT32 m_templateID;
};
///> is boss
class FindBossCondition : public IFindUnitCondition
{
public:
	virtual bool operator()(Unit* pUnit);
};

/////> 生物属性条件
/////> collide 
//class FindCollideUnitCondition : public IFindUnitCondition
//{
//public:
//	FindCollideUnitCondition() {}
//	virtual bool operator()(Unit *pUnit);
//};

#endif // __SCENEFINDER_H__