#ifndef _AIMSGCHANNEL_H_
#define _AIMSGCHANNEL_H_

#include "math/XVector3.h"
#include "entity/XActionDefine.h"

class Unit;
class Scene;

class AISceneMsgChannel
{
public:
	enum AIMsgTarget
	{
		Ally = 0,
		Role,
		Boss,
		Monster,
		Enemy,
		All,
		LevelMgr,
		GlobalAI,
	};

	enum AIMsgType
	{
		ExString = 0,
		Cmd = 1,
	};

public:
	AISceneMsgChannel();
	~AISceneMsgChannel();

public:
	void AddSceneBoss(Unit* boss);
	void RemoveSceneBoss(Unit* boss);
	void SetHostScene(Scene* scene){m_Scene = scene;};
	bool SendMsgTo(AIMsgTarget msgTargetType, AIMsgType msgType, XAIEventArgs& args);
	bool SendMsgTo(Unit* pUnit, AIMsgType msgType, XAIEventArgs& args);
private:
	std::set<UINT64> m_BossId;
	Scene* m_Scene;
};

#endif