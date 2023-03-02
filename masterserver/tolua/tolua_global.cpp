#include "pch.h"
#include "tolua_global.h"
#include "role/rolemanager.h"


CRole* GetRoleByID(UINT64 qwID)
{
	return CRoleManager::Instance()->GetByRoleID(qwID);
}