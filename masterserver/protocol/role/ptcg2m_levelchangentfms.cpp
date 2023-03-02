#include "pch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/ptcg2m_levelchangentfms.h"

// generate by ProtoGen at date: 2017/1/18 20:23:51

void PtcG2M_LevelChangeNtfMs::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(m_Data.roleid());
	if (NULL != role)
	{
		role->UpdateSelfServer(m_Data.level());
	}
}
