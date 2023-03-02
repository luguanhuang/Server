#ifndef __ROBOTCREATESTEP_H__
#define __ROBOTCREATESTEP_H__

#include "robotconfig.h"

class Role;

class IRobotGenStep
{
public:
	virtual ~IRobotGenStep(){}
	virtual void DoWork(RobotConf *pConf, Role *pRobot) = 0;
};


class RobotGenBasicStep : public IRobotGenStep
{
public:
	virtual void DoWork(RobotConf *pConf, Role *pRobot);
};

class RobotGenEquipStep : public IRobotGenStep
{
public:
	virtual void DoWork(RobotConf *pConf, Role *pRobot);
};

class RobotGenSkillStep : public IRobotGenStep
{
public:
	virtual void DoWork(RobotConf *pConf, Role *pRobot);
};

class RobotGenItemStep : public IRobotGenStep
{
public:
	virtual void DoWork(RobotConf *pConf, Role *pRobot);
};

class RobotGenFashionShowStep : public IRobotGenStep
{
public:
	virtual void DoWork(RobotConf *pConf, Role *pRobot);
};

class RobotGenEnhanceStep : public IRobotGenStep
{
public:
	virtual void DoWork(RobotConf *pConf, Role *pRobot);
};

#endif // __ROBOTCREATESTEP_H__