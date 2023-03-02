#include "pch.h"
#include "roleopdelay.h"

bool RoleOpDelay::HaveRole(UINT64 roleID)
{
	return (m_roles.find(roleID) != m_roles.end());
}

void RoleOpDelay::AddRole(UINT64 roleID, UINT32 delayEndTime)
{
	m_roles[roleID] = delayEndTime;
}

void RoleOpDelay::DelRole(UINT64 roleID)
{
	m_roles.erase(roleID);
}

void RoleOpDelay::Update(UINT32 now)
{
	for(auto it = m_roles.begin(); it != m_roles.end();)
	{
		if(it->second <= now)
		{
			m_roles.erase(it++);
		}
		else
		{
			it++;
		}
	}
}
