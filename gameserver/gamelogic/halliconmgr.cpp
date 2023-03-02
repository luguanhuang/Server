#include "pch.h"
#include "halliconmgr.h"
#include "globalconfig.h"
#include "util/XCommon.h"
#include "noticemgr.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "define/systemiddef.h"
#include "pvprecord.h"
#include "activity/ptcg2c_halliconsntf.h"
#include "MultActivityMgr.h"
#include "util/gametime.h"
#include "table/OpenSystemMgr.h"

INSTANCE_SINGLETON(HallIconMgr);


HallIconMgr::HallIconMgr(){}

HallIconMgr::~HallIconMgr(){}

bool HallIconMgr::Init()
{
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	m_mapIcon[MULT_ACTIVITY_SKYCITY].nState		= eStop;
	m_mapIcon[MULT_ACTIVITY_SKYCITY].nTime		= 0;
	m_mapIcon[MULT_ACTIVITY_SKYCITY].nSystemID	= SYS_SKYCITY;
	return true;
}

void HallIconMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
}

void HallIconMgr::CheckSkyCityState(UINT32 nID)
{
	UINT32 nOpenTime = MultActivityMgr::Instance()->GetOpenTime(nID);
	UINT32 nEndTime  = MultActivityMgr::Instance()->GetEndTime(nID);
	bool bOpen       = MultActivityMgr::Instance()->IsActivityTimeOpen(nID);

	UINT32 nTime = GameTime::GetTime();
	if (bOpen&&m_mapIcon[nID].nState==eStop&&nTime>=m_mapIcon[nID].nTime)
	{
		m_mapIcon[nID].nState  = eRuning;
		m_mapIcon[nID].nTime   = nEndTime;
		CheckOpenNotice(m_mapIcon[nID].nSystemID, KKSG::HICONS_BEGIN);
	}

	if (m_mapIcon[nID].nState == eRuning &&nTime>m_mapIcon[nID].nTime)
	{
		m_mapIcon[nID].nState = eStop;
		CheckOpenNotice(m_mapIcon[nID].nSystemID, KKSG::HICONS_END);
	}
}

void HallIconMgr::CheckOpenNotice(UINT32 nSystemID,UINT32 nState)
{
	auto itend = RoleManager::Instance()->End();
	for(auto it = RoleManager::Instance()->Begin(); it != itend; ++it)
	{
		HallIconNoticeCheck(*it, nSystemID,nState);
	}
}

void HallIconMgr::HallIconNoticeCheck(Role* pRole,UINT32 nSystemID,UINT32 nState)
{
	if(!pRole)	return ;
	if (OpenSystemMgr::Instance()->IsClosedSystem(nSystemID, pRole->GetID())) return ;
	PtcG2C_HallIconSNtf ptc;
	ptc.m_Data.set_systemid(nSystemID);
	ptc.m_Data.set_state((KKSG::HallIconState)nState);
	pRole->Send(ptc);
}

void HallIconMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	for(auto iter = m_mapIcon.begin(); iter!=m_mapIcon.end(); iter++)
	{
		CheckSkyCityState(iter->first);
	}
}

void HallIconMgr::OnRoleLogin(Role* pRole)
{
	for (auto iter = m_mapIcon.begin(); iter!=m_mapIcon.end(); iter++)
	{
		UINT32 nID = iter->first;
		UINT32 nState = iter->second.nState;
		UINT32 nSystemID = iter->second.nSystemID;
		if(nState==eRuning)
		{
			HallIconNoticeCheck(pRole,nSystemID,KKSG::HICONS_BEGIN);
		}else if(nState==eStop)
		{
			HallIconNoticeCheck(pRole,nSystemID,KKSG::HICONS_END);
		}
	}
}


void HallIconMgr::OnFirstEnterScene(Role* pRole)
{
	bool bRet = MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_WEEKENDNEST);
	if (bRet)
	{
		HallIconNoticeCheck(pRole,WeekEndNest,KKSG::HICONS_BEGIN);
	}
}
