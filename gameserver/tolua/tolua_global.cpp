#include "pch.h"
#include "tolua_global.h"
#include "unit/rolemanager.h"


Role* GetRoleByID(UINT64 qwID)
{
	return RoleManager::Instance()->FindByRoleID(qwID);
}