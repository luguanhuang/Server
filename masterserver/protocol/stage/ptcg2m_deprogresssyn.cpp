#include "pch.h"
#include "stage/ptcg2m_deprogresssyn.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "table/DragonExpRankMgr.h"

// generate by ProtoGen at date: 2016/9/6 10:45:58

void PtcG2M_DEProgressSyn::Process(UINT32 dwConnID)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_Data.roleid());
	if(NULL == pRole)
	{
		return ;
	}
	DragonExpRankMgr::Instance()->SynDragonExpProgress(m_Data.roleid(), m_Data.prosyn().sceneid(), m_Data.prosyn().bossavghppercent(), m_Data.isbattleend());
}
