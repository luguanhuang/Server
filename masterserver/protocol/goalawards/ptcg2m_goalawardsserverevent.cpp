#include "pch.h"
#include "goalawards/ptcg2m_goalawardsserverevent.h"
#include "goalawards/goalawardsmgr.h"

// generate by ProtoGen at date: 2017/9/6 10:48:24

void PtcG2M_GoalAwardsServerEvent::Process(UINT32 dwConnID)
{
    GoalAwardsMgr::Instance()->FreshValue(m_Data.roleid(), m_Data.goalawardsid(), m_Data.num(), m_Data.gkid());
}
