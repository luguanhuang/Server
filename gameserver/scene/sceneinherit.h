#ifndef __SCENEINHERIT_H__
#define __SCENEINHERIT_H__

#include "scenecb.h"

class SceneInherit : public ISceneEventListener
{
public:
	SceneInherit() { };
	virtual ~SceneInherit() { };

	virtual void OnEnterSceneFinally(Scene *pScene, Role* role);
	virtual void OnLeaveScene(Scene *pScene, Role* role);
};

#endif