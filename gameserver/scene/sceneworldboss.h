#ifndef __SCENEWORLDBOSSH_H__
#define __SCENEWORLDBOSSH_H__

#include "math/XVector3.h"
#include "table/globalconfig.h"
#include "unit/enemy.h"
#include "scene/scenecb.h"

enum EWorldBossSceneState
{
	eBeginPre	= 0,
	eBegin		= 1,
	eGoing		= 2,
	eWaitEnd	= 3,
	eEnd		= 4,

	eDummy,
};

enum EWorldBossState 
{
	eInit = 0,
	eBorning = 1,
	eActive = 2,
	eAIDisable = 3,
	eDead = 4,
};

struct BossInfo
{
	UINT32 dwId;
	UINT32 dwEnemyId;
	float fCurrentHp;

	Vector3 stPos;
	float fRotate;
	float fAttackPercent;
	float fMaxHp;

	BossInfo()
	{
		dwId = 0;
		dwEnemyId = 0;
		stPos.set(float( GetGlobalConfig().WorldBossPoint[0])
				, float( GetGlobalConfig().WorldBossPoint[1])
				, float( GetGlobalConfig().WorldBossPoint[2]));
		fRotate = float( GetGlobalConfig().WorldBossPoint[3]);
		fAttackPercent = 1.0f;
		fCurrentHp = 0.0f;
		fMaxHp = 0.0f;
	}
};

struct WorldBossRoleInfo
{
	UINT32 dwAddAttrCount;
	float nDamage;

	WorldBossRoleInfo()
	{
		dwAddAttrCount = 0;
		nDamage = 0.0f;
	}
};

class Role;
class Unit;
class Enemy;
class Scene;
class BossDamageRankList;
class WorldBossHandler : public EnemyEventListener, public ISceneEventListener, public ITimer
{
public:
	WorldBossHandler(Scene* scene);
	~WorldBossHandler();
public:
	static WorldBossHandler* CreateHandler(Scene* scene);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void Update();

	virtual void OnEnterSceneFinally(Scene* scene, Role* pRole);
	virtual void OnRemoveUnit(Scene *pScene, Unit* unit);

	void OnRoleReconnect(Role* role);

	void InitBoss(UINT32 enemyid, float attackpercent, float currenthp, float maxhp);

	///> 刷新世界boss
	bool CreateWorldBoss();
	void RemoveWorldBoss();

	bool IsBossActive() { return eActive == m_eBossState; }
	bool IsBossDead() { return eDead == m_eBossState; }

	void SubHp(float hp);

	///> boss伤害、死亡回调
	virtual void OnHurt(Enemy* pEnemy, Unit* pCastUnit, double value);
	virtual void OnDied(Enemy* pEnemy);

	///> 增加额外属性
	UINT32 GetAddAttrCount(UINT64 qwRoleId);
	UINT32 PushAddAttrCount(Role* role);

	///> 清空场景中的所有角色
	void KickOutAllRole();

	///> 场景开启和结束
	void OnSceneTimeBegin();
	void OnSceneTimeWaitEnd();
	void OnSceneTimeEnd();

	void OnRank();

	void BroadCast(UINT32 state, UINT32 time);

private:
	void EnableWorldBoss(bool bIsEnable);
	void InvincibleWorldBoss(bool bIsInvincible);

private:
	HTIMER m_timer;
	time_t m_timestamp;

	Scene* m_scene;
	Enemy* m_boss;
	BossInfo m_bossinfo;

	EWorldBossSceneState m_eState;
	EWorldBossState m_eBossState;

	struct WorldBossTimeInfo
	{
		UINT32 time;
		bool isnotify;

		WorldBossTimeInfo()
		{
			time = 0;
			isnotify = false;
		}
	};
	std::map<UINT64, WorldBossTimeInfo> m_roleid2time;
	std::unordered_map<UINT64, WorldBossRoleInfo> m_mapRoleInfo;
};

#endif