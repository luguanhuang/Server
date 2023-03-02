#ifndef __SCENEHORSE_H__
#define __SCENEHORSE_H__

class Scene;
class Role;
class SceneHorse
{
public:
	SceneHorse(Scene* scene);
	~SceneHorse(){}
	static SceneHorse* CreateSceneHorse(Scene* scene);

	void OnEnterScene(Role* role);
	void SetReEnter(bool reenter){m_reEnter = reenter;}
	bool IsReEnter(){return m_reEnter;}

protected:
	Scene* m_pScene;
	bool m_reEnter;
};

#endif // 
