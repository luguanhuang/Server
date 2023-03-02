#include "pch.h"
#include "weddingmgr.h"
#include "config.h"
#include "timeutil.h"
#include "role/rolesummarymgr.h"
#include "scene/scene.h"
#include "role/rolemanager.h"
#include "scene/sceneswitch.h"
#include "util/gametime.h"
#include "wedding/ptcm2c_weddinginvitentf.h"
#include "util/timespecificmgr.h"
#include "table/globalconfig.h"
#include "wedding/ptcm2g_weddingcarendm2g.h"
#include "network/gslink.h"
#include "marriage/marriage.h"
#include "marriage/marriagemgr.h"
#include "notice/notice.h"

WeddingMgr::WeddingMgr()
:m_autoIncreIndex(0)
{

}

bool WeddingMgr::Init()
{
	CTimerMgr::Instance()->SetTimer(this, 0, 10 * 1000, -1, __FILE__, __LINE__);
	return true;
}

void WeddingMgr::Uninit()
{

}

void WeddingMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	for (auto it = m_wroles.begin(); it != m_wroles.end(); )
	{
		WeddingRole* pWRole = it->second;
		if (pWRole->CanDestory())
		{
			delete pWRole;
			m_wroles.erase(it++);
		}
		else
		{
			it++;
		}
	}

	for (auto it = m_weddings.begin(); it != m_weddings.end(); )
	{
		Wedding* pWed = it->second;
		pWed->Update();
		if (pWed->CheckInvalidEnd())
		{
			LogError("wedding[%llu] invalid end", pWed->GetId());
			delete pWed;
			m_weddings.erase(it++);
		}
		else
		{
			it++;
		}
	}

	UINT32 now = GameTime::GetTime();
	if (m_curWeddingCar.IsRunning() && now > m_curWeddingCar.startTime && (now - m_curWeddingCar.startTime) > 180)
	{
		ResetWeddingCar();
	}
}

Wedding* WeddingMgr::CreateWedding(KKSG::WeddingType type, UINT64 roleId1, UINT64 roleId2)
{
	WeddingRole* pWedRole1 = GetWeddingRole(roleId1);
	WeddingRole* pWedRole2 = GetWeddingRole(roleId2);
	if (pWedRole1 == NULL || pWedRole2 == NULL)
	{
		return NULL;
	}
	UINT64 weddingId = NewID();
	Wedding* pWed = new Wedding(weddingId, type, roleId1, roleId2);

	pWedRole1->SetSelfWeddingId(weddingId);
	pWedRole2->SetSelfWeddingId(weddingId);

	pWed->CreateWeddingScene();

	m_weddings.insert(std::make_pair(weddingId, pWed));
	LogInfo("role1[%llu] role2[%llu] create wedding[%llu] type[%u]", roleId1, roleId2, weddingId, type);
//	Notice notice(WeddingNotice);
	return pWed;
}

Wedding* WeddingMgr::GetWedding(UINT64 weddingId)
{
	auto it = m_weddings.find(weddingId);
	return it == m_weddings.end() ? NULL : it->second;
}

void WeddingMgr::RemoveWedding(UINT64 weddingId)
{
	auto it = m_weddings.find(weddingId);
	if (it == m_weddings.end())
	{
		return;
	}
	delete it->second;
	m_weddings.erase(it);
}

Wedding* WeddingMgr::GetWeddingByRoleId(UINT64 roleId)
{
	WeddingRole* pWedRole = GetWeddingRole(roleId);
	if (pWedRole == NULL)
	{
		return NULL;
	}
	return pWedRole->GetSelfWedding();
}

bool WeddingMgr::IsRoleWeddingHoldding(UINT64 roleId)
{
	return GetWeddingByRoleId(roleId) != NULL;
}

UINT64 WeddingMgr::NewID()
{
	// 16位serverid 32位时间戳 16位自增id
	UINT64 temp = MSConfig::Instance()->GetServerID() & 0xFFFF;
	temp <<= 48;
	UINT64 t = TimeUtil::GetTime();
	temp |= ((t & 0xFFFFFFFF) << 16);
	temp |= ((++m_autoIncreIndex) & 0xFFFF);
	return temp;
}

WeddingRole* WeddingMgr::GetWeddingRole(UINT64 roleId)
{
	auto it = m_wroles.find(roleId);
	if (it != m_wroles.end())
	{
		return it->second;
	}
	CRoleSummary* pSu = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (pSu == NULL)
	{
		return NULL;
	}
	WeddingRole* pWedRole = new WeddingRole(roleId);
	m_wroles.insert(std::make_pair(roleId, pWedRole));
	return pWedRole;
}

void WeddingMgr::OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res)
{
	UINT64 weddingId = roParam.weddinginfo().data().weddingid();
	Wedding* pWed = GetWedding(weddingId);
	if (pWed == NULL)
	{
		LogError("can't find wedding[%llu]", weddingId);
		return;
	}
	if (scene == NULL)
	{
		LogError("create wedding[%llu] failed", weddingId);
		return;
	}
	if (res.result() != KKSG::ERR_SUCCESS)
	{
		LogError("create wedding[%llu] failed", weddingId);
		return;
	}
	LogInfo("Create Wedding[%llu] Scene[%u] Success", weddingId, scene->GetID());
	pWed->SetSceneId(scene->GetID());

	// 切场景
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(pWed->GetRoleId1());
	if (pRole != NULL) pWed->DoEnterWedding(pRole);
	pRole = CRoleManager::Instance()->GetByRoleID(pWed->GetRoleId2());
	if (pRole != NULL) pWed->DoEnterWedding(pRole);
	pWed->SetSendNotice(3);
}

void WeddingMgr::FillAllWeddingInfo(UINT64 roleId, KKSG::GetAllWeddingInfoRes& res)
{
	for (auto it = m_weddings.begin(); it != m_weddings.end(); ++it)
	{
		Wedding* pWed = it->second;
		if (pWed->CanEnter(roleId))
		{
			pWed->FillWeddingBrief(res.add_can_enter());
			continue;
		}
		if (!pWed->StrangerCanSee())
		{
			continue;
		}
		if (pWed->InRefuseList(roleId))
		{
			continue;
		}
		pWed->FillWeddingBrief(res.add_can_apply());
		res.add_is_apply(pWed->InApplyList(roleId));
	}
}

bool WeddingMgr::HasWeddingCarRunning()
{
	return m_curWeddingCar.IsRunning();
}

void WeddingMgr::SetWeddingCar(UINT64 roleId1, UINT64 roleId2, KKSG::WeddingType type)
{
	m_curWeddingCar.roleId1 = roleId1;
	m_curWeddingCar.roleId2 = roleId2;
	m_curWeddingCar.type = type;
	m_curWeddingCar.startTime = GameTime::GetTime();
}

bool WeddingMgr::OnWeddingCarEnd()
{
	if (!m_curWeddingCar.IsRunning())
	{
		return true;
	}
	LogInfo("Wedding Car End");
	PtcM2G_WeddingCarEndM2G ntf;
	ntf.m_Data.set_roleid1(m_curWeddingCar.roleId1);
	ntf.m_Data.set_roleid2(m_curWeddingCar.roleId2);
	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, ntf);

	Marriage* pMarr1 = MarriageMgr::Instance()->GetMarriage(m_curWeddingCar.roleId1);
	Marriage* pMarr2 = MarriageMgr::Instance()->GetMarriage(m_curWeddingCar.roleId2);

	ResetWeddingCar();
	return true;
}

void WeddingMgr::ResetWeddingCar()
{
	m_curWeddingCar.roleId1 = 0;
	m_curWeddingCar.roleId2 = 0;
	m_curWeddingCar.startTime = 0;
}

void WeddingMgr::StartWeddingCarTimer()
{
	if (m_curWeddingCar.startTime == 0)
	{
		return;
	}
	UINT32 t = m_curWeddingCar.startTime + GetGlobalConfig().WeddingPatrolTime;
	CTimeSpecificMgr::Instance()->RegisterEvent(t, std::bind(&WeddingMgr::OnWeddingCarEnd, this));
	LogInfo("role1[%llu] role2[%luu] start wedding car", m_curWeddingCar.roleId1, m_curWeddingCar.roleId2);

	Marriage* pMarr1 = MarriageMgr::Instance()->GetMarriage(m_curWeddingCar.roleId1);
	Marriage* pMarr2 = MarriageMgr::Instance()->GetMarriage(m_curWeddingCar.roleId2);

	if (pMarr1) pMarr1->SetTakeWeddingCar();
	if (pMarr2) pMarr2->SetTakeWeddingCar();
}

void WeddingMgr::NotifyWeddingCar()
{
	PtcM2C_WeddingInviteNtf ntf;
	ntf.m_Data.set_type(KKSG::Wedding_CarCutScene);
	CRole* pRole1 = CRoleManager::Instance()->GetByRoleID(m_curWeddingCar.roleId1);
	CRole* pRole2 = CRoleManager::Instance()->GetByRoleID(m_curWeddingCar.roleId2);
	if (pRole1) pRole1->Send(ntf);
	if (pRole2) pRole2->Send(ntf);
}

