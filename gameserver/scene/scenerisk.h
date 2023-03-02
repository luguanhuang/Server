#ifndef __SCENERISK_H__
#define __SCENERISK_H__

class Enemy;
class Scene;
class Role;
class SceneRisk
{
public:
	SceneRisk(Scene* scene);
	~SceneRisk(){}
	static SceneRisk* CreateSceneRisk(Scene* scene);

	void OnEnterScene(Role* role);
	void SetReEnter(bool reenter){m_reEnter = reenter;}
	bool IsReEnter(){return m_reEnter;}
protected:
	Scene* m_pScene;
	bool m_reEnter;
};



#endif // 
