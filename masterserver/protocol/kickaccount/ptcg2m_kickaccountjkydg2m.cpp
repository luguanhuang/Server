#include "pch.h"
#include "kickaccount/ptcg2m_kickaccountjkydg2m.h"
#include "account/accountkick.h"
#include "account/punishmgr.h"

// generate by ProtoGen at date: 2017/10/12 15:53:21

void PtcG2M_KickAccountJkydG2M::Process(UINT32 dwConnID)
{
    CAccountKicker::Instance()->KickAccount(m_Data.roleid(), m_Data.kt(), NULL);
	CPunishMgr::Instance()->DoPunishUser(m_Data.roleid(), m_Data.odata());
}
