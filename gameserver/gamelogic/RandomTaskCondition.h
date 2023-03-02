#ifndef __RANDOMTASKCONDITION_H__
#define __RANDOMTASKCONDITION_H__

#include "table/RandomTaskTable.h"

class Role;
class Scene;

class IRandomTaskCondition
{
public:
	virtual ~IRandomTaskCondition(){}
	virtual bool IsTaskComplete(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene) = 0;
};

class RandomTaskStarCountGreater : public IRandomTaskCondition
{
public:

	virtual bool IsTaskComplete(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene);
};

class RandomTaskComboCountGreater : public IRandomTaskCondition
{
public:

	virtual bool IsTaskComplete(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene);
};

class RandomTaskBeHitLess : public IRandomTaskCondition
{
public:

	virtual bool IsTaskComplete(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene);
};

class RandomTaskPassTimeLess : public IRandomTaskCondition
{
public:

	virtual bool IsTaskComplete(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene);
};

class RandomTaskTeamDamageGreater : public IRandomTaskCondition
{
public:

	virtual bool IsTaskComplete(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene);
};

class RandomTaskTeamBeHitLess : public IRandomTaskCondition
{
public:

	virtual bool IsTaskComplete(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene);
};

class RandomTaskOpenChestCount : public IRandomTaskCondition
{
public:

	virtual bool IsTaskComplete(RandomTaskTable::RowData *pConf, Role *pRole, Scene *pScene);
};

#endif // __RANDOMTASKCONDITION_H__