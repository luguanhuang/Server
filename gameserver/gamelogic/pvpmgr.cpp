#include "pch.h"
#include "pvpmgr.h"
#include "globalconfig.h"
#include "util/XCommon.h"
#include "noticemgr.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "define/systemiddef.h"
#include "pvprecord.h"
#include "activity/ptcg2c_halliconsntf.h"

INSTANCE_SINGLETON(PvpMgr);


PvpMgr::PvpMgr()
{
}

PvpMgr::~PvpMgr()
{

}

bool PvpMgr::Init()
{
	if(IsInOpenTime())
	{
		m_openInTNoticeFlag = true;
	}
	else
	{
		m_openInTNoticeFlag = false;
	}

	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 3000, -1, __FILE__, __LINE__);

	return true;
}

void PvpMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
}

bool PvpMgr::IsInOpenTime()
{
	return true;
}

time_t PvpMgr::GetGameTimeDown(bool allend)
{
	if(allend)
	{
		return (GetGlobalConfig().PVPGameTimeDown + GetGlobalConfig().PVPGameTimeDown);
	}
	return GetGlobalConfig().PVPGameTimeDown; 
}

void PvpMgr::CheckOpenNotice()
{
	return ;//音容僕
	bool flagNow = IsInOpenTime();
	if(m_openInTNoticeFlag == flagNow)
	{
		return ;
	}
	m_openInTNoticeFlag = flagNow;

	auto itend = RoleManager::Instance()->End();
	for(auto it = RoleManager::Instance()->Begin(); it != itend; ++it)
	{
		HallIconNoticeCheck(*it, m_openInTNoticeFlag);
	}
}

void PvpMgr::HallIconNoticeCheck(Role* pRole, bool open)
{
	return ;//音容僕
	if(NULL == pRole)	
	{
		return ;
	}
	if(!pRole->Get<CRoleSystem>()->IsSystemOpened(SystemId::SYS_PVP))
	{
		return ;
	}

	PtcG2C_HallIconSNtf ptc;
	ptc.m_Data.set_systemid(SystemId::SYS_PVP);
	if(open)
	{
		ptc.m_Data.set_state(KKSG::HICONS_BEGIN);
	}
	else
	{
		ptc.m_Data.set_state(KKSG::HICONS_END);
	}
	pRole->Send(ptc);
	SSDebug << __FUNCTION__ << " role = " << pRole->GetName() << " state = " << ptc.m_Data.state() << END;
}

void PvpMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	CheckOpenNotice();
}

void PvpMgr::OnRoleLogin(Role* pRole)
{
	if(pRole && pRole->Get<MyPvpRecord>()->LeftJoinCountTodayInT() > 0 && IsInOpenTime())
	{
		HallIconNoticeCheck(pRole, true);
	}
}

void PvpMgr::OnRoleFinishBattle(Role* pRole)
{
	if(pRole && pRole->Get<MyPvpRecord>()->LeftJoinCountTodayInT() <= 0 && IsInOpenTime())
	{
		HallIconNoticeCheck(pRole, false);
	}
}
