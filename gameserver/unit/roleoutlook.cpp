#include "pch.h"
#include "roleoutlook.h"
#include "role.h"
#include "scene/scene.h"
#include "gamelogic/designationRecord.h"
#include "gamelogic/designationMgr.h"
#include "unit/pet.h"
#include "gamelogic/titlerecord.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "gamelogic/sprite.h"
#include "gamelogic/spriterecord.h"
#include "gamelogic/military_rank_record.h"
#include "gamelogic/rolefashion.h"
#include "gamelogic/payv2Record.h"
RoleOutLook::RoleOutLook(Role* role)
:m_role(role)
{
	m_ischanged = false;
	m_ntf.m_Data.set_roleid(role->GetID());
	m_field = 0;
}

RoleOutLook::~RoleOutLook()
{

}

void RoleOutLook::Fill(UINT32 field, void* param)
{
	if (SUMMARY_EQUIP & field)
	{
		FillEquip(m_ntf.m_Data.mutable_outlook(), m_role, param);
		m_ischanged =  true;
	}
	if (SUMMARY_DESIGNATION & field)
	{
		FillDesignation(m_ntf.m_Data.mutable_outlook(), m_role);
		m_ischanged = true; 
	}
	if (SUMMARY_TITLE & field)
	{
		FillTitle(m_ntf.m_Data.mutable_outlook(), m_role);
		m_ischanged = true;
	}
	if (SUMMARY_GUILD & field)
	{
		FillGuild(m_ntf.m_Data.mutable_outlook(), m_role);
		m_ischanged = true;
	}
	if (SUMMARY_OP & field)
	{
		FillOp(m_ntf.m_Data.mutable_outlook(), m_role);
		m_ischanged = true;
	}
	if (SUMMARY_SPRITELEADER & field)
	{
		FillSprite(m_ntf.m_Data.mutable_outlook(), m_role);
		m_ischanged = true;
	}
	if (SUMMARY_STATE & field)
	{
		FillState(m_ntf.m_Data.mutable_outlook(), m_role);
		m_ischanged = true;
	}
	if (SUMMARY_MILITARY_RANK & field)
	{
		FillMilitaryRank(m_ntf.m_Data.mutable_outlook(), m_role);
		m_ischanged = true;
	}
	if (SUMMARY_DISPLAYFASHION & field)
	{
		FillDisplayFashion(m_ntf.m_Data.mutable_outlook(), m_role);
		m_ischanged = true;
	}
	

	if (SUMMARY_PAYSCOREPRE & field)
	{
		FillPayScore(m_ntf.m_Data.mutable_outlook(), m_role);
		m_ischanged = true;
	}
	m_field |= field;
}

void RoleOutLook::BroadCast()
{
	if (m_ischanged && NULL != m_role->GetCurrScene())
	{
		m_role->GetCurrScene()->Broadcast(m_role, m_ntf, m_field & SUMMARY_STATE ? NULL : m_role);
	}
}

void RoleOutLook::SendToSelf()
{
	m_role->Send(m_ntf);
}

bool RoleOutLook::FillEquip(KKSG::OutLook* tempout, Role* role, void* param)
{
	KKSG::OutLookEquip* out = tempout->mutable_equips();
	Bag* bag = role->Get<Bag>();
	if (NULL != param)
	{
		XItem* item = bag->GetItemOnBody(EQUIP, *(int*)param);
		out->add_slot(*(int*)param);
		if (NULL != item)
		{
			out->add_itemid(item->itemID);
			out->add_enhancelevel(NULL == item->enhanceInfo ? 0 : item->enhanceInfo->level);
		}
		else
		{
			out->add_itemid(0);
			out->add_enhancelevel(0);
		}
	}
	else
	{
		bag->GetOutLook(*out);
	}
	out->set_enhancemaster(bag->GetEnhanceMaster());
	return true;
}

bool RoleOutLook::FillDesignation(KKSG::OutLook* tempout, Role* role)
{
	KKSG::OutLookDesignation* out = tempout->mutable_designation();
	DesignationRecord* record = role->Get<DesignationRecord>();
	out->set_id(record->GetCoverDesignation());
	out->set_name(record->GetName());
	return true;
}

bool RoleOutLook::FillGuild(KKSG::OutLook* tempout, Role* role)
{
	CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (NULL != guild)
	{
		KKSG::OutLookGuild* out = tempout->mutable_guild();
		out->set_name(guild->GetGuildName());
		out->set_icon(guild->GetGuildIcon());
		out->set_id(guild->GetGuildId());
		return true;
	}
	else
	{
		KKSG::OutLookGuild* out = tempout->mutable_guild();
		out->Clear();
		return false;
	}
	return true;
}

bool RoleOutLook::FillTitle(KKSG::OutLook* tempout, Role* role)
{
	KKSG::OutLookTitle* out = tempout->mutable_title();
	out->set_titleid(role->Get<TitleRecord>()->GetTitle());
	return true;
}

bool RoleOutLook::FillOp(KKSG::OutLook* tempout, Role* role)
{
	if (role->GetRoleAllInfo().has_brief() && role->GetRoleAllInfo().brief().has_op())
	{
		KKSG::OutLookOp* out = tempout->mutable_op();
		*out = role->GetRoleAllInfo().brief().op();
	}
	return true;
}

bool RoleOutLook::FillSprite(KKSG::OutLook* tempout, Role* role)
{
	Sprite* pSprite = role->Get<CSpriteRecord>()->GetInFightSprite(0);
	if(NULL == pSprite)
	{
		tempout->mutable_sprite()->set_leaderid(0);
	}
	else
	{
		tempout->mutable_sprite()->set_leaderid(pSprite->GetSpriteID());
	}
	return true;
}

bool RoleOutLook::FillState(KKSG::OutLook* tempout, Role* role)
{
	tempout->mutable_state()->CopyFrom(role->GetState());
	return true;
}

bool RoleOutLook::FillMilitaryRank(KKSG::OutLook* tempout, Role* role)
{
	KKSG::OutLookMilitaryRank* out = tempout->mutable_military();
	out->set_military_rank(role->Get<MilitaryRankRecord>()->GetMilitaryRank());
	return true;
}

bool RoleOutLook::FillDisplayFashion(KKSG::OutLook* out, Role* role)
{
	KKSG::OutLookDisplayFashion *display_fashion = out->mutable_display_fashion();
	role->Get<RoleFashion>()->GetDisplayFashion(display_fashion);
	return true;
}

bool RoleOutLook::FillWeddingDisplayFash(KKSG::OutLook* out, Role* role)
{
	KKSG::OutLookDisplayFashion *display_fashion = out->mutable_display_fashion();
	role->Get<RoleFashion>()->GetWeddingDisplayFashion(display_fashion);
	return true;
}

void RoleOutLook::FillDisplayFashion()
{
	FillDisplayFashion(m_ntf.m_Data.mutable_outlook(), m_role);
}

void RoleOutLook::FillWeddingDisplayFash()
{
	FillWeddingDisplayFash(m_ntf.m_Data.mutable_outlook(), m_role);
}

bool RoleOutLook::FillPayScore(KKSG::OutLook* out, Role* role)
{
	KKSG::OutLookConsumePre* pScore = out->mutable_pre();
	role->Get<CPayV2Record>()->GetPayScorePre(pScore);
	return true;
}
