#ifndef __ROLESUMMARYHANDLER_H__
#define __ROLESUMMARYHANDLER_H__

#include "roleeventlistener.h"

class RoleSummaryHandler : public RoleEventListener
{
public:
	static RoleSummaryHandler GlobalHandler;

	virtual void OnLogin(Role* role);
	virtual void OnLogout(Role* role, const char *msg);

	virtual void OnEnterGs(Role* role);
	virtual void OnLeaveGs(Role* role);

	virtual void OnLevelup(Role* role);
	virtual void OnSetProfession(Role* role);

	virtual void OnChangeAttr(Role* pRole);

	virtual void OnWearEquip(Role* pRole, UINT32 pos);
	virtual void OnEnhanceEquip(Role* pRole, UINT32 pos);
	void OnEnhanceAllEquip(Role* pRole, bool isNotify);
	virtual void OnEnhanceTrans(Role* pRole, UINT32 pos);	
	virtual void OnChangeJadeOnEquip(Role* pRole, UINT32 pos);
	virtual void OnForgeEquip(Role* pRole, UINT32 pos);
	virtual void OnEnchantEquip(Role* pRole, UINT32 pos);
	virtual void OnSmeltEquip(Role* pRole, UINT32 pos);
	virtual void OnFuseEquip(Role* pRole, UINT32 pos);
	virtual void OnUpgradeEquip(Role* pRole, UINT32 pos);

	virtual void OnWearFashion(Role* pRole);

	virtual void OnDisplayFashion(Role* pRole);
	virtual void OnMilitaryChange(Role* pRole);
	virtual void OnWearEmblem(Role* role, UINT32 pos);
	virtual void OnSmeltEmblem(Role* pRole, UINT32 pos);

	virtual void OnWearArtifact(Role* pRole, UINT32 pos);

	virtual void OnChangeVip(Role* role);

	virtual void OnTitleLevelUp(Role* role);

	virtual void OnChangeSpriteInFight(Role* poRole, UINT32 pos);
	virtual void OnPetChange(Role* pRole, UINT64 petUid);

	virtual void OnChangePayMember(Role* pRole, INT32 ID);

	virtual void OnChangePre(Role* pRole);
};

#endif