#include "pch.h"
#include "platfriend/ptcs2m_s2minvfightmsgntf.h"
#include "invfight/invfightcrossmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "invfight/invfightmgr.h"

// generate by ProtoGen at date: 2017/4/14 19:31:31

void PtcS2M_S2MInvFightMsgNtf::Process(UINT32 dwConnID)
{
	if (m_Data.invmsgs_size() == 0)
	{
		return;
	}
	for (int i = 0; i < m_Data.invmsgs_size(); ++i)
	{
		auto& ref = m_Data.invmsgs(i);
		InvFightCrossMgr::Instance()->AddToCrossFight(ref);
	}

	CRole* role = CRoleManager::Instance()->GetByRoleID(m_Data.invmsgs(0).toid());

	if(role)
	{
		InvFightMgr::Instance()->OnLogin(role);
	}
}
