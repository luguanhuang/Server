#ifndef __DRAGONEXPBASE_H__
#define __DRAGONEXPBASE_H__ 

#include "pb/enum.pb.h"

namespace KKSG
{
	class DEStageProgress;
	class DragonExpResult;
}

const int DEDefaultHpPercent = 100;

struct DEBossHpPercent
{
	DEBossHpPercent()
	{
		bossID = 0;
		bossHpPercent = 100;
	}
	UINT32 bossID;
	int bossHpPercent;
};

class CDEStageProgress
{
public:
	CDEStageProgress(UINT32 sceneid)
	{
		m_sceneID = sceneid;
		//m_state = KKSG::DEPS_NOTOPEN;
	}

	UINT32 GetSceneID(){ return m_sceneID; }

	void SetBossHpPercent(UINT32 bossid, int percent);
	int GetBossHpPercent(UINT32 bossid);
	int GetAVGBossHpPercent();
	
	//void SetProgressState(KKSG::DEProgressState st) { m_state = st; }
	//KKSG::DEProgressState GetProgressState(){ return m_state; }

	void Save(KKSG::DEStageProgress& sp);
	void Load(const KKSG::DEStageProgress& sp);

private:
	UINT32 m_sceneID;
	//KKSG::DEProgressState m_state;
	std::unordered_map<UINT32, DEBossHpPercent> m_bossHpPercent;
};

class Scene;
class Role;
class Enemy;

class CDESceneHandler
{
public:
	CDESceneHandler(Scene* pScene);
	~CDESceneHandler();

	void OnRoleEnter(Role* pRole);
	//include leave after dead
	void OnRoleLeave(Role* pRole);
	void OnBossApper(Enemy* pBoss);
	void OnBossDie(Enemy* pBoss);

	void OnSceneUpdate(int sceneState);

	void SaveLeftBossHpPercent(Role* pRole);

	void FillResult(KKSG::DragonExpResult* pDEResult, Role* pRole, bool iswin);

private:
	bool m_errFlag;//½ø´í³¡¾° ·ÀË¢
	int m_oldBossHpPercent;
	bool m_saveFlag;
	bool m_rewardFlag;
	Scene* m_scene;
};
#endif