#ifndef __SCENEDRAGON_H__
#define __SCENEDRAGON_H__

class Enemy;
class Scene;
class Role;
class SceneSwitchData;

struct DragonBuffInfo
{
	DragonBuffInfo()
	{
		buffID = 0;
		buffLevel = 0;
	}
	UINT32 buffID;
	UINT32 buffLevel;
};
class SceneDragon
{
public:
	SceneDragon(Scene* scene);
	~SceneDragon(){}
	static SceneDragon* CreateSceneDragon(Scene* scene);

	void InitBattle(const KKSG::CreateBattleParam& data);
	void Init(const KKSG::SceneSwitchData& data);

	void InitReviveData(std::set<UINT32>& serverids);

	void FinishScene(std::vector<Role*>& roles);
	void ChangeScene();

	bool IsFinalScene(bool isWin);
	bool CanRevive() { return m_canrevive; }

	void AddBuff(Enemy* pEnemy);

	bool CheckSendBattleResult();

	bool CheckTeleportChangeScene();

protected:
	void _InitCrossWeakBuff(const KKSG::CreateBattleParam& data);


protected:
	bool m_canrevive;
	Scene* m_pScene;

	DragonBuffInfo m_buff;
};



#endif // 
