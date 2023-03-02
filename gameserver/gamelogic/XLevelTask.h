#ifndef __XLEVEL_TASK_H__
#define __XLEVEL_TASK_H__

#include "math/XVector3.h"

class XLevelSpawner;
class DummyRole;
class Role;

class XLevelBaseTask
{
public:
	 int m_Id;
	 XLevelBaseTask(XLevelSpawner* pSpawner):m_Id(0), m_pSpawner(pSpawner) {}
	 virtual void Execute(float time) {}

protected:
	XLevelSpawner* m_pSpawner;
};

class XLevelSpawnTask : public XLevelBaseTask
{
public:

	XLevelSpawnTask(XLevelSpawner* pSpawner);
	virtual void Execute(float time);

public:
	int m_EnemyID;
	int m_MonsterRotate;
	int m_MonsterIndex;
	Vector3 m_MonsterPos;
	float m_scale;
};

class Scene;
class XLevelScriptTask : public XLevelBaseTask
{
public:
	XLevelScriptTask(XLevelSpawner* pSpawner);
	virtual void Execute(float time);

public:
	std::string m_strScriptName;
	Scene* m_scene;
};

class XLevelRoleScriptTask : public XLevelBaseTask
{
public:
	XLevelRoleScriptTask(XLevelSpawner* pSpawner);
	virtual void Execute(float time);

public:
	std::string m_strScriptName;
	Scene* m_scene;
	bool m_isReconnect;
	//使用role指针，则push进去后，必须同一帧执行完
	Role* m_role;
};

#endif