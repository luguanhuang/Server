#ifndef __ACTIVITYHANDLER_H__
#define __ACTIVITYHANDLER_H__

#include "roleeventlistener.h"

class CActivityHandler : public RoleEventListener//> ��Ծ��
{
public:
	static CActivityHandler GlobalHandler;

	void FinishScene(Role* poRole, UINT32 sceneType);

	virtual void OnLevelup(Role *poRole);

	virtual void OnTaskComplete(Role *poRole, int TaskID);

	virtual void OnCompleteScene(Role *poRole, Scene *poScene);

	virtual void OnSceneFailed(Role *poRole, Scene *poScene, bool isEndScene, bool isdraw);
	
	virtual void OnSweepScene(Role *poRole, SceneInfo *pSceneInfo);

	virtual void OnEnhanceEquip(Role* poRole, UINT32 pos); //ǿ��װ��

	virtual void OnComposeJade(Role* poRole, UINT32 jadeLevel, UINT32 count); //��������

	virtual void OnCompose(Role* poRole); //��������

	virtual void OnDecompose(Role* poRole); //�ֽ����

	virtual void OnBuy(Role* poRole, UINT32 dwItemId); //�����ң���������

	virtual void OnShopBuy(Role* poRole); //�̵깺��

	virtual void OnAuction(Role* poRole); //����������

	virtual void OnHitWorldBoss(Role* poRole); //������boss

	virtual void OnHitGuildBoss(Role* poRole); //�򹫻�boss

	//virtual void OnReinforce(Role* poRole);

	virtual void OnTeamStage(Role* poRole, int type); //��Ӹ������糲Ѩ

	virtual void OnGuildActivity(Role* poRole, int type); //0�ǹ���ǩ��, 1��С���˿�

	virtual void OnSendFlower(Role* poRole);

	virtual void OnEnhanceFashion(Role* poRole);

	virtual void OnSkillUp(Role* poRole);

	virtual void OnSmelting(Role* poRole);

	virtual void OnSlotAttr(Role* poRole);

	virtual void OnQA(Role* poRole, UINT32 type); //0��ȫ����⣬1�ǹ������

	virtual void OnBuyAileen(Role* poRole);

	virtual void OnBuyIBShopItem(Role* poRole);

	virtual void OnGuildInherit(Role* poRole);

	virtual void OnBuyPayMember(Role* poRole, int ID);
};

#endif