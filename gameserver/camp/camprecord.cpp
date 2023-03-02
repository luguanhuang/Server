#include "pch.h"
#include "camprecord.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"

CCampRecord::CCampRecord(Role* role)
{
	m_pRole = role;
	m_lastCampID = 0;
}

CCampRecord::~CCampRecord()
{
}

bool CCampRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_camprolerecord())
	{
		const KKSG::CampRoleRecord* record = &poRoleAllInfo->camprolerecord();
		m_lastCampID = record->lastcampid();
	}
	return true;
}

void CCampRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_switch.TestAndReset())
	{
		KKSG::CampRoleRecord* record = poRoleAllInfo->mutable_camprolerecord();
		record->Clear();
		record->set_lastcampid(m_lastCampID);

		roChanged.insert(record);
	}
}

