#ifndef __SCENEBATTLEREADY_H__
#define __SCENEBATTLEREADY_H__
class Scene;



class SceneBFReady
{
public:
	SceneBFReady(Scene* scene);
	~SceneBFReady();
	static SceneBFReady* CreateSceneBFReady(Scene* scene);
protected:
	Scene* m_pScene;
private:


};


#endif
