#ifndef __ROLELEVELHANDLER_H__
#define __ROLELEVELHANDLER_H__

#include "roleeventlistener.h"

class RoleCommonHandler : public RoleEventListener
{
public:
	static RoleCommonHandler GlobalHandler;

	virtual void OnChangeAttr(Role* pRole);
	virtual void OnFirstTimeEnterScene(Role *poRole, Scene *poScene);
	virtual void OnLoginReconnect(Role* poRole, Scene* poScene);
	virtual void OnHallGsEnterScene(Role *poRole, Scene *poScene);
	virtual void OnLevelup(Role *poRole);	
	virtual void OnTaskComplete(Role *poRole, int TaskID);
	virtual void OnEnterScene(Role *poRole, Scene *poScene);
	virtual void OnLeaveScene(Role *poRole, Scene *poScene);
	virtual void OnCompleteScene(Role *poRole, Scene *poScene);
	virtual void OnSceneFailed(Role *poRole, Scene *poScene, bool isEndScene, bool isdraw);
	virtual void OnSceneDraw(Role *poRole, Scene* scene);
	virtual void OnSweepScene(Role *poRole, SceneInfo *pSceneInfo);
	virtual void OnLogin(Role *poRole);
	virtual void OnLogout(Role *poRole, const char *msg);
	virtual void OnEnterGs(Role *poRole); 
	virtual void OnLeaveGs(Role *poRole);
	virtual void OnAttack(Role *poRole, Unit *pTargetUnit, double value);
	virtual void OnHurt(Role *poRole, Unit* pCastUnit, double value);
	virtual void OnGetItem(Role *poRole, XItem* item);
	virtual void OnChangeVirtualItem(Role* poRole, UINT32 item, INT64 count);
	virtual void OnBuy(Role* poRole, UINT32 dwItemId);
	virtual void OnRecoverFatigue(Role* poRole, int num, int lastRecoverTime, int thisReoverTime);
	virtual void OnChangeVip(Role* poRole);
	virtual void OnTitleLevelUp(Role* poRole);
	virtual void OnPay(Role* poRole, UINT32 totalPay, UINT32 PayNum);
	virtual void OnFirstPay(Role* poRole);
	virtual void OnQA(Role* poRole, UINT32 type);
	virtual	void OnBuyPayMember(Role* poRole, INT32 ID);
	virtual void OnChangeJadeOnEquip(Role* poRole, UINT32 pos);
	virtual void OnWearEquip(Role* pRole, UINT32 pos);
	virtual void OnWearEmblem(Role* pRole, UINT32 pos);
	virtual void OnThroughDragon(Role* pRole);
	virtual void OnBuyGrouwthFund(Role* pRole);
	virtual void OnBuyAileen(Role* poRole);
};

#endif // __ROLELEVELHANDLER_H__