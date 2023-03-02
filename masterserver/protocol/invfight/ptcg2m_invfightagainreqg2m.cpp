#include "pch.h"
#include "invfight/ptcg2m_invfightagainreqg2m.h"
#include "role/rolemanager.h"
#include "invfight/invfightmgr.h"

// generate by ProtoGen at date: 2017/8/30 15:40:13

void PtcG2M_InvfightAgainReqG2M::Process(UINT32 dwConnID)
{
	if(m_Data.data().createinfos_size() == 2)
	{
		UINT64 roleID1 = m_Data.data().createinfos(0).roleid();
		UINT64 roleID2 = m_Data.data().createinfos(1).roleid();
		if(!CRoleManager::Instance()->GetByRoleID(roleID1))	
		{
			SSWarn << " find role failed roleid = " << roleID1 << END;
			return;
		}
		if(!CRoleManager::Instance()->GetByRoleID(roleID2))
		{
			SSWarn << " find role failed roleid = " << roleID2 << END;
			return;
		}
		InvFightMgr::Instance()->CreateSceneBegin(roleID1, roleID2);
	}
}
