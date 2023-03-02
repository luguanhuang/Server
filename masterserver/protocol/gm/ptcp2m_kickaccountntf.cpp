#include "pch.h"
#include "gm/ptcp2m_kickaccountntf.h"
#include "role/rolesummarymgr.h"
#include "account/accountkick.h"
#include "account/punishmgr.h"

// generate by ProtoGen at date: 2017/4/2 16:16:19

void PtcP2M_KickAccountNtf::Process(UINT32 dwConnID)
{
	std::vector<UINT64> m_vRoleList;
	CRoleSummaryMgr::Instance()->GetRoleIdByAccount(m_Data.account(), m_vRoleList);
	for (UINT32 i = 0; i < m_vRoleList.size(); i ++)
	{
		CPunishMgr::Instance()->DoPunishTemp(m_vRoleList[i]);
		CAccountKicker::Instance()->KickAccount(m_vRoleList[i], KKSG::KICK_NORMAL, NULL);
	}
}
