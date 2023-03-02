#include "pch.h"
#include "role.h"
#include "calcattribute.h"
#include "combatattribute.h"
#include "table/PlayerLevelManager.h"
#include "pb/project.pb.h"
#include "entity/XRole.h"
#include "component/XBuffComponent.h"
#include "gamelogic/bag.h"
#include "table/ProfessionMgr.h"
#include "table/globalconfig.h"
#include "table/PowerPointMgr.h"
#include "gamelogic/rolefashion.h"
#include "gamelogic/skilllevel.h"
#include "gamelogic/designationMgr.h"
#include "unit/pet.h"
#include "gamelogic/titlerecord.h"
#include "gamelogic/atlas.h"
#include "gamelogic/spriterecord.h"
#include "guild/guildrecord.h"
#include "scene/scene.h"
#include "scene/sceneconfig.h"
#include "table/XSkillEffectMgr.h"
#include "SkillList.h"
#include "gamelogic/emblemmgr.h"
#include "gamelogic/rolemisc.h"
#include "npcfeeling/npcfeelingrecord.h"

CalcRoleAttribute::CalcRoleAttribute()
{
}

CalcRoleAttribute::~CalcRoleAttribute()
{

}


bool CalcRoleAttribute::SetupBasicAttribute(int level, int prof, CombatAttribute *combatAttr)
{
	PlayerLevelTable::RowData *row = PlayerLevelManager::Instance()->GetLevelInfo(level);
	if (row == NULL)
	{
		return false;
	}
	
	double Attr = 0;
	int Lv = ProfessionMgr::PromoteLevel(prof);
	switch(Lv)
	{
	case 1: Attr = row->InitialAttr; break; // 未转职属性
	case 2: Attr = row->Trans1Attr; break;  // 1转属性
	case 3: Attr = row->Trans2Attr; break;  // 2转属性
	default: break;
	}
	
	assert(Attr != 0);

	combatAttr->Clear();

	auto percentConf = ProfessionMgr::Instance()->GetProfessionGrowthPercentConf(prof);
	int Total = percentConf->Agility + percentConf->Intelligence + percentConf->Vitality + percentConf->Strength;

	combatAttr->SetAttr(BASIC_Strength, percentConf->Strength * Attr / Total);
	combatAttr->SetAttr(BASIC_Agility, percentConf->Agility * Attr / Total);
	combatAttr->SetAttr(BASIC_Intelligence, percentConf->Intelligence * Attr / Total);
	combatAttr->SetAttr(BASIC_Vitality, percentConf->Vitality * Attr / Total);
	combatAttr->SetAttr(BASIC_MaxMP, row->MaxMP);

	combatAttr->SetAttr(BASIC_RUN_SPEED,  GetGlobalConfig().RunSpeed);
	combatAttr->SetAttr(BASIC_WALK_SPEED,  GetGlobalConfig().WalkSpeed);
	combatAttr->SetAttr(BASIC_DASH_SPEED,  GetGlobalConfig().DashSpeed);
	combatAttr->SetAttr(BASIC_ROTATION_SPEED,  GetGlobalConfig().TurnSpeed);
	combatAttr->SetAttr(BASIC_AUTOROTATION_SPEED,  GetGlobalConfig().AutoTurnSpeed);
	combatAttr->SetAttr(BASIC_ATTACK_SPEED,  GetGlobalConfig().AttackSpeed);
	combatAttr->SetAttr(BASIC_SKILL_CD,  GetGlobalConfig().SkillCD);
	combatAttr->SetAttr(BASIC_ENMITY,  GetGlobalConfig().GeneralCombatParam);
	combatAttr->SetAttr(BASIC_XULI,  GetGlobalConfig().GeneralCombatParam);
	//combatAttr->SetAttr(BASIC_MaxSuperArmor, 10000);

	UINT32 basicProf = ProfessionMgr::Instance()->GetBasicProfession(prof); 
	int MpRecovery = (basicProf - 1) >= GetGlobalConfig().MpRecovery.size() ? 5 : GetGlobalConfig().MpRecovery[basicProf - 1];
	combatAttr->SetAttr(BASIC_MPRecovery,  MpRecovery);

	combatAttr->SetAttr(BASIC_SuperArmorAtk, row->SuperArmorAttack[basicProf-1]);
	combatAttr->SetAttr(BASIC_SuperArmorDef, row->SuperArmorDefend[basicProf-1]);
	combatAttr->SetAttr(BASIC_SuperArmorReg, row->SuperArmorRecovery[basicProf-1]);
	combatAttr->SetAttr(BASIC_MaxSuperArmor, row->SuperArmorMax[basicProf-1]);

	return true;
}


bool CalcRoleAttribute::Execute(Role *pRole)
{
	CombatAttribute *combatAttr = pRole->GetAttrPtr();

	if (SetupBasicAttribute(pRole->GetLevel(), pRole->GetProfession(), combatAttr) == false)
	{
		return false;
	}
	///> 重新计算装备属性
	pRole->Get<Bag>()->ApplyEquipEffect(combatAttr);

	///> 重新计算时装属性
	pRole->Get<RoleFashion>()->ApplyEffect(combatAttr);

	///> 称号
	DesignationMgr::Instance()->ApplyEffect(pRole, combatAttr);

	///> 头衔
	pRole->Get<TitleRecord>()->ApplyEffect(combatAttr);

	///> pet
	pRole->Get<PetSys>()->ApplyEffect(combatAttr);

	///> 精灵
	pRole->Get<CSpriteRecord>()->ApplyEffect(combatAttr);

	///> 图鉴数据加成
	pRole->Get<CAtlasSys>()->ApplyEffect(combatAttr);

	///> 公会技能
	pRole->Get<CGuildRecord>()->ApplyEffect(combatAttr);

	///> npc好感度加成
	pRole->Get<CNpcFeelingRecord>()->ApplyEffect(combatAttr);

	///> 恢复buff属性
	if (NULL != pRole->GetCurrScene() && SceneConfig::Instance()->IsBattleSceneExcept(pRole->GetCurrScene()->GetSceneType()))
	{
		pRole->GetXRole()->BuffComponent()->ReApplyChangeAttributeEffect();
	}

	///> GM增加属性
	pRole->Get<RoleMisc>()->ApplyGMAttr(combatAttr);

	///> 计算一级属性到二级属性的转换
	CalcAttributeConvert(combatAttr, pRole->GetProfession());

	RerangeAttribute(combatAttr);
	
	CalcPowerPoint(combatAttr, pRole->GetRoleSkillLevelMgr()->GetUsedSkillPoint(), pRole);
	
	return true;
}

void CalcRoleAttribute::CalcAttributeConvert(CombatAttribute *combatAttr, int prof)
{
	CombatAttrDef FirstAttribute[] = { BASIC_Strength, BASIC_Agility, BASIC_Intelligence, BASIC_Vitality };
	for (unsigned i = 0; i < sizeof(FirstAttribute)/sizeof(FirstAttribute[0]); ++i)
	{
		auto pConf = ProfessionMgr::Instance()->GetProfessionAttrConvertParams(prof, FirstAttribute[i]);
		double value = combatAttr->GetAttr((CombatAttrDef)(CA_TOTAL_START+FirstAttribute[i]));

		if(value < 1e-6) continue;

		combatAttr->AddAttr(BASIC_PhysicalAtk ,pConf->PhysicalAtk*value);
		combatAttr->AddAttr(BASIC_PhysicalDef ,pConf->PhysicalDef*value);
		combatAttr->AddAttr(BASIC_MagicAtk ,pConf->MagicAtk*value);
		combatAttr->AddAttr(BASIC_MagicDef ,pConf->MagicDef*value);
		combatAttr->AddAttr(BASIC_Critical ,pConf->Critical*value);
		combatAttr->AddAttr(BASIC_CritResist ,pConf->CritResist*value);
		combatAttr->AddAttr(BASIC_Stun ,pConf->Stun*value);
		combatAttr->AddAttr(BASIC_StunResist ,pConf->StunResist*value);
		combatAttr->AddAttr(BASIC_Paralyze ,pConf->Paralyze*value);
		combatAttr->AddAttr(BASIC_ParaResist ,pConf->ParaResist*value);
		combatAttr->AddAttr(BASIC_MaxHP ,pConf->MaxHP*value);
		combatAttr->AddAttr(BASIC_MaxMP ,pConf->MaxMP*value);
		combatAttr->AddAttr(BASIC_CombatSocre, pConf->CombatScore*value);
		combatAttr->AddAttr(BASIC_CritDamage, pConf->CritDamage*value);
	}
}

void CalcRoleAttribute::CalcPowerPoint(CombatAttribute *combatAttr, int SkillPoint, Role* pRole, int prof)
{
	if (pRole != NULL)
	{
		prof = pRole->GetProfession();
	}
	double ppt = PowerPointMgr::Instance()->CalcPowerPoint(combatAttr, prof);
	ppt += GetGlobalConfig().SkillPointPPTWeight * SkillPoint;
	
	if (pRole == NULL)
	{
		combatAttr->SetAttr(BASIC_POWERPOINT, ppt);
		return;
	}

	Bag* pBag = pRole->Get<Bag>();
	const std::vector<UINT32>& bindSkill = pRole->Get<RoleSkillSet>()->GetBindSkillIDS();
	std::vector<UINT32> helpSkill;

	for (auto i = pRole->Get<RoleSkillSet>()->begin(); i != pRole->Get<RoleSkillSet>()->end(); i++)
	{
		if (i->isbasic)
		{
			continue;
		}
		SkillList::RowData* pRow = XSkillEffectMgr::Instance()->GetSkillConfig(i->skillhash, i->skilllevel);
		if (pRow != NULL && pRow->SkillType == KKSG::Skill_Help)
		{
			helpSkill.push_back(i->skillhash);
		}
	}

	for (UINT32 i = 0; i < SkillEmblemCount; i++) //技能纹章战力
	{
		const SkEmblem* pSk = pBag->GetWearedSkillEmblemBySlot(i);
		if (pSk == NULL)
		{
			continue;
		}
		bool isHave = false;
		for (auto j = bindSkill.begin(); j != bindSkill.end(); j++)
		{
			if (pSk->IsEqualHash(*j))
			{
				isHave = true;
				break;
			}
		}
		for (auto j = helpSkill.begin(); j != helpSkill.end() && !isHave; j++)
		{
			if (pSk->IsEqualHash(*j))
			{
				isHave = true;
				break;
			}
		}
		if (isHave)
		{
			ppt += pSk->conf->SkillPPT;
		}
	}

	combatAttr->SetAttr(BASIC_POWERPOINT, ppt);
}

void CalcRoleAttribute::RerangeAttribute(CombatAttribute *combatAttr)
{
	combatAttr->SetAttr(BASIC_CurrentHP, combatAttr->GetAttr(TOTAL_MaxHP));
	combatAttr->SetAttr(BASIC_CurrentMP, combatAttr->GetAttr(TOTAL_MaxMP));
	combatAttr->SetAttr(BASIC_PhysicalAtkMod, combatAttr->GetAttr(BASIC_PhysicalAtk));
	combatAttr->SetAttr(BASIC_PhysicalDefMod, combatAttr->GetAttr(BASIC_PhysicalDef));
	combatAttr->SetAttr(BASIC_MagicAtkMod,    combatAttr->GetAttr(BASIC_MagicAtk));
	combatAttr->SetAttr(BASIC_MagicDefMod,    combatAttr->GetAttr(BASIC_MagicDef));
}

