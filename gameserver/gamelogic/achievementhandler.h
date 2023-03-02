#ifndef __ACHIEVEMENTHANDLER_H__
#define __ACHIEVEMENTHANDLER_H__

#include "roleeventlistener.h"

class AChievementHandler : public RoleEventListener
{
public:
	static AChievementHandler GlobalHandler;

	virtual void OnLevelup(Role *poRole);
	virtual void OnSetProfession(Role *poRole);
	virtual void OnCompleteScene(Role *poRole, Scene *poScene);
	virtual void OnSweepScene(Role *poRole, SceneInfo *pSceneInfo);
	virtual void OnChangeAttr(Role* pRole);
	virtual void OnLogin(Role *poRole);
	virtual void OnAddFashion(Role *poRole);

};

#endif // __ACHIEVEMENTHANDLER_H__