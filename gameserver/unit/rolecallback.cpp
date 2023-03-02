#include "pch.h"
#include "role.h"
#include "scene/scene.h"
#include "gamelogic/roleeventlistener.h"
#include "gamelogic/designationMgr.h"
#include "gamelogic/achievev2Mgr.h"
#include "rolemanager.h"
#include "dummyrolemanager.h"
#include "dummyrole.h"
#include "define/tlogenum.h"

#define CallBack(Fun, ...)   ForeachCall(m_eventListener, Fun, ## __VA_ARGS__) 

void Role::OnHurt(Unit* pCastUnit, double value)
{	
	///> 打出的伤害，找它的召唤者
	if (NULL != pCastUnit)
	{
		Unit* hostunit = pCastUnit->GetFinalHostUnit();
		if (NULL != hostunit)
		{
			pCastUnit = hostunit;
		}
	}
	m_statistics->BeHit();
	m_statistics->BeHurt(pCastUnit, value);
	CallBack(OnHurt, this, pCastUnit, value);

	if (GetCurrScene())
	{
		GetCurrScene()->OnHurt(this, pCastUnit, value);
	}
}

void Role::OnAttack(Unit *pTargetUnit, double value)
{
	CallBack(OnAttack, this, pTargetUnit, value);

	m_statistics->AddComboCount();
	m_statistics->AddAttackInfo(pTargetUnit, value);
}

void Role::OnTreat(Unit *pTargetUnit, double value)
{
	m_statistics->AddTreatInfo(pTargetUnit, value);
}

void Role::OnDied()
{
	m_statistics->AddDeathCount();

	UINT64 killerID = GetKillerID();
	if(killerID)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(killerID);
		if(pRole)
		{
			pRole->Statistics().AddKillCount();
		}
		else
		{
			DummyRole* pDummy = DummyRoleManager::Instance()->Find(killerID);
			if(pDummy)
			{
				pDummy->Statistics().AddKillCount();
			}
		}
	}

	CallBack(OnDied, this);	

	if (m_currScene != NULL)
	{
		m_currScene->OnRoleDead(this);
		if (m_currScene)//防止在OnRoleDead里面改变
		{
			LogInfo("sceneid [%u], role [%llu] is killed by [%llu]", m_currScene->GetSceneID(), GetID(), killerID);
		}
	}
}

void Role::OnWearFashion()
{
	CallBack(OnWearFashion, this);
}

void Role::OnDisplayFashion()
{
	CallBack(OnDisplayFashion, this);
}

void Role::OnMilitaryChange()
{
	CallBack(OnMilitaryChange, this);
}
void Role::OnChangeVip()
{
	CallBack(OnChangeVip, this);
}

void Role::OnAddFashion(UINT32 fashionId)
{
	CallBack(OnAddFashion, this);
	//DesignationMgr::Instance()->OnGetFashion(this, fashionId);
}

void Role::OnReinforce()
{
	CallBack(OnReinforce, this);
}

void Role::OnTeamStage(int type)
{
	CallBack(OnTeamStage, this, type);
}

void Role::OnGuildActivity(int type)
{
	CallBack(OnGuildActivity, this, type);
}

void Role::OnSendFlower()
{
	CallBack(OnSendFlower, this);
}

void Role::OnEnhanceFashion()
{
	CallBack(OnEnhanceFashion, this);
}

void Role::OnSkillUp()
{
	CallBack(OnSkillUp, this);
}

void Role::OnGetItem(XItem* item)
{
	CallBack(OnGetItem, this, item);
}

void Role::OnChangeVirtualItem(UINT32 itemid, INT64 itemcount)
{
	CallBack(OnChangeVirtualItem, this, itemid, itemcount);
}

void Role::OnInGuild()
{
	CallBack(OnInGuild, this);
}

void Role::OnOutGuild()
{
	//CallBack(OnOutGuild, this, guild);
}

void Role::OnSmelting()
{
	CallBack(OnSmelting, this);
}

void Role::OnComposeJade(UINT32 jadeLevel, UINT32 count)
{
	CallBack(OnComposeJade, this, jadeLevel, count);
}

void Role::OnCompose()
{
	CallBack(OnCompose, this);
}

void Role::OnDecompose()
{
	CallBack(OnDecompose, this);
}

void Role::OnBuy(UINT32 dwItemId)
{
	CallBack(OnBuy, this, dwItemId);
}

void Role::OnShopBuy()
{
	CallBack(OnShopBuy, this);
}

void Role::OnAuction()
{
	CallBack(OnAuction, this);
}

void Role::OnHitWorldBoss()
{
	CallBack(OnHitWorldBoss, this);
}

void Role::OnHitGuildBoss()
{
	CallBack(OnHitGuildBoss, this);
}

void Role::OnWearEquip(UINT32 pos)
{
	CallBack(OnWearEquip, this, pos);
}

void Role::OnEnhanceEquip(UINT32 pos)
{
	CallBack(OnEnhanceEquip, this, pos);
}

void Role::OnEnhanceTrans(UINT32 pos)
{
	CallBack(OnEnhanceTrans, this, pos);
}

void Role::OnChangeJadeOnEquip(UINT32 pos)
{
	CallBack(OnChangeJadeOnEquip, this, pos);
}

void Role::OnForgeEquip(UINT32 pos)
{
	CallBack(OnForgeEquip, this, pos);
}

void Role::OnEnchantEquip(UINT32 pos)
{
	CallBack(OnEnchantEquip, this, pos);
}

void Role::OnSmeltEquip(UINT32 pos)
{
	CallBack(OnSmeltEquip, this, pos);
}

void Role::OnFuseEquip(UINT32 pos)
{
	CallBack(OnFuseEquip, this, pos);
}

void Role::OnUpgradeEquip(UINT32 pos)
{
	CallBack(OnUpgradeEquip, this, pos);
}

void Role::OnWearEmblem(UINT32 pos)
{
	CallBack(OnWearEmblem, this, pos);
}

void Role::OnSmeltEmblem(UINT32 pos)
{
	CallBack(OnSmeltEmblem, this, pos);
}

void Role::OnWearArtifact(UINT32 pos)
{
	CallBack(OnWearArtifact, this, pos);
}

void Role::OnSceneComplete(Scene* scene)
{
	CallBack(OnCompleteScene, this, scene);
}

void Role::OnCompleteSceneWithPartner(Scene* pScene, std::vector<Role*>& partners, bool isWin)
{
	CallBack(OnCompleteSceneWithPartner, this, pScene, partners, isWin);
}

void Role::OnSweepScene(SceneInfo *pSceneInfo)
{
	CallBack(OnSweepScene, this, pSceneInfo);
}

void Role::OnTaskComplete(int TaskID)
{
	CallBack(OnTaskComplete, this, TaskID);
}

void Role::OnSceneFailed(Scene* scene, bool isEndScene, bool isdraw)
{
	CallBack(OnSceneFailed, this, scene, isEndScene, isdraw);
}

void Role::OnSceneDraw(Scene* scene)
{
	CallBack(OnSceneDraw, this, scene);
}

void Role::OnSlotAttr()
{
	CallBack(OnSlotAttr, this);
}

void Role::OnChangeDesignation(UINT32 designationId)
{
	CallBack(OnChangeDesignation, this, designationId);
}

void Role::OnChangeAchieveV2(UINT32 achievev2Id)
{
	CallBack(OnChangeAchieveV2, this, achievev2Id);
}

void Role::OnPay(UINT32 totalPay, UINT32 PayNum)
{
	CallBack(OnPay, this, totalPay, PayNum);
}

void Role::OnFirstPay()
{
	CallBack(OnFirstPay, this);
}

void Role::OnQA(UINT32 type)
{
	CallBack(OnQA, this, type);
}

void Role::OnTitleLevelUp()
{
	CallBack(OnTitleLevelUp, this);
}

void Role::OnRecoverFatigue(int num, int lastRecoverTime, int thisReoverTime)
{
	CallBack(OnRecoverFatigue, this, num, lastRecoverTime, thisReoverTime);
}

void Role::OnBuyAileen()
{
	CallBack(OnBuyAileen, this);
}

void Role::OnChangeSpriteInFight(UINT32 pos)
{
	CallBack(OnChangeSpriteInFight, this, pos);
}

void Role::OnPetChange(UINT64 petUid)
{
	CallBack(OnPetChange, this, petUid);
}

void Role::OnGuildDare()
{
	CallBack(OnGuildDare, this);
}

void Role::OnEnhanceMaster(UINT32 level)
{
	CallBack(OnEnhanceMaster, this, level);
}

void Role::OnLoginOneDay()
{
	CallBack(OnLoginOneDay, this);
}

void Role::OnGetItemCountAndReason(UINT32 itemID, UINT32 count, int nReason, int nSubReason)
{
	CallBack(OnGetItemCountAndReason, this, itemID, count, nReason, nSubReason);
}

void Role::OnTakeItemCountAndReason(UINT32 itemID, UINT32 count, int nReason, int nSubReason)
{
	CallBack(OnTakeItemCountAndReason, this, itemID, count, nReason, nSubReason);
}

void Role::OnSendFriendGift()
{
	CallBack(OnSendFriendGift, this);
}

void Role::OnBuyIBShopItem()
{
	CallBack(OnBuyIBShopItem, this);
}

void Role::OnChangePayMember(INT32 ID)
{
	CallBack(OnChangePayMember, this, ID);
}

void Role::OnBuyPayMember(INT32 ID)
{
	CallBack(OnBuyPayMember, this, ID);
}

void Role::OnBuyGrouwthFund()
{
	CallBack(OnBuyGrouwthFund, this);
}

void Role::OnGuildInherit()
{
	CallBack(OnGuildInherit, this);
}

void Role::OnThroughDragon()
{
	CallBack(OnThroughDragon, this);
}

void Role::OnAddActivityPoint(UINT32 point)
{
	CallBack(OnAddActivityPoint, this, point);
}
void Role::OnChangePre()
{
	CallBack(OnChangePre, this);
}