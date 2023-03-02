#ifndef __SPACTIVITYHANDLER_H__
#define __SPACTIVITYHANDLER_H__

#include "gamelogic/roleeventlistener.h"

class SpActivityHandler : public RoleEventListener
{
public:
	static SpActivityHandler GlobalHandler;	

	void CompleteTaskAfterStartSpActivity(Role* pRole, UINT32 actid);	

	void UpdateTaskWhenEnterHallGS(Role* pRole);

	void CompleteGetItemTask(Role* pRole, UINT32 itemID, UINT32 count, int nReason, int nSubReason, UINT32 actid);
	void CompleteLevelUpTask(Role* pRole, UINT32 actid);	

	virtual void OnHallGsEnterScene(Role* pRole, Scene* pScene);

	virtual void OnCompleteScene(Role* pRole, Scene* pScene);

	virtual void OnCompleteSceneWithPartner(Role* pRole, Scene* pScene, std::vector<Role*>& partners, bool isWin);

	virtual void OnSceneFailed(Role* pRole, Scene *poScene, bool isEndScene, bool isdraw);

	virtual void OnSweepScene(Role *pRole, SceneInfo *pSceneInfo);

	virtual void OnHitWorldBoss(Role* pRole); //打世界boss

	virtual void OnHitGuildBoss(Role* pRole); //打公会boss

	virtual void OnLoginOneDay(Role* pRole);

	virtual void OnGuildDare(Role* pRole); //公会挑战

	virtual void OnQA(Role* pRole, UINT32 type); //0是全民答题，1是公会答题

	virtual void OnEnhanceMaster(Role* pRole, UINT32 level);

	virtual void OnGetItemCountAndReason(Role* pRole, UINT32 itemID, UINT32 count, int nReason, int nSubReason);

	virtual void OnTakeItemCountAndReason(Role* pRole, UINT32 itemID, UINT32 count, int nReason, int nSubReason);

	virtual void OnComposeJade(Role* pRole, UINT32 jadeLevel, UINT32 count);

	virtual void OnSendFriendGift(Role* pRole);

	virtual void OnLevelup(Role* pRole);

	virtual void OnAddActivityPoint(Role* pRole, UINT32 point);

	virtual void OnInGuild(Role* pRole);

	void OnFriendChange(Role* pRole);

	void OnGuildDonate(Role* pRole, UINT32 count);

	void OnSubTaskComplete(Role* pRole, UINT32 type);
};

#endif