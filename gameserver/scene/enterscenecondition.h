#ifndef __ENTERSCENECONDITION_H__
#define __ENTERSCENECONDITION_H__

class Role;
struct SceneInfo;

///> ���볡��������
class IEnterSceneCondition
{
public:
	virtual int  CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg = NULL) = 0;
	virtual void TakeEnterSceneCost(Role *pRole, SceneInfo *pSceneInfo, bool iswin = false) {}
};

///> ����Ҫ��
class EnterSceneFatigueCondition : public IEnterSceneCondition
{
public:
	virtual int  CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg = NULL);
	virtual void TakeEnterSceneCost(Role *pRole, SceneInfo *pSceneInfo, bool iswin = false);
};

///> �ؿ�����
class EnterSceneCountCondition : public IEnterSceneCondition
{
public:
	virtual int CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg = NULL);
	virtual void TakeEnterSceneCost(Role *pRole, SceneInfo *pSceneInfo, bool iswin = false);
};

///> ʱ����������
class EnterSceneTimeLimitCondition : public IEnterSceneCondition
{
public:
	virtual int CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg = NULL);
	virtual void TakeEnterSceneCost(Role *pRole, SceneInfo *pSceneInfo, bool iswin = false);
};

///> һ�������󣬲��ܽ���ͬID�ؿ���Loading״̬���ܽ��ؿ�
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

// ���ո����������
class EnterHolidaySceneCondition : public IEnterSceneCondition
{
public:
	virtual int CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg = NULL);
};

#endif // __ENTERSCENECONDITION_H__