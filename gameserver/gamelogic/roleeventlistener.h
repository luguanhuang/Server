#ifndef __ROLEEVENTLISTENER_H__
#define __ROLEEVENTLISTENER_H__

class Role;
class Scene;
class Unit;
class Guild;
struct SceneInfo;
struct XItem;
class RoleEventListener
{
public:
	virtual ~RoleEventListener(){}

	virtual void OnLogin(Role *poRole) {}
	virtual void OnLogout(Role *poRole, const char *msg) {}
	virtual void OnLoginReconnect(Role* poRole, Scene* poScene) {};

	///> 切场景前 切换场景时 (离开一个GS，进入另一个GS)
	virtual void OnEnterGs(Role *poRole) {}	
	virtual void OnLeaveGs(Role *poRole) {}
	///> 切场景后
	virtual void OnFirstTimeEnterScene(Role *poRole, Scene *poScene) {}
	virtual void OnHallGsEnterScene(Role *poRole, Scene *poScene) {};	
	virtual void OnEnterScene(Role *poRole, Scene *poScene) {}
	virtual void OnLeaveScene(Role *poRole, Scene *poScene) {}

	virtual void OnCompleteScene(Role *poRole, Scene *poScene) {}
	virtual void OnCompleteSceneWithPartner(Role* pRole, Scene* pScene, std::vector<Role*>& partners, bool isWin) {};
	virtual void OnSceneFailed(Role *poRole, Scene *poScene, bool isEndScene, bool isdraw) {}
	virtual void OnSceneDraw(Role *poRole, Scene* scene) {}
	virtual void OnSweepScene(Role *poRole, SceneInfo *pSceneInfo) {}
	virtual void OnAttack(Role *poRole, Unit *pTargetUnit, double value) {}
	virtual void OnHurt(Role *poRole, Unit* pCastUnit, double value) {}
	virtual void OnDied(Role *poRole) {}
	virtual void OnLevelup(Role *poRole) {}
	virtual void OnTaskComplete(Role *poRole, int TaskID) {}
	virtual void OnSetProfession(Role *poRole) {}
	virtual void OnEnhanceEquip(Role* poRole, UINT32 pos) {}
	virtual void OnChangeJadeOnEquip(Role* poRole, UINT32 pos) {}
	virtual void OnComposeJade(Role* poRole, UINT32 jadeLevel, UINT32 count) {}
	virtual void OnCompose(Role* poRole) {}
	virtual void OnDecompose(Role* poRole) {}
	virtual void OnBuy(Role* poRole, UINT32 dwItemId) {}
	virtual void OnShopBuy(Role* poRole){}
	virtual void OnAuction(Role* poRole){};
	virtual void OnHitWorldBoss(Role* pRole) {}
	virtual void OnHitGuildBoss(Role* pRole) {}
	virtual void OnChangeAttr(Role* pRole) {}
	virtual void OnWearEquip(Role* pRole, UINT32 pos) {}
	virtual void OnForgeEquip(Role* pRole, UINT32 pos) {}
	virtual void OnEnchantEquip(Role* pRole, UINT32 pos) {}
	virtual void OnSmeltEquip(Role* pRole, UINT32 pos) {}
	virtual void OnFuseEquip(Role* pRole, UINT32 pos) {}
	virtual void OnWearFashion(Role* pRole) {}
	virtual void OnDisplayFashion(Role* pRole) {}
	virtual void OnMilitaryChange(Role* pRole) {}
	virtual void OnWearEmblem(Role* pRole, UINT32 pos) {}	
	virtual void OnSmeltEmblem(Role* pRole, UINT32 pos) {}
	virtual void OnWearArtifact(Role* pRole, UINT32 pos) {}
	virtual void OnUpgradeEquip(Role* pRole, UINT32 pos) {}
	virtual void OnChangeVip(Role* poRole) {}
	virtual void OnAddFashion(Role* poRole) {}
	virtual void OnReinforce(Role* poRole) {}
	virtual void OnTeamStage(Role* poRole, int type) {}
	virtual void OnGuildActivity(Role* poRole, int type) {}
	virtual void OnSendFlower(Role* poRole) {}
	virtual void OnEnhanceFashion(Role* poRole) {}
	virtual void OnSkillUp(Role* poRole) {}
	virtual void OnGetItem(Role* poRole, XItem* item) {}
	virtual void OnChangeVirtualItem(Role* poRole, UINT32 item, INT64 count) {}
	virtual void OnSmelting(Role* poRole) {}
	virtual void OnSlotAttr(Role* poRole) {}
	virtual void OnChangeDesignation(Role* poRole, UINT32 designationId) {}
	virtual void OnChangeAchieveV2(Role* poRole, UINT32 achievev2Id) {}
	virtual void OnQA(Role* poRole, UINT32 type){}
	virtual void OnTitleLevelUp(Role* poRole){}
	virtual void OnRecoverFatigue(Role* poRole, int num, int lastRecoverTime, int thisReoverTime){}
	virtual void OnBuyAileen(Role* poRole){}
	virtual void OnPay(Role* poRole, UINT32 totalPay, UINT32 PayNum){}
	virtual void OnFirstPay(Role* poRole){}
	virtual void OnChangeSpriteInFight(Role* poRole, UINT32 pos){}
	virtual void OnPetChange(Role* pRole, UINT64 petUid){}
	virtual void OnGuildDare(Role* pRole){}
	virtual void OnEnhanceMaster(Role* pRole, UINT32 level){}
	virtual void OnEnhanceTrans(Role* pRole, UINT32 pos){};
	virtual void OnLoginOneDay(Role* pRole){}
	virtual void OnGetItemCountAndReason(Role* pRole, UINT32 itemID, UINT32 count, int nReason, int nSubReason){}
	virtual void OnTakeItemCountAndReason(Role* pRole, UINT32 itemID, UINT32 count, int nReason, int nSubReason){}
	virtual void OnSendFriendGift(Role* pRole){}
	virtual void OnBuyIBShopItem(Role* pRole){}
	virtual void OnChangePayMember(Role* pRole, INT32 ID){}
	virtual	void OnBuyPayMember(Role* pRole, INT32 ID){}
	virtual void OnGuildInherit(Role* pRole){}
	virtual void OnThroughDragon(Role* pRole){}
	virtual void OnBuyGrouwthFund(Role* pRole){}
	virtual void OnAddActivityPoint(Role* pRole, UINT32 point){}
	virtual void OnChangePre(Role* pRole){}
	virtual void OnInGuild(Role* pRole){}
};

#endif // __ROLEEVENTLISTENER_H__