#include "pch.h"
#include "gmfbase.h"
#include "pb/project.pb.h"
#include <time.h>
#include "gamelogic/rolesummarymgr.h"
#include "guild/ptcg2c_gmfbasedatantf.h"
#include "unit/rolemanager.h"
#include "guildmgr.h"
#include "guild/guilddef.h"

GuildMFHalfRoom::GuildMFHalfRoom()
{
	m_guildID = 0;
	m_prestigerank = 0;
}

GuildMFRole* GuildMFHalfRoom::GetGuildMFRoleByRoleID(UINT64 roleID)
{
	for(size_t i = 0; i < m_roles.size(); ++i)
	{
		if(m_roles[i].roleID == roleID)
		{
			return (&m_roles[i]);		
		}
	}
	return NULL;
}

GuildMFRole* GuildMFHalfRoom::GetGulldMFRoleByIndex(UINT32 index)
{
	if(index >= m_roles.size())
	{
		return NULL;
	}
	return (&m_roles[index]);
}

bool GuildMFHalfRoom::HaveRoleID(UINT64 roleID)
{
	for(UINT32 i = 0; i < m_roles.size(); ++i)
	{
		if(m_roles[i].roleID == roleID)
		{
			return true;
		}
	}
	return false;
}

void GuildMFHalfRoom::AddRoleID(UINT64 roleID, const std::string& name, UINT32 pkpoint, int profession)
{
	SSDebug << __FUNCTION__ << " roleid = " << roleID << END;
	if(HaveRoleID(roleID))
	{
		return ;
	}

	GuildMFRole gmfr;
	gmfr.roleID = roleID;
	gmfr.state = KKSG::GUILD_MF_WAITING;

	gmfr.name = name;
	gmfr.pkpoint = pkpoint;
	gmfr.profession = profession;

	m_roles.push_back(gmfr);
}

void GuildMFHalfRoom::DelRoleID(UINT64 roleID)
{
	for(auto it = m_roles.begin(); it != m_roles.end(); ++it)
	{
		if(it->roleID == roleID)
		{
			m_roles.erase(it);
			break;
		}
	}
}

void GuildMFHalfRoom::TestPrint()
{
	SSDebug << "GuildMFHalfRoom: " << END;
	SSDebug << "guildid = " << GetGuildID() << END;
	for(size_t i = 0; i < m_roles.size(); ++i)
	{
		SSDebug <<" roleid = " << m_roles[i].roleID <<" state = " << m_roles[i].state << END;
	}
}

UINT64 GuildMFHalfRoom::GetNextFightRoleID(Scene* pScene)
{
	for(UINT32 i = 0; i < m_roles.size(); ++i)
	{
		//判断是否可以 如果可以 返回ID
		if(KKSG::GUILD_MF_WAITING == m_roles[i].state)
		{
			Role* pRole = RoleManager::Instance()->FindByRoleID(m_roles[i].roleID);
			if(NULL == pRole || pRole->GetCurrScene() != pScene)
			{
				m_roles[i].state = KKSG::GUILD_MF_ERR; 
				continue;
			}

			m_roles[i].state = KKSG::GUILD_MF_FIGHTING;
			return m_roles[i].roleID;
		}
	}
	return 0;
}

void GuildMFHalfRoom::FillHalfRoles(KKSG::GmfHalfRoles& data)
{
	int count = (int)(m_roles.size());
	for(int i = 0; i < count; ++i)
	{
		const GuildMFRole& GRole = m_roles[i];
		KKSG::GmfRole* fr = data.add_roles();
		fr->set_index(i+1);
		fr->set_roleid(GRole.roleID);
		fr->set_state(GRole.state);
		fr->set_rolename(GRole.name);
		fr->set_pkpoint(GRole.pkpoint);
		fr->set_profession(GRole.profession);
		auto pGuild = GuildSimpleMgr::Instance()->GetGuildSimple(m_guildID);
		if(pGuild)
		{
			fr->set_guildpos((int)(pGuild->GetPosition(GRole.roleID)));
		}
		else
		{
			fr->set_guildpos(GPOS_MAX);
		}
		SSDebug << __FUNCTION__ << " roleid = " << fr->roleid() << " state = " << fr->state() << END;
	}

}

GuildMFRoom::GuildMFRoom()
{
	m_createTime = time_t(0);
	m_UID = 0;
	m_oWarType = 0;
	m_oBattleId = 0;
}

void GuildMFRoom::SaveData(KKSG::GmfRoom& data)
{
	data.set_uid(m_UID);
	data.set_wartype(m_oWarType);
	data.set_battleid(m_oBattleId);
	data.set_guildid11(m_halfRome11.GetGuildID());
	data.set_perstigerank11(m_halfRome11.GetPretigeRank());
	data.set_guildid22(m_halfRome22.GetGuildID());
	data.set_perstigerank22(m_halfRome22.GetPretigeRank());
	data.set_createtime((UINT32)m_createTime);
}

void GuildMFRoom::LoadData(const KKSG::GmfRoom& data)
{
	m_UID = data.uid();
	m_oWarType = data.wartype();
	m_oBattleId = data.battleid();
	m_halfRome11.SetGuildID(data.guildid11());
	m_halfRome11.SetPretigeRank(data.perstigerank11());
	m_halfRome22.SetGuildID(data.guildid22());
	m_halfRome22.SetPretigeRank(data.perstigerank22());
	m_createTime = data.createtime();
}

GMFHalfRoomType GuildMFRoom::GetHalfRoomTypeByRoleID(UINT64 roleid)
{
	if(m_halfRome11.HaveRoleID(roleid))
	{
		return GMF_HRT_ONE;
	}
	else if(m_halfRome22.HaveRoleID(roleid))
	{
		return GMF_HRT_TWO;
	}
	return GMF_HRT_NONE;
}

GMFHalfRoomType GuildMFRoom::GetHalfRoomTypeByGuildID(UINT64 guildid)
{
	if(m_halfRome11.GetGuildID() == guildid)
	{
		return GMF_HRT_ONE;
	}
	else if(m_halfRome22.GetGuildID() == guildid)
	{
		return GMF_HRT_TWO;
	}
	return GMF_HRT_NONE;
}
	
UINT64 GuildMFRoom::GetGuildIDByRoleID(UINT64 roleid)
{
	if(m_halfRome11.HaveRoleID(roleid))
	{
		return m_halfRome11.GetGuildID();
	}
	else if(m_halfRome22.HaveRoleID(roleid))
	{
		return m_halfRome22.GetGuildID();
	}
	return 0;
}

KKSG::GuildMatchFightState  GuildMFRoom::GetRoleMFState(UINT64 roleID)
{
	GuildMFRole* mfRole = GetGuildMFRole(roleID);
	if(mfRole)
	{
		return mfRole->state;
	}
	return KKSG::GUILD_MF_NONE;
}

void GuildMFRoom::SetRoleMFState(UINT64 roleID, KKSG::GuildMatchFightState state)
{
	GuildMFRole* mfRole = GetGuildMFRole(roleID);
	if(mfRole)
	{
		mfRole->state = state;
	}
}

UINT64 GuildMFRoom::GetNextFightRoleID(Scene* pScene, GMFHalfRoomType type)
{
	if(GMF_HRT_ONE == type)
	{
		return m_halfRome11.GetNextFightRoleID(pScene);
	}
	else if(GMF_HRT_TWO == type)
	{
		return m_halfRome22.GetNextFightRoleID(pScene);
	}
	return 0;
}

void GuildMFRoom::AddRoleID(UINT64 guildid, UINT64 newRoleID, const std::string& name, UINT32 pkpoint, int profession)
{
	if(m_halfRome11.GetGuildID() == guildid)
	{
		m_halfRome11.AddRoleID(newRoleID, name, pkpoint, profession);
	}
	else if(m_halfRome22.GetGuildID() == guildid)
	{
		m_halfRome22.AddRoleID(newRoleID, name, pkpoint, profession);
	}
}

void GuildMFRoom::DelRoleID(UINT64 roleID)
{
	if(m_halfRome11.HaveRoleID(roleID))
	{
		m_halfRome11.DelRoleID(roleID);
	}
	else if(m_halfRome22.HaveRoleID(roleID))
	{
		m_halfRome22.DelRoleID(roleID);
	}
}

bool GuildMFRoom::HaveRoleID(UINT64 roleID)
{
	if(m_halfRome11.HaveRoleID(roleID) || m_halfRome22.HaveRoleID(roleID))
	{
		return true;
	}
	return false;
}

void GuildMFRoom::FillBaseData(KKSG::GmfRoleDatas& data)
{
	m_halfRome11.FillHalfRoles(*data.mutable_halfrole11());
	m_halfRome22.FillHalfRoles(*data.mutable_halfrole22());
}

UINT64 GuildMFRoom::GetGuildID(GMFHalfRoomType type)
{
	if(GMF_HRT_ONE == type)
	{
		return m_halfRome11.GetGuildID();
	}
	else if(GMF_HRT_TWO == type)
	{
		return m_halfRome22.GetGuildID();
	}
	return 0;
}

UINT32 GuildMFRoom::GetPrestigeRank(GMFHalfRoomType type)
{
	if(GMF_HRT_ONE == type)
	{
		return m_halfRome11.GetPretigeRank();
	}
	else if(GMF_HRT_TWO == type)
	{
		return m_halfRome22.GetPretigeRank();
	}
	return (UINT32)(-1);
}

UINT32 GuildMFRoom::GetRoleCount(GMFHalfRoomType type)
{
	if(GMF_HRT_ONE == type)
	{
		return m_halfRome11.GetRoleCount();
	}
	else if(GMF_HRT_TWO == type)
	{
		return m_halfRome22.GetRoleCount();
	}
	return UINT32(-1);
}

void GuildMFRoom::TestPrint()
{
	SSDebug << " GuildMFRoom : " << END;
	SSDebug << " wartype = " << m_oWarType << END;
	SSDebug << " battleid = " << m_oBattleId << END;
	m_halfRome11.TestPrint();
	m_halfRome22.TestPrint();
}

GuildMFRole* GuildMFRoom::GetGuildMFRole(UINT64 roleID)
{
	GuildMFRole* GMFRole = m_halfRome11.GetGuildMFRoleByRoleID(roleID);
	if(NULL == GMFRole)
	{
		GMFRole = m_halfRome22.GetGuildMFRoleByRoleID(roleID);
	}
	return GMFRole;
}

GMFHalfRoomType GuildMFRoom::GetOpHalfRoomType(GMFHalfRoomType type)
{
	if(GMF_HRT_ONE == type)
	{
		return GMF_HRT_TWO;
	}
	if(GMF_HRT_TWO == type)
	{
		return GMF_HRT_ONE;
	}
	return GMF_HRT_NONE;
}

void GuildMFRoom::GetAllRoles(std::vector<UINT64>& allroles)
{
	const auto& half11 = m_halfRome11.GetRoles();
	for(size_t i = 0; i < half11.size(); ++i)
	{
		allroles.push_back(half11[i].roleID);
	}
	const auto& half22 = m_halfRome22.GetRoles();
	for(size_t i = 0; i < half22.size(); ++i)
	{
		allroles.push_back(half22[i].roleID);
	}
}

void GuildMFRoom::Clear()
{
	m_halfRome11.Clear();
	m_halfRome22.Clear();
}
