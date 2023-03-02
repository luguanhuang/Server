#ifndef __SCENESKYWAIT_H__
#define __SCENESKYWAIT_H__

class Scene;
class Role;
class SceneSkyCityWait
{
public:
	SceneSkyCityWait(Scene* scene);
	~SceneSkyCityWait(){}
	static SceneSkyCityWait* CreateSceneSkyWait(Scene* scene);

	void OnEnterScene(Role* role);
	void SetReEnter(bool reenter){m_reEnter = reenter;}
	bool IsReEnter(){return m_reEnter;}

protected:
	Scene* m_pScene;
	bool m_reEnter;
};

#endif // 
