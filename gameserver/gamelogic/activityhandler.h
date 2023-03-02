#ifndef __ACTIVITYHANDLER_H__
#define __ACTIVITYHANDLER_H__

#include "roleeventlistener.h"

class CActivityHandler : public RoleEventListener//> 活跃度
{
public:
	static CActivityHandler GlobalHandler;

	void FinishScene(Role* poRole, UINT32 sceneType);

	virtual void OnLevelup(Role *poRole);

	virtual void OnTaskComplete(Role *poRole, int TaskID);

	virtual void OnCompleteScene(Role *poRole, Scene *poScene);

	virtual void OnSceneFailed(Role *poRole, Scene *poScene, bool isEndScene, bool isdraw);
	
	virtual void OnSweepScene(Role *poRole, SceneInfo *pSceneInfo);

	virtual void OnEnhanceEquip(Role* poRole, UINT32 pos); //强化装备

	virtual void OnComposeJade(Role* poRole, UINT32 jadeLevel, UINT32 count); //升级龙玉

	virtual void OnCompose(Role* poRole); //制作道具

	virtual void OnDecompose(Role* poRole); //分解道具

	virtual void OnBuy(Role* poRole, UINT32 dwItemId); //购买金币，购买体力

	virtual void OnShopBuy(Role* poRole); //商店购买

	virtual void OnAuction(Role* poRole); //交易所购买

	virtual void OnHitWorldBoss(Role* poRole); //打世界boss

	virtual void OnHitGuildBoss(Role* poRole); //打公会boss

	//virtual void OnReinforce(Role* poRole);

	virtual void OnTeamStage(Role* poRole, int type); //组队副本，如巢穴

	virtual void OnGuildActivity(Role* poRole, int type); //0是公会签到, 1是小丑扑克

	virtual void OnSendFlower(Role* poRole);

	virtual void OnEnhanceFashion(Role* poRole);

	virtual void OnSkillUp(Role* poRole);

	virtual void OnSmelting(Role* poRole);

	virtual void OnSlotAttr(Role* poRole);

	virtual void OnQA(Role* poRole, UINT32 type); //0是全民答题，1是公会答题

	virtual void OnBuyAileen(Role* poRole);

	virtual void OnBuyIBShopItem(Role* poRole);

	virtual void OnGuildInherit(Role* poRole);

	virtual void OnBuyPayMember(Role* poRole, int ID);
};

#endif