#include "pch.h"
#include "rolesummarybase.h"
#include "timeutil.h"
#include "table/OpenSystemMgr.h"
#include "define/systemiddef.h"
#include "util/gametime.h"
#include "tablemgr/prerogativemgr.h"

bool RoleSummaryBase::Init(const KKSG::RoleSummaryStored& rodata)
{
	m_oData = rodata;
	SetMaxPPT(m_oData.powerpoint());
	return true;
}

void RoleSummaryBase::SetUnitBasic(KKSG::UnitAppearance* pUnitAppearnce)
{
	pUnitAppearnce->set_uid(GetID());
	pUnitAppearnce->set_level(GetLevel());
	pUnitAppearnce->set_unittype(GetProfession());
	pUnitAppearnce->set_powerpoint(GetPowerPoint());
	SetMaxPPT(m_oData.powerpoint());
	pUnitAppearnce->set_unitname(GetName());
	pUnitAppearnce->set_isservercontrol(false);
	pUnitAppearnce->set_isdead(false);
	pUnitAppearnce->set_nickid(GetNickID());
	pUnitAppearnce->set_viplevel(GetVipLevel());
	pUnitAppearnce->set_paymemberid(GetPayMemberID());

	*pUnitAppearnce->mutable_outlook()->mutable_op() = *m_oData.mutable_op(); 
}

void RoleSummaryBase::SetUnitAttr(KKSG::UnitAppearance* pUnitAppearnce)
{
	*pUnitAppearnce->mutable_attributes() = m_oData.attribute();
}

void RoleSummaryBase::SetUnitFashion(KKSG::UnitAppearance* pUnitAppearnce)
{
	for (int i = 0; i < m_oData.fashion_size(); ++i)
	{
		pUnitAppearnce->add_fashion(m_oData.fashion(i));
	}
}

void RoleSummaryBase::SetUnitEquip(KKSG::UnitAppearance* pUnitAppearnce)
{
	for (int i = 0; i < m_oData.equip_size(); ++i)
	{
		KKSG::Item* item = pUnitAppearnce->add_equip();
		*item = m_oData.equip(i);
	}
	SetUnitEquipLook(pUnitAppearnce);
}

void RoleSummaryBase::SetUnitEquipLook(KKSG::UnitAppearance* pUnitAppearnce)
{
	KKSG::OutLookEquip* equip = pUnitAppearnce->mutable_outlook()->mutable_equips();
	for (int i = 0; i < m_oData.equip_size(); ++i)
	{
		const KKSG::Item& item = m_oData.equip(i);
		equip->add_slot(i);
		equip->add_itemid(item.itemid());
		equip->add_enhancelevel(item.enhancelevel());
	}
	equip->set_enhancemaster(m_oData.enhancemaster());
}

void RoleSummaryBase::SetUnitSkill(KKSG::UnitAppearance* pUnitAppearnce)
{
	for (int i = 0; i < m_oData.skills_size(); ++i)
	{
		KKSG::SkillInfo *pInfo = pUnitAppearnce->add_skills();
		*pInfo = m_oData.skills(i);
	}
	for (int j = 0; j < m_oData.bindskills_size(); ++j)
	{
		pUnitAppearnce->add_bindskills(m_oData.bindskills(j));
	}
}

void RoleSummaryBase::SetUnitEmblem(KKSG::UnitAppearance* pUnitAppearance)
{
	for (int i = 0; i < m_oData.emblem_size(); ++i)
	{
		KKSG::Item* item = pUnitAppearance->add_emblem();
		*item = m_oData.emblem(i);
	}
}

void RoleSummaryBase::SetUnitArtifact(KKSG::UnitAppearance* pUnitAppearance)
{
	for (int i = 0; i < m_oData.artifact_size(); ++i)
	{
		KKSG::Item* item = pUnitAppearance->add_artifact();
		*item = m_oData.artifact(i);
	}
}
void RoleSummaryBase::SetUnitDisplayFashionLook(KKSG::UnitAppearance* pUnitAppearnce)
{
	KKSG::OutLook*  out_look = pUnitAppearnce->mutable_outlook();
	out_look->clear_display_fashion();
	for (int i = 0; i < m_oData.display_fashion_size(); ++i)
	{
		out_look->mutable_display_fashion()->add_display_fashions(m_oData.display_fashion(i));	
	}
	//　头发信息
	out_look->mutable_display_fashion()->set_hair_color_id(m_oData.hair_color_id());

	// 如果外显空，就把穿的时装显示
	if (!out_look->has_display_fashion())
	{
		for (int i = 0; i < m_oData.fashion_size(); ++i)
		{			
			out_look->mutable_display_fashion()->add_display_fashions(m_oData.fashion(i));	
		}
	}

	//　时装特效信息
	out_look->mutable_display_fashion()->set_special_effects_id(m_oData.fashion_specialeffects_id());
}

void RoleSummaryBase::SetMilitaryRank(KKSG::UnitAppearance* pUnitAppearnce)
{
	KKSG::OutLook*  out_look = pUnitAppearnce->mutable_outlook();
	m_oData.clear_military_record();

	KKSG::OutLookMilitaryRank*  record = out_look->mutable_military();
	record->set_military_rank(m_oData.military_record().military_rank());
	
}

void RoleSummaryBase::SetPayScore(KKSG::UnitAppearance* pUnitAppearnce)
{
	KKSG::OutLookConsumePre* pOutLook = pUnitAppearnce->mutable_outlook()->mutable_pre();
	for (UINT32 i = 0; i < m_oData.pre().setid_size(); i ++)
	{
		pOutLook->add_setid(m_oData.pre().setid(i));
	}
}
void RoleSummaryBase::Build(KKSG::UnitAppearance* appearance, int mask)
{
	if (SUMMARY_BASIC & mask)
	{
		SetUnitBasic(appearance);
	}
	if (SUMMARY_ATTR & mask)
	{
		SetUnitAttr(appearance);
	}
	if (SUMMARY_FASHION & mask)
	{
		SetUnitFashion(appearance);
		SetUnitDisplayFashionLook(appearance);
	}
	if (SUMMARY_EQUIP & mask)
	{
		SetUnitEquip(appearance);		
	}
	if (SUMMARY_EQUIPOUT & mask)
	{
		SetUnitEquipLook(appearance);		
	}
	if (SUMMARY_SKILL & mask)
	{
		SetUnitSkill(appearance);
	}
	if (SUMMARY_EMBLEM & mask)
	{
		SetUnitEmblem(appearance);
	}
	if (SUMMARY_ARTIFACT & mask)
	{
		SetUnitArtifact(appearance);
	}
	if (SUMMARY_DESIGNATION & mask)
	{
		appearance->mutable_outlook()->mutable_designation()->set_id(GetCoverDesignationID());
	}
	if (SUMMARY_VIPLEVEL & mask)
	{
		appearance->set_viplevel(GetVipLevel());
	}

	if (SUMMARY_TITLE & mask)
	{
		appearance->mutable_outlook()->mutable_title()->set_titleid(GetTitleID());
	}
	if (SUMMARY_OP & mask)
	{
		*appearance->mutable_outlook()->mutable_op() = *m_oData.mutable_op(); 
	}
	if (SUMMARY_SPRITELEADER & mask)
	{
		appearance->mutable_outlook()->mutable_sprite()->set_leaderid(GetSpriteLeaderID());
	}
	if (SUMMARY_SPRITE & mask)
	{
		for (int i = 0; i < m_oData.sprites_size(); ++i)
		{
			KKSG::SpriteInfo* sprite = appearance->add_sprites();
			*sprite = m_oData.sprites(i);
		}
	}
	if (SUMMARY_PETS & mask)
	{
		 for (int i = 0; i < m_oData.pets_size(); ++i)
		 {
			if (m_oData.fightpetid() == m_oData.pets(i).uid())
			{
				KKSG::PetSingle* pData = appearance->mutable_pet();
				*pData = m_oData.pets(i);
			}
		 }
	}
	if (SUMMARY_PAYMEMBER & mask)
	{
		appearance->set_paymemberid(GetPayMemberID());
	}
	if (SUMMARY_LASTLOGOUT & mask)
	{
		// fill by derived
	}
	if (SUMMARY_GUILD & mask)
	{
		// fill by derived
	}
	//　时装外显
	if (SUMMARY_DISPLAYFASHION & mask)
	{
		SetUnitDisplayFashionLook(appearance);
	}
	
	if (SUMMARY_MILITARY_RANK & mask)
	{
		SetMilitaryRank(appearance);
	}

	if (SUMMARY_PAYSCOREPRE & mask)
	{
		SetPayScore(appearance);
	}
}

//玩家购买的特权有可能在下线后过期，需要保存过期时间判断;
UINT32 RoleSummaryBase::GetPayMemberID()
{
	UINT32 ID = 0;
	for (INT32 i = 0; i < m_oData.privilege_size(); i ++)
	{
		if (m_oData.privilege(i).expiretime() > INT32(time(NULL)))
		{
			ID |= 1 << m_oData.privilege(i).id();
		}
	}
	return ID;
}

bool RoleSummaryBase::IsQQVip()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_QQVIP))
	{
		return false;
	}
	const KKSG::QQVipInfo& info = m_oData.qqvip();
	if (!info.is_vip())
	{
		return false;
	}
	UINT32 now = TimeUtil::GetTime();
	if (now >= info.qq_vip_end())
	{
		return false;
	}
	return true;
}

bool RoleSummaryBase::IsQQSVip()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_QQVIP))
	{
		return false;
	}
	const KKSG::QQVipInfo& info = m_oData.qqvip();
	if (!info.is_svip())
	{
		return false;
	}
	UINT32 now = TimeUtil::GetTime();
	if (now >= info.qq_svip_end())
	{
		return false;
	}
	return true;
}

bool RoleSummaryBase::IsQQYearVip()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_QQVIP))
	{
		return false;
	}
	const KKSG::QQVipInfo& info = m_oData.qqvip();
	if (!info.is_year_vip())
	{
		return false;
	}
	UINT32 now = TimeUtil::GetTime();
	if (now >= info.qq_year_vip_end())
	{
		return false;
	}
	return true;
}

KKSG::StartUpType RoleSummaryBase::GetStartUpType()
{ 
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_QQVIP))
	{
		return KKSG::StartUp_Normal;;
	}
	return m_oData.starttype(); 
}

void RoleSummaryBase::GetFashion(std::vector<UINT32>& fashions)
{
	fashions.clear();
	for(int i = 0; i < m_oData.fashion_size(); ++i)
	{
		fashions.push_back(m_oData.fashion(i));
	}
}


UINT32 RoleSummaryBase::GetMilitaryRank()
{
	if (m_oData.has_military_record())
	{
		return m_oData.military_record().military_rank();
	}

	return 0;
}

bool RoleSummaryBase::IsBackFlowRole()
{
	UINT32 now = GameTime::GetTime();
	return now < m_oData.backflowendtime();
}
