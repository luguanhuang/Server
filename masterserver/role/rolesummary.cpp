#include "pch.h"
#include "time.h"
#include "rolesummary.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/rolemanager.h"
#include "team/tarjateammgr.h"
#include "util/gametime.h"

CRoleSummary::CRoleSummary()
{
	logouttime = 0;
	lastupdatetime = 0;
}

CRoleSummary::~CRoleSummary()
{
}

void CRoleSummary::SaveToTeamMember(KKSG::TeamMember *pMemberData)
{
	if(NULL == pMemberData)
	{
		return ;
	}

	UINT32 nTime = TarjaTeamMgr::Instance()->GetTime(GetID());
	if (nTime > GameTime::GetTime())
	{
		LogDebug("--SaveToTeamMember=%llu-- ntime=%u", pMemberData->memberid(),nTime);
		pMemberData->set_tarjatime(nTime);
	}

	pMemberData->set_serverid(MSConfig::Instance()->GetServerID());
	pMemberData->set_name(GetName());
	pMemberData->set_level(GetLevel());
	pMemberData->set_profession((KKSG::RoleType)GetProfession());
	pMemberData->set_powerpoint((int)GetPowerPoint());
	pMemberData->set_viplevel(GetVipLevel());
	pMemberData->set_paymemberid(GetPayMemberID());
	for(int i = 0; i < m_oData.fashion_size(); ++i)
	{
		pMemberData->add_fashion(m_oData.fashion(i));
	}

	SetOutLook(pMemberData->mutable_outlook());
}

void CRoleSummary::SetOutLook(KKSG::OutLook* outlook)
{
	KKSG::OutLookEquip* equip = outlook->mutable_equips();
	for (int i = 0; i < m_oData.equip_size(); ++i)
	{
		const KKSG::Item& item = m_oData.equip(i);
		equip->add_slot(i);
		equip->add_itemid(item.itemid());
		equip->add_enhancelevel(item.enhancelevel());
	}

	if (GetData().has_op())
	{
		KKSG::OutLookOp* op = outlook->mutable_op();
		*op = GetData().op();
	}

	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(GetID());
	if (NULL != guild)
	{
		KKSG::OutLookGuild* tempguild = outlook->mutable_guild();
		tempguild->set_name(guild->GetName());
		tempguild->set_icon(guild->GetIcon());
	}

	KKSG::OutLookDesignation* desigation = outlook->mutable_designation();
	desigation->set_id(GetCoverDesignationID());

	KKSG::OutLookTitle* title = outlook->mutable_title();
	title->set_titleid(GetTitleID());

	KKSG::OutLookSprite* sprite = outlook->mutable_sprite();
	sprite->set_leaderid(GetSpriteLeaderID());

	KKSG::OutLookMilitaryRank* military = outlook->mutable_military();
	military->set_military_rank(GetMilitaryRank());

	// ÉèÖÃÍâÏÔ
	KKSG::OutLookDisplayFashion* display = outlook->mutable_display_fashion();
	for (int i = 0; i < m_oData.display_fashion_size(); ++i)
	{
		display->add_display_fashions(m_oData.display_fashion(i));
	}

	display->set_hair_color_id(m_oData.hair_color_id());
	display->set_special_effects_id(m_oData.fashion_specialeffects_id());

	KKSG::OutLookConsumePre* pScore =  outlook->mutable_pre();
	for (UINT32 i = 0; i < m_oData.pre().setid_size(); i ++)
	{
		pScore->add_setid(m_oData.pre().setid(i));
	}
}

void CRoleSummary::Build(KKSG::UnitAppearance* appearance, int mask)
{
	if (SUMMARY_GUILD & mask)
	{
		Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(GetID());
		if (NULL != guild)
		{
			KKSG::OutLookGuild* tempguild = appearance->mutable_outlook()->mutable_guild();
			tempguild->set_name(guild->GetName());
			tempguild->set_icon(guild->GetIcon());
			tempguild->set_id(guild->GetID());
		}
	}
	if (SUMMARY_LASTLOGOUT & mask)
	{
		appearance->set_lastlogin(GetLastLogout());
	}
	RoleSummaryBase::Build(appearance, mask);
}

bool CRoleSummary::IsPushTypeForbid(UINT32 type)
{
	for (INT32 i = 0; i < m_oData.push_info().configs_size(); ++i)
	{
		const KKSG::PushConfig& item = m_oData.push_info().configs(i);
		if (item.type() == type)
		{
			return item.forbid();
		}
	}
	return false;
}

void CRoleSummary::FillSmallInfo(KKSG::RoleSmallInfo& data)
{
	data.set_roleid(GetID());
	data.set_rolename(GetName());
	data.set_rolelevel(GetLevel());
	data.set_roleprofession(GetProfession());
	data.set_roleppt(GetPowerPoint());
}

void CRoleSummary::FillRoleOutLookBrief(KKSG::RoleOutLookBrief& data)
{
	data.set_roleid(GetID());
	data.set_profession((KKSG::RoleType)GetProfession());
	data.set_name(GetName());
	data.set_level(GetLevel());
	data.set_ppt(GetPowerPoint());
	SetOutLook(data.mutable_outlook());
	data.set_viplevel(GetVipLevel());
	data.set_paymemberid(GetPayMemberID());
	for(int i = 0; i < m_oData.fashion_size(); ++i)
	{
		data.add_fashion(m_oData.fashion(i));
	}
}

void CRoleSummary::SetLastLogout(UINT32 _time)
{
	logouttime = 0 == _time ? (UINT32)time(NULL) : _time;
}

UINT32 CRoleSummary::GetLastLogout()
{
	if (CRoleManager::Instance()->IsOnline(m_oData.roleid()))
	{
		return 0;
	}
	UINT32 nowtime = GameTime::GetTime();
	return (0 != logouttime && nowtime > logouttime) ? (UINT32)(nowtime - logouttime): 0;
}

void CRoleSummary::FillRolePvpInfo(KKSG::PvpRoleInfo& info)
{
	info.set_roleid(GetID());
	info.set_name(GetName());	
	info.set_level(GetLevel());
	info.set_profession(GetProfession());
	info.set_military_rank(GetMilitaryRank());
}

void CRoleSummary::FillWeddingRoleInfo(KKSG::WeddingRoleBrief& info)
{
	info.set_roleid(GetID());
	info.set_name(GetName());	
	info.set_level(GetLevel());
	info.set_profession(GetProfession());
	info.set_ppt(GetPowerPoint());
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(GetID());
	if (guild)
	{
		info.set_guildname(guild->GetName());
	}
}

void CRoleSummary::FillDailyTaskRefreshRoleInfo(KKSG::DailyTaskRefreshRoleInfo& info)
{
	info.set_roleid(GetID());
	info.set_name(GetName());
	info.set_profession((KKSG::RoleType)GetProfession());
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(GetID());
	info.set_is_online(pRole != NULL);
}

