#include "pch.h"
#include <time.h>
#include "payrecord.h"
#include "pb/project.pb.h"
#include "unit/role.h"

PayRecord::PayRecord(Role *pRole) : m_pRole(pRole)
{
}

PayRecord::~PayRecord()
{

}

bool PayRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	return true;
}

void PayRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
}
