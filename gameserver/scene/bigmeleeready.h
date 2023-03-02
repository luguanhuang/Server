#ifndef __SCENEBMREADY_H__
#define __SCENEBMREADY_H__
#include "timer.h"

class Scene;
class Role;
class SceneBMReady : public ITimer
{
public:
	SceneBMReady(Scene* scene);
	~SceneBMReady();
	static SceneBMReady* CreateSceneBMReady(Scene* scene);
	void SyncTime();
	void OnEnterScene(Role* role);
	void OnTimer(UINT32 dwID, UINT32 dwCount);
protected:
	Scene* m_pScene;
private:
	HTIMER m_timerId;

};

#endif
