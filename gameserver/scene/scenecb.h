#ifndef __SCENECB_H__
#define __SCENECB_H__

class Scene;
class Role;
class DummyRole;
class Enemy;
class Unit;

class ISceneListener
{
public:
	virtual ~ISceneListener() {}

	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win) { };
};

class ISceneEventListener
{
public:
	virtual ~ISceneEventListener() {}

	virtual void OnPreEnterScene(Scene *pScene, Role* role) { };
	virtual void OnEnterSceneFinally(Scene *pScene, Role* role) { };
	virtual void OnPreEnterSceneFinally(Scene *pScene, Role* role) { };

	virtual void OnAddNonRole(Scene *pScene, Unit* unit) { };
	virtual void OnRemoveUnit(Scene *pScene, Unit* unit) { };
	virtual void OnRemoveWaitingRole(Scene* pScene, Role* role) {};

	virtual void OnLeaveScene(Scene *pScene, Role* role) { };

	virtual void OnRoleDie(Scene* pScene, Role* role) { };
	virtual void OnDummyDie(Scene* pScene, DummyRole* dummy) { };
	virtual void OnEnemyDie(Scene* pScene, Enemy* enemy) { };
	virtual void OnRoleReconnect(Role* role) { };

	virtual void OnBeginFinish(Scene* pScene) { };
};

#endif // __SCENECB_H__