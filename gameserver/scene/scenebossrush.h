#ifndef __SCENEBOSSRUSH_H__
#define __SCENEBOSSRUSH_H__

#include "scenecb.h"

class Enemy;
class Scene;
class Role;

class CSceneBossRush : public ISceneEventListener
{
public:
	CSceneBossRush(Scene* scene);
	~CSceneBossRush();

	virtual void OnLeaveScene(Scene *pScene, Role* role);
	virtual void OnRoleDie(Scene* pScene, Role* role);
	virtual void OnEnemyDie(Scene* pScene, Enemy* enemy);

	void OnBossAppear(Enemy* pBoss);
	void MakeCountinue();

	bool IsWin();

	void Update();

	void TestDead(bool rolefirst);

private:
	void OnBossDie(Enemy* pBoss);
	void ResetAll();
	void StopAIHurt();

	void SetLastBoss(Enemy* pBoss);
	void ClearLastBoss();

	void ClearBuffInfo();
	void CheckAddBuff();

	void AddBuff(Role* pRole, UINT32 buffid, UINT32 bufflevel);

	inline bool Stoped() { return m_stopFlag; }
	void SetStopFlag(bool flag);

	UINT32 m_buffID;
	UINT32 m_buffLevel;
	time_t m_addBuffTime;

	time_t m_lastBossAppearT;
	UINT64 m_bossID;

	bool m_stopFlag;

	Scene* m_scene;
};

#endif