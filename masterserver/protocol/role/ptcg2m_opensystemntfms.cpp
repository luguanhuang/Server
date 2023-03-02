#include "pch.h"
#include "role/ptcg2m_opensystemntfms.h"
#include "role/role.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2017/2/9 20:12:09

void PtcG2M_OpenSystemNtfMs::Process(UINT32 dwConnID)
{
	UINT64 roleID = m_Data.roleid();
	CRole* role = CRoleManager::Instance()->GetByRoleID(roleID);
	if (!role)
	{
		return;
	}
	std::vector<UINT32> openids;
	for (int i = 0; i < m_Data.systems().sysids_size(); ++i)
	{
		role->DoOpenSystem(m_Data.systems().sysids(i));
	}
}
