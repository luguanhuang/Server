#include "pch.h"
#include "roleview.h"
#include "role.h"
#include "scene/scene.h"
#include "table/globalconfig.h"

#define FRIEND_VALUE GetGlobalConfig().ViewGridFriendDegree

RoleView::RoleView(Role* role)
:m_role(role)
,m_isneedpre(false)
,m_bindrole(0)
{
	
}

RoleView::~RoleView()
{

}

void RoleView::Add(UINT64 roleid, ViewType type, UINT32 value)
{
	if (m_views.size() > 300)
	{
		SSWarn<<"view too many, roleid:"<<m_role->GetID()<<" size:"<<m_views.size()<<END;
		return;
	}

	auto i = m_role2viewunit.find(roleid);
	if (i == m_role2viewunit.end())
	{
		ViewUnit view;
		view.roleid = roleid;
		view.mask |= (1 << type);
		view.value = value;
		if (view.value > FRIEND_VALUE)
		{
			view.mask |= (1 << View_FriendValue);
		}

		std::pair<std::set<ViewUnit>::iterator, bool> ret = m_views.insert(view);
		if (ret.second)
		{
			m_role2viewunit[roleid] = ret.first;
		}
	}
	else
	{
		ViewUnit view = *i->second;
		view.mask |= (1 << type);
		view.value = value;
		if (view.value > FRIEND_VALUE)
		{
			view.mask |= (1 << View_FriendValue);
		}
	
		m_views.erase(i->second);

		std::pair<std::set<ViewUnit>::iterator, bool> ret = m_views.insert(view);
		if (ret.second)
		{
			m_role2viewunit[roleid] = ret.first;
		}
	}
}

void RoleView::Del(UINT64 roleid, ViewType type)
{
	auto i = m_role2viewunit.find(roleid);
	if (i != m_role2viewunit.end())
	{
		ViewUnit view = *i->second;
		view.mask ^= (1 << type);

		m_views.erase(i->second);
		m_role2viewunit.erase(i);
		
		if (0 != view.mask)
		{
			std::pair<std::set<ViewUnit>::iterator, bool> ret = m_views.insert(view);
			if (ret.second)
			{
				m_role2viewunit.insert(std::make_pair(roleid, ret.first));
			}
		}
	}
}
