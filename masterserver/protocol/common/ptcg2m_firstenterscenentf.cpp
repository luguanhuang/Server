#include "pch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "common/ptcg2m_firstenterscenentf.h"

// generate by ProtoGen at date: 2016/8/4 23:18:29

void PtcG2M_FirstEnterSceneNtf::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(m_Data.roleid());
	if (NULL != role)
	{
		role->OnFirstEnterScene();
	}
}
