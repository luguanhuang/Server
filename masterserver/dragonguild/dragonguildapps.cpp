#include "pch.h"
#include <time.h>
#include "role/role.h"
#include "role/rolemanager.h"
#include "dragonguildapps.h"
#include "dragonguildmgr.h"
#include "dragonguilddef.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "foreach.h"

DragonGuildApplication::DragonGuildApplication(DragonGuild* pDGuild):DragonGuildBase(pDGuild)
{

}

void DragonGuildApplication::Add(UINT64 roleId)
{
	auto it = FindAppIterator(roleId);
	if (it == m_appRoles.end())
	{
		DragonGuildApplicationInfo app;
		app.roleId = roleId;
		app.appTime = time(NULL);
		app.deleted = false;
		m_appRoles.push_back(app);

		SetDirty();
	}
}

KKSG::ErrorCode DragonGuildApplication::Accept(UINT64 qwRole)
{
	auto it = FindAppIterator(qwRole);
	if (it != m_appRoles.end())
	{
		if (DragonGuildMgr::Instance()->GetDragonGuildIdByRole(qwRole) != INVAILD_DRAGONGUILD)
		{
			return KKSG::ERR_DG_ALREADY_IN_DG;
		}

		if (m_owner->IsFull())
		{
			return KKSG::ERR_DG_FULL;
		}

		CRole *pAddRole = CRoleManager::Instance()->GetByRoleID(qwRole);
		if (pAddRole)
		{
			m_owner->AddMember(pAddRole->GetID(), DGPOS_MEMBER);
			//DragonGuildMgr::Instance()->HintNotify(pAddRole);
		}
		else
		{
			m_owner->AddMember(qwRole, DGPOS_MEMBER);
		}

		m_appRoles.erase(it);

		SetDirty();
		return KKSG::ERR_SUCCESS;
	}
	return KKSG::ERR_DG_NOT_IN_APPROVAL;
}

void DragonGuildApplication::Reject(UINT64 qwRole)
{
	auto it = FindAppIterator(qwRole);
	if (it != m_appRoles.end())
	{
		m_appRoles.erase(it);
		SetDirty();
	}
}

void DragonGuildApplication::RejectAll()
{
	m_appRoles.clear();
	SetDirty();
}

bool DragonGuildApplication::LoadFromDB(const KKSG::DragonGuildSaveData& data)
{
	KKSG::DragonGuildApps apps;
	if (!apps.ParseFromString(data.applications()))
	{
		return false;
	}

	for (int i=0; i < apps.apps_size(); ++ i)
	{
		const auto& a = apps.apps(i);
		DragonGuildApplicationInfo info;
		info.roleId = a.roleid();
		info.appTime = a.time();
		info.deleted = false;
		m_appRoles.push_back(info);
	}
	return true;
}

void DragonGuildApplication::SaveToDB(KKSG::DragonGuildSaveData& data)
{
	data.mutable_applications()->assign(ToBinary());
}

const std::string DragonGuildApplication::ToBinary()
{
	KKSG::DragonGuildApps apps;
	foreach(i in m_appRoles)
	{
		if (i->deleted)
		{
			continue;
		}

		auto *pA = apps.add_apps();
		pA->set_roleid(i->roleId);
		pA->set_time((UINT32)i->appTime);
	}

	return apps.SerializeAsString();
}

void DragonGuildApplication::ClearDeleted()
{
	bool isChange = false;
	for (auto i = m_appRoles.begin(); i != m_appRoles.end(); )
	{
		if (i->deleted)
		{
			m_appRoles.erase(i++);
			isChange = true;
		}
		else
		{
			++i;
		}
	}
	if (isChange) SetDirty();
}

std::list<DragonGuildApplicationInfo>::iterator DragonGuildApplication::FindAppIterator(UINT64 qwRoleID)
{
	foreach(i in m_appRoles)
	{
		if (i->roleId == qwRoleID)
		{
			return i;
		}
	}

	return m_appRoles.end();
}

void DragonGuildApplication::GetApplicationInfo(KKSG::FetchDGAppRes& res)
{
	foreach(i in m_appRoles)
	{
		if (DragonGuildMgr::Instance()->GetDragonGuildIdByRole(i->roleId) != INVAILD_DRAGONGUILD)
		{
			i->deleted = true;
			continue;
		}

		CRoleSummary *pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(i->roleId);
		if (NULL != pRoleSum)
		{
			res.add_roleid(pRoleSum->GetID());
			res.add_rolename(pRoleSum->GetName());
			res.add_time((UINT32)(GameTime::GetTime() - i->appTime));
			res.add_level(pRoleSum->GetLevel());
			res.add_ppt(pRoleSum->GetPowerPoint());
			res.add_profession((KKSG::RoleType)pRoleSum->GetProfession());
		}
		else
		{
			i->deleted = true;
		}
	}
}

bool DragonGuildApplication::IsApplyFull()
{
	return m_appRoles.size() >= GetGlobalConfig().DragonGuildApplyMax; 
}