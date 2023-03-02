#include "pch.h"
#include "designation/ptcd2g_d2gdesignationntf.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/designationMgr.h"

// generate by ProtoGen at date: 2016/7/29 0:53:15

void PtcD2G_D2GDesignationNtf::Process(UINT32 dwConnID)
{
	///>SSInfo << "Handle Designation Notify From DBServer, RoleID:" << m_Data.roleid() << ", DesignationCount:" << m_Data.datalist_size() << END;
	Role* pRole = RoleManager::Instance()->FindByRoleID( m_Data.roleid() );
	if( NULL == pRole )
	{
		SSError << "Cannot Find Role, RoleID:" << m_Data.roleid() << END;
		return; 
	}
	for( INT32 i=0; i<m_Data.datalist_size(); i++ )
	{
		UINT32 did = (UINT32)m_Data.datalist(i).key();
		UINT32 time = (UINT32)m_Data.datalist(i).value();
		DesignationMgr::Instance()->UpdateDesignation(pRole, did, time);
	}
}
