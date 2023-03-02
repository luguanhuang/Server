#ifndef __SCENERESWARPVE_H__
#define __SCENERESWARPVE_H__

class Enemy;
class Scene;
class Role;

class SceneResWarPVE
{
public:
	SceneResWarPVE(Scene* pScene);
	~SceneResWarPVE(){}
	static SceneResWarPVE* CreateSceneResWarPVE(Scene* pScene);
	void Spawn();
	void OnBossAppear(Enemy* pBoss);
	void Update();
	
	void OnEnterScene(Role* role);
	void AddBuff(Role* pRole, UINT32 nBuffID, UINT32 nBuffLevel);
	void SetMineID(UINT32 nMineID)
	{
		m_nMineID = nMineID;
	}
	UINT32 GetMineID()
	{
		return m_nMineID;
	}
	void SetGuildID(UINT64 nGuildID)
	{
		m_nGuildID = nGuildID;
	}
	UINT64 GetGuildID()
	{
		return m_nGuildID;
	}
	void SetTeamID(UINT32 nTeamID)
	{
		m_nTeamID = nTeamID;
	}
	UINT32 GetTeamID()
	{
		return m_nTeamID;
	}
	UINT32 GetGuildBuffTime()
	{
		return m_nBuffTime;
	}
	void SetGuildBuffTime(UINT32 nTime)
	{
		m_nBuffTime = nTime;
	}
	void SetRandBuffID(UINT32 nBuffID)
	{
		m_nRandBuffID = nBuffID;
	}
	UINT32 GetRandBuffID()
	{
		return m_nRandBuffID;
	}
protected:
	time_t m_nTime;
	UINT32 m_nBuffTime;
	Scene* m_pScene;
	UINT32 m_nMineID;
	UINT64 m_nGuildID;
	UINT32 m_nTeamID;
	time_t m_addBuffTime;
	UINT32 m_buffID;
	UINT32 m_buffLevel;
	UINT32 m_bossID;
	UINT32 m_nRandBuffID;
};

#endif // 
