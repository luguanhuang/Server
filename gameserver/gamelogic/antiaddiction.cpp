#include "pch.h"
#include "unit/role.h"
#include "util/gametime.h"
#include "idip/idiprecord.h"
#include "antiaddictionmgr.h"
#include "tablemgr/prerogativemgr.h"
#include "unit/unit.h"
#include "scene/scene.h"

void Role::InitAntiAddictionInfo()
{
	SetAntiAddictionTime((UINT32)GameTime::GetTime());
	SetSendAntiAddictionRemind(false);
	SetAntiAddictionRemindCount(0);
	SetAntiAddictionHg(HealthyGameFlag0, 0, 0);
	SetIsOpenHg(false);
}

void Role::SetIsOpenHg(bool isGetHg)
{
	m_pRoleInfo->mutable_idiprecord()->set_isgethg(isGetHg);
}

void Role::SetAntiAddictionTime(UINT32 tTime)
{
	m_pRoleInfo->mutable_idiprecord()->set_lastsendantiaddictiontime((UINT32)tTime);
}

void Role::SetSendAntiAddictionRemind(bool isRemind)
{
	m_pRoleInfo->mutable_idiprecord()->set_issendantiaddictionremind(isRemind);
}

void Role::SetAntiAddictionRemindCount(UINT32 count)
{
	m_pRoleInfo->mutable_idiprecord()->set_antiaddictionremindcount(count);
}

void Role::SetAntiAddictionHg(INT32 type, UINT32 hgBanTime, UINT32 hgGameTime)
{
	m_pRoleInfo->mutable_idiprecord()->set_hgflag(type);
	m_pRoleInfo->mutable_idiprecord()->set_hgbantime(hgBanTime);
	m_pRoleInfo->mutable_idiprecord()->set_hggametime(hgGameTime);
}

void Role::CheckAntiAddictionHg()
{
	if (GetCurrScene() == NULL || std::find_if(GetGlobalConfig().ForceExitSceneType.begin(), GetGlobalConfig().ForceExitSceneType.end(), EqualPreID(GetCurrScene()->GetSceneType())) == GetGlobalConfig().ForceExitSceneType.end())
	{
		return;
	}
	UINT32 hgflag = m_pRoleInfo->idiprecord().hgflag();
	UINT32 banTime = 0;
	if (hgflag == HealthyGameFlag2 || hgflag == HealthyGameFlag5)
	{
		banTime = UINT32(GameTime::GetTime()) + m_pRoleInfo->idiprecord().hgbantime();
	}
	else if (hgflag == HealthyGameFlag3  || hgflag == HealthyGameFlag4)
	{
		banTime = m_pRoleInfo->idiprecord().hgbantime();
	}
	else
	{
		return;
	}
	std::string reason = CAntiAddictionMgr::Instance()->GetForceExitMsg(this, hgflag, banTime, m_pRoleInfo->idiprecord().hggametime());
	CAntiAddictionMgr::Instance()->KickAccountNtfClientMsg(GetID(), KKSG::KICK_HG, banTime, reason, hgflag);
	SetAntiAddictionHg(HealthyGameFlag0, 0, 0);
	LogInfo("account=%s,roleid=%llu,hg kick account,banTime=%u,hgflag=%d", GetAccount().c_str(), GetID(), banTime, hgflag);
}




