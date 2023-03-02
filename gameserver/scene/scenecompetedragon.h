#ifndef __SCENECOMPETEDRAGON_H__
#define __SCENECOMPETEDRAGON_H__

class Enemy;
class Scene;
class Role;
class SceneSwitchData;

class SceneCompeteDragon
{
public:
	SceneCompeteDragon(Scene* scene);
	~SceneCompeteDragon(){}
	static SceneCompeteDragon* CreateSceneCompeteDragon(Scene* scene);

	void FinishScene(std::vector<Role*>& roles);
	void ChangeScene();

	bool CheckSendBattleResult();

protected:
	Scene* m_pScene;
};

#endif // 
