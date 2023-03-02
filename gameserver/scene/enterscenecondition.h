#ifndef __ENTERSCENECONDITION_H__
#define __ENTERSCENECONDITION_H__

class Role;
struct SceneInfo;

///> 进入场景的条件
class IEnterSceneCondition
{
public:
	virtual int  CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg = NULL) = 0;
	virtual void TakeEnterSceneCost(Role *pRole, SceneInfo *pSceneInfo, bool iswin = false) {}
};

///> 体力要求
class EnterSceneFatigueCondition : public IEnterSceneCondition
{
public:
	virtual int  CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg = NULL);
	virtual void TakeEnterSceneCost(Role *pRole, SceneInfo *pSceneInfo, bool iswin = false);
};

///> 关卡次数
class EnterSceneCountCondition : public IEnterSceneCondition
{
public:
	virtual int CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg = NULL);
	virtual void TakeEnterSceneCost(Role *pRole, SceneInfo *pSceneInfo, bool iswin = false);
};

///> 时间限制条件
class EnterSceneTimeLimitCondition : public IEnterSceneCondition
{
public:
	virtual int CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg = NULL);
	virtual void TakeEnterSceneCost(Role *pRole, SceneInfo *pSceneInfo, bool iswin = false);
};

///> 一般性需求，不能进相同ID关卡，Loading状态不能进关卡
class EnterSceneNormalCondition : public IEnterSceneCondition
{
public:
	virtual int CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg = NULL);
	virtual void TakeEnterSceneCost(Role *pRole, SceneInfo *pSceneInfo, bool iswin = false);
};

class EnterGuildSceneCondition : public IEnterSceneCondition
{
public:
	virtual int CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg = NULL);
};

class EnterTowerSceneCondition : public IEnterSceneCondition
{
public:
	virtual int CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg = NULL);
};

// 节日副本条件检查
class EnterHolidaySceneCondition : public IEnterSceneCondition
{
public:
	virtual int CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg = NULL);
};

#endif // __ENTERSCENECONDITION_H__