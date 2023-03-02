#ifndef __SCENETRANSCTRL_H__
#define __SCENETRANSCTRL_H__

#include "scenetransconfig.h"
#include "buff/XBuffTemplateManager.h"

class Role;
class Scene;
class SceneTransCtrl
{
public:
	SceneTransCtrl(Scene* scene, TransConfig* config);

	void OnEnter(Role* role);
	bool OnLeave(Role* role);

	void OnTick(bool isinit);

private:
	bool IsInTime(UINT32 offset = 0);
	BuffDesc RandomBuff();

private:
	bool m_isin;
	Scene* m_scene;
	UINT32 m_timestamp;
	BuffDesc m_buff;
	TransConfig* m_config;
};

#endif