#ifndef __AISCENEHANDLER_H__
#define __AISCENEHANDLER_H__

class XAISkillHurtEventArgs;
class XAIComponent;
class XEntity;
class Scene;
class Unit;

class AISceneHandler
{
public:
	typedef int (*GetMonsterIDFunc)(AISceneHandler& handler, int type);
	typedef void (*OnEnterFightFunc)(AISceneHandler& handler, Unit* pTarget);
	typedef void (*OnEnemyDetectedFunc)(AISceneHandler& handler, Unit* pTarget);

	AISceneHandler(XAIComponent* pAIComponent, Scene* pScene);

	int GetMonsterID(int type);
	void OnEnterFight(Unit* pTarget);
	void OnEnemyDetected(Unit* pTarget);

private:
	GetMonsterIDFunc _GetMonsterID;
	OnEnterFightFunc _OnEnterFight;
	OnEnemyDetectedFunc _OnEnemyDetected;

	void _SetDefaultFunc();
	void _SetMobaFunc();
public:

	static AISceneHandler* CreateSceneHandler(XAIComponent* pAIComponent, Scene* pScene);
public:
	inline XAIComponent* GetAIComponent() { return m_pAIComponent; }
	inline XEntity* GetEntity() { return m_pEntity; }
	inline Scene* GetScene() { return m_pScene; }
protected:
	XAIComponent* m_pAIComponent;
	XEntity* m_pEntity;
	Scene* m_pScene;
};

#endif // __RANDOMTASKCONFIG_H__