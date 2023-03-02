#include "pch.h"
#include "time.h"
#include "unit/role.h"
#include "rolefashion.h"
#include "rolesummary.h"
#include "payv2Record.h"
#include "designationRecord.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "unit/rolemanager.h"
#include "titlerecord.h"
#include "spriterecord.h"
#include "sprite.h"
#include "foreach.h"
#include "unit/calcattribute.h"
#include "table/PowerPointMgr.h"
#include "table/PushTableConfig.h"
#include "payconfig.h"
#include "gamelogic/rolepushinforecord.h"
#include "gamelogic/qqviprecord.h"
#include "reportdatamgr.h"
#include "gamelogic/pkrecord.h"
#include "util/gametime.h"
#include "gamelogic/military_rank_record.h"
#include "gamelogic/designationMgr.h"

RoleSummary::RoleSummary()
{
	istodelete = false;
	IsRobot = false;
	qwRobotID = 0;

	oSprites.resize(GetGlobalConfig().SpritePositionLevel.size());
	for(UINT32 i = 0; i < oSprites.size(); i++)
	{
		oSprites[i].set_spriteid(0);
	}
	lastupdatetime = 0;
}

void RoleSummary::Update(Role *pRole, int mask, void* arg)
{
	lastupdatetime = (UINT32)time(NULL);

	m_oData.set_roleid(pRole->GetID());
	m_oData.set_account(pRole->GetAccount());
	m_oData.set_device_id(pRole->GetClientConf().deviceid());
	m_oData.set_plat_type(pRole->GetClientConf().platid());
	m_oData.set_pkpoint(pRole->Get<CPkRecord>()->GetPoint());
	m_oData.set_token(pRole->GetClientConf().token());
	m_oData.set_backflowendtime(pRole->GetBackFlowEndTime());

	if (SUMMARY_LEVEL & mask)
	{
		if (pRole->GetLevel() > (int)GetLevel())
		{
			m_oData.set_leveluptime((UINT32)time(NULL));
		}
		m_oData.set_level(pRole->GetLevel());
	}
	if (SUMMARY_PROFESSION & mask)
	{
		m_oData.set_profession(pRole->GetProfession());
	}
	if (SUMMARY_NAME & mask)
	{
		m_oData.set_name(pRole->GetName());
	}
	if (SUMMARY_ATTR & mask)
	{
		oAttribute = *pRole->GetAttrPtr();
		UINT32 ppt = (UINT32)oAttribute.GetAttr(TOTAL_POWERPOINT);
		if (ppt > GetPowerPoint())
		{
			m_oData.set_pptuptime((UINT32)time(NULL));
		}
		if (ppt != GetPowerPoint())
		{
			ReportDataMgr::Instance()->OnChangePPT(pRole);
		}
		m_oData.set_powerpoint(ppt);
	}
	if (SUMMARY_FASHION & mask)
	{
		oFashion = pRole->Get<RoleFashion>()->GetFashion();

		CombatAttribute combatAttr;
		pRole->Get<RoleFashion>()->ApplyEffect(&combatAttr);
		CalcRoleAttribute fashionAttr;
		fashionAttr.CalcAttributeConvert(&combatAttr, pRole->GetProfession());

		m_oData.set_fashionppt((UINT32)PowerPointMgr::Instance()->CalcPowerPoint(&combatAttr, pRole->GetProfession()));
		m_oData.set_fashionpptuptime((UINT32)time(NULL));
	}
	if (SUMMARY_EQUIP & mask)
	{
		pRole->Get<Bag>()->SetSummaryEquip(&oEquip, arg);
		m_oData.set_enhancemaster(pRole->Get<Bag>()->GetEnhanceMaster());		
	}
	if (SUMMARY_SKILL & mask)
	{
		oSkills = pRole->GetRoleSkillLevelMgr()->GetSyncSkill();
	}
	if (SUMMARY_EMBLEM & mask)
	{
		pRole->Get<Bag>()->SetSummaryEmblem(&oEmblem, arg);
	}
	if (SUMMARY_ARTIFACT & mask)
	{
		pRole->Get<Bag>()->SetSummaryArtifact(&oArtifact, arg);
	}
	if (SUMMARY_VIPLEVEL & mask)
	{
		m_oData.set_viplevel(pRole->Get<CPayV2Record>()->GetVipLevel());
	}
	if (SUMMARY_DESIGNATION & mask)
	{
		UINT32 nDesID = pRole->Get<DesignationRecord>()->GetCoverDesignation();
		stDesignationInfo* pDesi =  pRole->Get<DesignationRecord>()->GetDesignationInfo(nDesID);
		if (pDesi)
		{
			m_oData.set_desigation(nDesID);
			m_oData.set_covername(pDesi->szName);
			m_oData.set_covertype(pDesi->dwType);
		}
	}
	if (SUMMARY_NICKID & mask)
	{
		m_oData.set_nickid(pRole->GetNickId());
	}
	if (SUMMARY_TITLE & mask)
	{
		m_oData.set_titleid(pRole->Get<TitleRecord>()->GetTitle());
	}
	if (SUMMARY_OP & mask)
	{
		*m_oData.mutable_op() = pRole->GetRoleAllInfoPtr()->mutable_brief()->op();
	}
	if (SUMMARY_SPRITE & mask)
	{
		pRole->Get<CSpriteRecord>()->SetSummarySprite(oSprites, arg);
	}
	if (SUMMARY_PUSHINFO & mask)
	{
		pRole->Get<RolePushInfoRecord>()->ToKKSG(*m_oData.mutable_push_info());
	}
	if ((SUMMARY_PETS & mask) && (NULL != arg))
	{
		UINT64 petUid = *(UINT64*)arg;
		Pet* pPet = pRole->Get<PetSys>()->GetPet(petUid);
		if (pPet != NULL)
		{
			KKSG::PetSingle& petSingle = oPetInfo.m_rolePetMap[petUid];
			petSingle.set_uid(pPet->uid);
			petSingle.set_petid(pPet->petid);
			petSingle.set_level(pPet->level);
			petSingle.set_exp(pPet->exp);
			petSingle.set_sex(pPet->sex);
			petSingle.set_power(pPet->power);
			petSingle.set_mood(pPet->mood);
			petSingle.set_hungry(pPet->hungry);

			petSingle.clear_fixedskills();
			foreach (i in pPet->fixedSkills)
			{
				petSingle.add_fixedskills(*i);
			}

			petSingle.clear_randskills();
			foreach (j in pPet->randSkills)
			{
				petSingle.add_randskills(*j);
			}
		}
		else
		{
			oPetInfo.m_rolePetMap.erase(petUid);
		}
		m_oData.set_fightpetid(pRole->Get<PetSys>()->GetFightId()); 
	}
	else if (SUMMARY_PETS & mask)
	{
		std::map<UINT64, Pet>& allPet = pRole->Get<PetSys>()->GetAllPet();
		foreach (k in allPet)
		{
			KKSG::PetSingle& petSingle = oPetInfo.m_rolePetMap[k->first];
			petSingle.set_uid(k->second.uid);
			petSingle.set_petid(k->second.petid);
			petSingle.set_level(k->second.level);
			petSingle.set_exp(k->second.exp);
			petSingle.set_sex(k->second.sex);
			petSingle.set_power(k->second.power);
			petSingle.set_mood(k->second.mood);
			petSingle.set_hungry(k->second.hungry);

			petSingle.clear_fixedskills();
			foreach (i in k->second.fixedSkills)
			{
				petSingle.add_fixedskills(*i);
			}

			petSingle.clear_randskills();
			foreach (j in k->second.randSkills)
			{
				petSingle.add_randskills(*j);
			}
		}
		m_oData.set_fightpetid(pRole->Get<PetSys>()->GetFightId());
	}
	 

	if (SUMMARY_PAYMEMBER & mask)
	{
		CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
		m_oData.clear_privilege();
		for (auto it = pRecord->memberInfo.begin(); it != pRecord->memberInfo.end(); it ++)
		{
			if (it->expiretime() >= INT32(GameTime::GetTime()))
			{
				KKSG::PayMember* pMem = m_oData.add_privilege();
				pMem->set_id(it->id());
				pMem->set_expiretime(it->expiretime());
			}
		}
	}
	if (SUMMARY_QQVIP & mask)
	{
		pRole->Get<QQVipRecord>()->ToKKSG(*m_oData.mutable_qqvip());
	}
	if (SUMMARY_STARTUP & mask)
	{
		m_oData.set_starttype(pRole->GetStartUpType());
	}
	if (SUMMARY_MILITARY_RANK & mask)
	{
		m_oData.clear_military_record();
		KKSG::MilitaryRecord* military = m_oData.mutable_military_record();
		military->set_military_rank(pRole->Get<MilitaryRankRecord>()->GetMilitaryRank());
		military->set_military_exploit(pRole->Get<MilitaryRankRecord>()->GetMilitaryExploit());
	}
	if (SUMMARY_DISPLAYFASHION & mask)
	{
		m_oData.clear_display_fashion();
		std::set<UINT32> fashions;
		pRole->Get<RoleFashion>()->GetDispplayFashion(fashions);
		auto it = fashions.begin();
		auto it_end = fashions.end();
		for (; it != it_end; ++it)
		{
			m_oData.add_display_fashion(*it);
		}
		m_oData.set_hair_color_id(pRole->Get<RoleFashion>()->GetDispplayHairColor());	
		m_oData.set_fashion_specialeffects_id(pRole->Get<RoleFashion>()->GetDispplaySpecialEffectsID());		
	}

	if (SUMMARY_PAYSCOREPRE & mask)
	{
		m_oData.clear_pre();
		KKSG::PayConsume* pScore = m_oData.mutable_pre();
		pRole->Get<CPayV2Record>()->GetPayScorePre(pScore);
	}
}
