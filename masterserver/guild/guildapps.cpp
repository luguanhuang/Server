#include "pch.h"
#include <time.h>
#include "role/role.h"
#include "role/rolemanager.h"
#include "guildapps.h"
#include "guildmgr.h"
#include "guild.h"
#include "foreach.h"

GuildApplication::GuildApplication(Guild *pGuild) : m_pGuild(pGuild)
{

}

GuildApplication::~GuildApplication()
{

}

void GuildApplication::Add(UINT64 roleId)
{
	auto it = FindAppIterator(roleId);
	if (it == m_appRoles.end())
	{
		GuildApplicationInfo app;
		app.qwRoleID = roleId;
		app.appTime = time(NULL);
		app.deleted = false;
		m_appRoles.push_back(app);

		m_ismodified.Set();
	}
}

bool GuildApplication::Accept(UINT64 qwRole)
{
	auto it = FindAppIterator(qwRole);
	if (it != m_appRoles.end())
	{
		if (CGuildMgr::Instance()->GetRoleGuild(qwRole) != INVALID_GUILDID)
		{
			return false;
		}

		if (m_pGuild->IsFull())
		{
			return false;
		}

		CRole *pAddRole = CRoleManager::Instance()->GetByRoleID(qwRole);
		if (pAddRole)
		{
			m_pGuild->AddMember(pAddRole->GetID(), GPOS_MEMBER);
			CGuildMgr::Instance()->HintNotify(pAddRole);
		}
		else
		{
			m_pGuild->AddMember(qwRole, GPOS_MEMBER);
		}

		m_appRoles.erase(it);

		m_ismodified.Set();
		return true;
	}
	return false;
}

void GuildApplication::Reject(UINT64 qwRole)
{
	auto it = FindAppIterator(qwRole);
	if (it != m_appRoles.end())
	{
		m_appRoles.erase(it);
		m_ismodified.Set();
	}
}

void GuildApplication::RejectAll()
{
	m_appRoles.clear();
	m_ismodified.Set();
}

std::list<GuildApplicationInfo>::iterator GuildApplication::FindAppIterator(UINT64 qwRoleID)
{
	foreach(i in m_appRoles)
	{
		if (i->qwRoleID == qwRoleID)
		{
			return i;
		}
	}

	return m_appRoles.end();
}

void GuildApplication::ClearDeleted()
{
	for (auto i = m_appRoles.begin(); i != m_appRoles.end(); )
	{
		if (i->deleted)
		{
			m_appRoles.erase(i++);
		}
		else
		{
			++i;
		}
	}
}

void GuildApplication::Foreach(GAppCB cb, void *arg)
{
	for (auto i = m_appRoles.begin(); i != m_appRoles.end(); ++i)
	{
		if (!i->deleted)
		{
			cb(&*i, arg);
		}
	}
}

bool GuildApplication::Load(const KKSG::GuildAllDBData & guilddata)
{
	KKSG::GStoreApplications apps;
	if (!apps.ParseFromString(guilddata.application()))
	{
		return false;
	}

	for (int i = 0; i < apps.apps_size(); ++i)
	{
		const auto &a = apps.apps(i);
		GuildApplicationInfo info;
		info.qwRoleID = a.roleid();
		info.appTime = a.time();
		info.deleted = false;
		m_appRoles.push_back(info);
	}
	return true;
}

const std::string GuildApplication::ToBinary()
{
	KKSG::GStoreApplications apps;
	foreach(i in m_appRoles)
	{
		if (i->deleted)
		{
			continue;
		}

		auto *pA = apps.add_apps();
		pA->set_roleid(i->qwRoleID);
		pA->set_time((UINT32)i->appTime);
	}

	return apps.SerializeAsString();
}

void GuildApplication::Save(KKSG::GuildAllDBData & guilddata)
{
	guilddata.mutable_application()->assign(ToBinary());
}

bool GuildApplication::IsChanged()
{
	return m_ismodified.TestAndReset();
}
