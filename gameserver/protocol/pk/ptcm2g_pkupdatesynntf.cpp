#include "pch.h"
#include "pk/ptcm2g_pkupdatesynntf.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/pkrecord.h"

// generate by ProtoGen at date: 2017/6/13 11:39:12

void PtcM2G_PkUpdateSynNtf::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
	if(NULL == pRole)
	{
		SSError << " find role failed roleid = " << m_Data.roleid() << END;
		return;
	}
	pRole->Get<CPkRecord>()->Update();
	pRole->Get<CPkRecord>()->PkWeekTimeSyn(m_Data.ctime());
}