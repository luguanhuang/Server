#include "pch.h"
#include "vshandler.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scenegmf.h"
#include "scenevsbase.h"
#include "unit/dummyrolemanager.h"
#include "unit/dummyrole.h"
#include "unit/combatstatistics.h"
#include "scene.h"
#include "scenecastle.h"
#include "unit/unit.h"

VsLoadMgr::VsLoadMgr()
{
	m_lastUpTime = time(NULL);
}

void VsLoadMgr::AddRole(UINT64 roleid)
{
	m_load_roles[roleid] = time(NULL);
}

void VsLoadMgr::DelRole(UINT64 roleid)
{
	m_load_roles.erase(roleid);
}

bool VsLoadMgr::HaveRole(UINT64 roleid)
{
	return (m_load_roles.find(roleid) != m_load_roles.end());
}

int VsLoadMgr::LoadedTime(UINT64 roleid)
{
	time_t now = time(NULL);
	auto it = m_load_roles.find(roleid);
	if(it == m_load_roles.end())
	{
		return -1;
	}
	if(now >= it->second)
	{
		return (int)(now - it->second);
	}
	return 0;
}

void VsLoadMgr::Update()
{
	
}

void VsLoadMgr::CheckSwitchWatcher(UINT32 SwitchCountMax, SceneGWB* gwb)
{
	if(NULL == gwb)
	{
		return ;
	}
	time_t now = time(NULL);
	if(m_lastUpTime + 1 > now)
	{
		return;
	}

	m_lastUpTime = now + 1;

	UINT32 changecount = 0;
	std::vector<UINT64> wait_switch;
	for(auto it = m_load_roles.begin(); it != m_load_roles.end() && changecount < SwitchCountMax;)
	{
		UINT64 roleid = it->first;
		if(!gwb->IsFightRole(roleid))
		{
			++changecount;
			wait_switch.push_back(roleid);
			m_load_roles.erase(it++);//假设一定成功
		}
		else
		{
			it++;
		}
	}

	for(size_t i = 0; i < wait_switch.size(); ++i)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(wait_switch[i]);
		if(pRole)
		{
			gwb->OnBeforeChangeWather(pRole);
			gwb->ChangeWatcherState(pRole, true);
		}
	}
}

VsRoleDataMgr::VsRoleDataMgr(Scene* pScene)
{
	m_pScene = pScene;
}

VsRoleDataMgr::~VsRoleDataMgr()
{

}

void VsRoleDataMgr::OnUnitCombatStop(UINT64 unitID/* = 0*/)
{
	for(auto it = m_roledata.begin(); it != m_roledata.end(); ++it)
	{
		auto& data = (it->second);
		if(data.Stoped())
		{
			continue;
		}
		if(0 != unitID && (it->first) != unitID)
		{
			continue;
		}
		Unit* pUnit = Unit::FindUnit(it->first);
		if(NULL == pUnit)
		{
			continue;
		}
		if(pUnit->IsRole())
		{
			data.SetStatistics(((Role*)(pUnit))->Statistics());
		}
		else if(pUnit->IsDummyRole())
		{
			data.SetStatistics(((DummyRole*)(pUnit))->Statistics());
		}
	}
}

UINT32 VsRoleDataMgr::GetGroupKillCount(UINT32 group)
{
	UINT32 count = 0;
	for(auto it = m_roledata.begin(); it != m_roledata.end(); ++it)
	{
		if((it->second).GetGroup() == group)
		{
			count += (it->second).Statistics(m_pScene).GetKillCount();
		}
	}
	return count;
}

double VsRoleDataMgr::GetGroupTotalDamage(UINT32 group)
{
	double damage = 0.0;
	for(auto it = m_roledata.begin(); it != m_roledata.end(); ++it)
	{
		if((it->second).GetGroup() == group)
		{
			damage += (it->second).Statistics(m_pScene).GetTotalDamage();
		}
	}
	return damage;
}

void VsRoleDataMgr::AddUnit(UINT64 unitID, UINT32 group)
{
	VsRoleData data(unitID, group);
	if(m_roledata.find(unitID) != m_roledata.end())
	{
		SSError << " add repeat unitID = " << unitID << END;
		return ;
	}
	m_roledata.insert(std::make_pair(unitID, data));
}

UINT32 VsRoleDataMgr::GetUnitKillCount(UINT64 unitID)
{
	auto it = m_roledata.find(unitID);
	if(it == m_roledata.end())
	{
		return 0;
	}
	return (it->second).Statistics(m_pScene).GetKillCount();
}

UINT32 VsRoleDataMgr::GetUnitDeadCount(UINT64 unitID)
{
	auto it = m_roledata.find(unitID);
	if(it == m_roledata.end())
	{
		return 0;
	}
	return (it->second).Statistics(m_pScene).GetDeathCount();
}

double VsRoleDataMgr::GetUnitTotalDamage(UINT64 unitID)
{
	auto it = m_roledata.find(unitID);
	if(it == m_roledata.end())
	{
		return 0.0;
	}
	return (it->second).Statistics(m_pScene).GetTotalDamage();
}

UINT64 VsRoleDataMgr::GetMVPID(UINT32 group)
{
	UINT64 mvpID = 0;
	UINT32 mvpkill = 0;
	UINT32 mvpdead = 0;
	double mvpdamage = 0.0;

	for(auto it = m_roledata.begin(); it != m_roledata.end(); ++it)
	{
		auto& data = (it->second);
		if(data.GetGroup() != group)
		{
			continue;
		}
		UINT32 tmpkill = data.Statistics(m_pScene).GetKillCount();
		UINT32 tmpdead = data.Statistics(m_pScene).GetDeathCount();
		double tmpdamage = data.Statistics(m_pScene).GetTotalDamage();	
		if((0 == mvpID)||
			(tmpkill > mvpkill)||
			(tmpkill == mvpkill && tmpdead < mvpdead) ||
			(tmpkill == mvpkill && tmpdead == mvpdead && tmpdamage > mvpdamage))
		{
			mvpID = data.GetUnitID();
			mvpkill = tmpkill;
			mvpdead = tmpdead;
			mvpdamage = tmpdamage;
		}
	}

	return mvpID;
}

void VsRoleDataMgr::Clear()
{
	m_roledata.clear();
}

const CombatStatistics* VsRoleDataMgr::GetStatistics(UINT64 roleId, Scene* pScene)
{
	auto it = m_roledata.find(roleId);
	if (it == m_roledata.end())
	{
		return NULL;
	}
	return &(it->second.Statistics(pScene));
}

const CombatStatistics& VsRoleData::Statistics(Scene* pScene)
{
	if(Stoped())
	{
		return m_combatStatistics;
	}
	if(NULL == pScene)
	{
		return m_combatStatistics;
	}
	Unit* pUnit = pScene->FindSceneUnit(m_unitid);
	if(pUnit)
	{
		if(pUnit->IsRole())
		{
			return ((Role*)(pUnit))->Statistics();
		}
		else if(pUnit->IsDummyRole())
		{
			return ((DummyRole*)(pUnit))->Statistics();
		}
	}
	//SSError << __FUNCTION__ << "find unit failed unitid = " << m_unitid << END;
	return m_combatStatistics;
}

VsJvDian::VsJvDian(int type, const Vector3& sPos, float tr, SceneCastle* castle)
{
	m_type = type;
	m_pos = sPos;
	m_r = tr;
	m_pCastle = castle;

	m_beginTime = 0;
	m_roleID = 0;
	m_group = (UINT32)(-1);
}

VsJvDian::~VsJvDian()
{
	
}

void VsJvDian::Update()
{
	if(m_roleID)//有人正在占领
	{
		if(CheckInterrupt())
		{
			m_pCastle->OnOccupyState(m_roleID, m_type, KKSG::GCFZL_BREAK);

			m_roleID = 0;
			m_beginTime = 0;
		}
		else if(CheckEnd())
		{
			m_pCastle->OnOccupyState(m_roleID, m_type, KKSG::GCFZL_END);

			UINT32 tmp = m_pCastle->GetGroup(m_roleID);
			m_group = tmp;//所属分组

			m_roleID = 0;
			m_beginTime = 0;
		}
	}
	else
	{
		UINT64 roleID = CheckBegin();
		if(roleID)
		{
			m_roleID = roleID;
			m_beginTime = UINT32(time(NULL));

			m_pCastle->OnOccupyState(m_roleID, m_type, KKSG::GCFZL_BEGIN);
		}
	}
}

UINT64 VsJvDian::CheckBegin()
{
	std::vector<UINT64> roles;
	GetRolesInCircle(roles);

	if(roles.empty())
	{
		return 0;
	}

	for(size_t i = 1; i < roles.size(); ++i)
	{
		if(m_pCastle->GetGroup(roles[i-1]) != m_pCastle->GetGroup(roles[i]))
		{
			return 0;
		}
	}

	if(m_pCastle->GetGroup(roles[0]) == m_group)
	{
		return 0;
	}

	return roles[0];
}

bool VsJvDian::CheckInterrupt()
{
	Role* pRole = m_pCastle->GetScene()->FindSceneRole(m_roleID);
	if(NULL == pRole)
	{
		return true;
	}
	if(pRole->IsDead())
	{
		return true;
	}

	if(!InCircle(pRole->GetXObject()->GetPosition()))
	{
		return true;
	}

	std::vector<UINT64> roles;
	GetRolesInCircle(roles);
	if(roles.empty())//已出圈
	{
		return true;
	}

	for(size_t i = 0; i < roles.size(); ++i)
	{
		if(m_pCastle->GetGroup(m_roleID) != m_pCastle->GetGroup(roles[i]))
		{
			return true;
		}
	}
	return false;
}

bool VsJvDian::CheckEnd()
{
	UINT32 ConfigTime = GetGlobalConfig().GuildCastleJvDianOccupyTime;
	if(m_beginTime + ConfigTime <= time(NULL))
	{
		return true;
	}
	return false;
}

bool VsJvDian::InCircle(const Vector3& tmpPos)
{
	if(Vector3::Distance(tmpPos, m_pos) <= m_r)
	{
		return true;
	}
	return false;
}

void VsJvDian::GetRolesInCircle(std::vector<UINT64>& roles)
{
	const auto& allRoles = m_pCastle->GetScene()->GetAllRoles();	
	for(auto it = allRoles.begin(); it != allRoles.end(); ++it)
	{
		Role* tmp = (*it);
		if(!tmp->IsDead() && InCircle(tmp->GetXObject()->GetPosition()))
		{
			roles.push_back(tmp->GetID());
		}
	}
}
