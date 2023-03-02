#ifndef __SCENEGUILDCAMP_H__
#define __SCENEGUILDCAMP_H__

#include "scene/scenecb.h"

class Enemy;
class Scene;
class Role;


class SceneGuildCamp : public ISceneEventListener
{
public:
	SceneGuildCamp(Scene* scene);
	~SceneGuildCamp(){}
	static SceneGuildCamp* CreateSceneGuildCamp(Scene* scene);

	virtual void OnEnemyDie(Scene* pScene, Enemy* enemy);

	virtual void OnBeginFinish(Scene* pScene);

	virtual void OnEnterSceneFinally(Scene *pScene, Role* role);

	void notifyAll();

	void endScene(bool win);
	void onEnterScene(Role* role);

	/*int getRank(Role* role) const;
	void QueryRankInfo(UINT64 roleID);
	void SetRoleRank(Role* role, int rank);*/

protected:
	UINT64 m_guildID;
	Scene* m_pScene;
};



#endif // 
