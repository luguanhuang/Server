#include "pch.h"
#include "tarjateammgr.h"
#include <math.h>
#include "role/role.h"
#include "role/rolemanager.h"
#include "table/globalconfig.h"
#include "team/ptcm2c_tarjabriefntf.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "team/teammember.h"
#include "util/gametime.h"




INSTANCE_SINGLETON(TarjaTeamMgr)

TarjaTeamMgr::TarjaTeamMgr()
{
	m_Handler = INVALID_HTIMER;
}

TarjaTeamMgr::~TarjaTeamMgr()
{
}


bool TarjaTeamMgr::Init()
{
	m_Handler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	return true;
}

void TarjaTeamMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_Handler);
}


void TarjaTeamMgr::SyncStatus(CRole *pRole)
{
	if (pRole == NULL)
		return;
	Team *pTeam =  TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
	if (pTeam!=NULL)
	{
		pTeam->NotifyAllMemberStateChanged();
	}
}


void TarjaTeamMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	time_t nTime = GameTime::GetTime();
	for (auto iter  = m_mapRole.begin();iter!=m_mapRole.end();)
	{
		if (nTime>iter->second)
		{
			PtcM2C_TarjaBriefNtf ntf;
			ntf.m_Data.set_time(0);
			CRole *pRole = CRoleManager::Instance()->GetByRoleID(iter->first);
			if (pRole!=NULL)
			{
				pRole->Send(ntf);
				SyncStatus(pRole);
			}
			m_mapRole.erase(iter++);
		}else
		{
			iter++;
		}
	}
}

void TarjaTeamMgr::SetTime(UINT64 nRoleID,UINT32 nTime)
{
	CRole *pRole = CRoleManager::Instance()->GetByRoleID(nRoleID);
	time_t nCur  = GameTime::GetTime();
	if ( nTime > nCur)
	{
		m_mapRole[nRoleID] = nTime;
		SyncStatus(pRole);
		if (pRole!=NULL)
		{
			PtcM2C_TarjaBriefNtf ntf;
			ntf.m_Data.set_time(nTime);
			pRole->Send(ntf);
		}
	}
}

void TarjaTeamMgr::OnLogin(CRole *pRole)
{
	if (pRole!=NULL)
	{
		auto iter = m_mapRole.find(pRole->GetID());
		if (iter!=m_mapRole.end())
		{
			PtcM2C_TarjaBriefNtf ntf;
			ntf.m_Data.set_time(iter->second);
			pRole->Send(ntf);
		}

	}
}


UINT32 TarjaTeamMgr::GetTime(UINT64 nRole)
{
	auto iter = m_mapRole.find(nRole);
	if (iter!=m_mapRole.end())
		return iter->second;
	return 0;
}

