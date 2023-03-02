#include "pch.h"
#include "rolefindercondition.h"
#include "role.h"

RoleSceneFindCondition::RoleSceneFindCondition(std::vector<UINT32>& vec):m_sceneIDVec(vec)
{
}

bool RoleSceneFindCondition::operator()(CRole* role)
{
	if (!role)
	{
		return true;
	}
	for (auto iter = m_sceneIDVec.begin(); iter != m_sceneIDVec.end(); ++iter)
	{
		if (role->GetMapID() == *iter)
		{
			m_foundRoles.push_back(role);
			break;
		}
	}

	return true;
}
