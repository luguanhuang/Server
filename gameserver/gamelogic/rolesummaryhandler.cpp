#include "pch.h"
#include "time.h"
#include "rolesummaryhandler.h"
#include "rolesummarymgr.h"
#include "unit/roleoutlook.h"
#include "config.h"
#include "unit/role.h"

RoleSummaryHandler RoleSummaryHandler::GlobalHandler;

void RoleSummaryHandler::OnLogin(Role *poRole)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(poRole, SUMMARY_ALL);
	RoleSummaryMgr::Instance()->MarkToDelete(poRole->GetID(), false);
}

void RoleSummaryHandler::OnLogout(Role *poRole, const char *msg)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(poRole, SUMMARY_LASTLOGOUT);
	RoleSummaryMgr::Instance()->MarkToDelete(poRole->GetID(), true);
}

void RoleSummaryHandler::OnEnterGs(Role* poRole)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(poRole, SUMMARY_ALL);
	RoleSummaryMgr::Instance()->MarkToDelete(poRole->GetID(), false);
}

void RoleSummaryHandler::OnLeaveGs(Role* poRole)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(poRole, SUMMARY_LASTLOGOUT);
	RoleSummaryMgr::Instance()->MarkToDelete(poRole->GetID(), true);
}

void RoleSummaryHandler::OnLevelup(Role *poRole)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(poRole, SUMMARY_LEVEL);
}

void RoleSummaryHandler::OnSetProfession(Role *poRole)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(poRole, SUMMARY_PROFESSION);
}

void RoleSummaryHandler::OnChangeAttr(Role* pRole)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_ATTR);
}

void RoleSummaryHandler::OnWearEquip(Role* pRole, UINT32 pos)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_EQUIP, (void*)&pos);

	RoleOutLook outlook(pRole);
	outlook.Fill(SUMMARY_EQUIP, (void*)&pos);
	outlook.BroadCast();
}

void RoleSummaryHandler::OnEnhanceEquip(Role* pRole, UINT32 pos)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_EQUIP, (void*)&pos);

	RoleOutLook outlook(pRole);
	outlook.Fill(SUMMARY_EQUIP, (void*)&pos);
	outlook.BroadCast();
}

void RoleSummaryHandler::OnEnhanceAllEquip(Role* pRole, bool isNotify)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_EQUIP, NULL);

	if (isNotify)
	{
		RoleOutLook outlook(pRole);
		outlook.Fill(SUMMARY_EQUIP, NULL);
		outlook.BroadCast();
	}
}

void RoleSummaryHandler::OnEnhanceTrans(Role* pRole, UINT32 pos)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_EQUIP, (void*)&pos);

	RoleOutLook outlook(pRole);
	outlook.Fill(SUMMARY_EQUIP, (void*)&pos);
	outlook.BroadCast();
}

void RoleSummaryHandler::OnChangeJadeOnEquip(Role* pRole, UINT32 pos)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_EQUIP, (void*)&pos);
}

void RoleSummaryHandler::OnForgeEquip(Role* pRole, UINT32 pos)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_EQUIP, (void*)&pos);
}

void RoleSummaryHandler::OnEnchantEquip(Role* pRole, UINT32 pos)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_EQUIP, (void*)&pos);
}

void RoleSummaryHandler::OnSmeltEquip(Role* pRole, UINT32 pos)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_EQUIP, (void*)&pos);
}

void RoleSummaryHandler::OnFuseEquip(Role* pRole, UINT32 pos)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_EQUIP, (void*)&pos);
}

void RoleSummaryHandler::OnUpgradeEquip(Role* pRole, UINT32 pos)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_EQUIP, (void*)&pos);
}

void RoleSummaryHandler::OnWearFashion(Role* pRole)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_FASHION);
}

void RoleSummaryHandler::OnDisplayFashion(Role* pRole)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_DISPLAYFASHION);
}

void RoleSummaryHandler::OnMilitaryChange(Role* pRole)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_MILITARY_RANK);
}

void RoleSummaryHandler::OnWearEmblem(Role* pRole, UINT32 pos)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_EMBLEM, (void*)&pos);
}

void RoleSummaryHandler::OnSmeltEmblem(Role* pRole, UINT32 pos)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_EMBLEM, (void*)&pos);
}

void RoleSummaryHandler::OnWearArtifact(Role* pRole, UINT32 pos)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_ARTIFACT, (void*)&pos);
}

void RoleSummaryHandler::OnChangeVip(Role* pRole)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_VIPLEVEL);
}

void RoleSummaryHandler::OnTitleLevelUp(Role* pRole)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_TITLE);

	RoleOutLook outlook(pRole);
	outlook.Fill(SUMMARY_TITLE, NULL);
	outlook.BroadCast();
}

void RoleSummaryHandler::OnChangeSpriteInFight(Role* pRole, UINT32 pos)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_SPRITE, (void*)&pos);
}

void RoleSummaryHandler::OnPetChange(Role* pRole, UINT64 petUid)
{
	UINT64 uid = petUid; 
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_PETS, (void*)&uid);
}

void RoleSummaryHandler::OnChangePayMember(Role* pRole, INT32 ID)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_PAYMEMBER, (void*)&ID);
}

void RoleSummaryHandler::OnChangePre(Role* pRole)
{
	RoleSummaryMgr::Instance()->UpdateRoleSummary(pRole, SUMMARY_PAYSCOREPRE);
}