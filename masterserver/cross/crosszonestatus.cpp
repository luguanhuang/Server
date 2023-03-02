#include "pch.h"
#include "crosszonestatus.h"
#include "pvp/pvpmgr.h"
#include "leaguebattle/leaguebattlematchmgr.h"
#include "herobattle/herobattlemgr.h"
#include "match/matchmgr.h"

INSTANCE_SINGLETON(CrossZoneStatus)

CrossZoneStatus::CrossZoneStatus()
:m_worldOk(false)
,m_crossGsNum(0)
{

}

CrossZoneStatus::~CrossZoneStatus()
{

}


bool CrossZoneStatus::Init()
{
	return true;
}

void CrossZoneStatus::Uninit()
{

}

bool CrossZoneStatus::IsCrossZoneUsable()
{
	if (!m_worldOk)
	{
		return false;
	}
	return m_crossGsNum > 0;
}

void CrossZoneStatus::SetWorldStatus(bool status)
{
	bool old = IsCrossZoneUsable();
	m_worldOk = status;
	bool newStatus = IsCrossZoneUsable();

	if (newStatus == false && old == true)
	{
		OnCrossZoneUnusable();
	}
}

void CrossZoneStatus::SetCrossGsNum(UINT32 num)
{
	bool old = IsCrossZoneUsable();
	m_crossGsNum = num;
	bool newStatus = IsCrossZoneUsable();

	if (newStatus == false && old == true)
	{
		OnCrossZoneUnusable();
	}
}

void CrossZoneStatus::OnCrossZoneUnusable()
{
	SSWarn << "Cross zone unusable" << END;
	//PvpMgr::Instance()->OnCrossZoneUnusable();
	LeagueBattleMatchMgr::Instance()->OnCrossZoneUnusable();
	//HeroBattleMgr::Instance()->OnCrossZoneUnusable();
	MatchMgr::Instance()->OnCrossZoneUnusable();
}
