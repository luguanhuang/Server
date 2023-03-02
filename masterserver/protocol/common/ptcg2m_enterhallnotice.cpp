#include "pch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "common/ptcg2m_enterhallnotice.h"

// generate by ProtoGen at date: 2016/11/28 14:32:10

void PtcG2M_EnterHallNotice::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(m_Data.roleid());
	if (NULL != role)
	{
		role->OnEnterHall();
	}
}
