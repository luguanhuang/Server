#include "pch.h"
#include "wedding.h"
#include "scene/scenecreate.h"
#include "weddingmgr.h"
#include "role/rolesummarymgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "wedding/ptcm2c_weddinginvitentf.h"
#include "table/globalconfig.h"
#include "util/gametime.h"
#include "scene/sceneswitch.h"
#include "loghelper/tlogr.h"
#include "marriagemgr.h"
#include "marriage.h"
#include "timeutil.h"

Wedding::Wedding(UINT64 id, KKSG::WeddingType type, UINT64 roleId1, UINT64 roleId2)
:m_id(id)
,m_type(type)
,m_roleID1(roleId1)
,m_roleID2(roleId2)
,m_permitStranger(true)
,m_sceneId(0)
{
	m_state = KKSG::WeddingState_Prepare;
	m_stateStartTime = TimeUtil::GetTime();
	m_createTime = m_stateStartTime;
	m_sendNoticeTime = 0;
	m_sendNotice = false;
}

bool Wedding::IsOwner(UINT64 roleId)
{
	return roleId == m_roleID1 || roleId == m_roleID2;
}

void Wedding::FillWeddingBrief(KKSG::WeddingBrief* data)
{
	data->set_weddingid(m_id);
	data->set_type(m_type);
	CRoleSummary* pSu = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roleID1);
	if (pSu)
	{
		pSu->FillWeddingRoleInfo(*data->mutable_role1());
	}
	pSu = CRoleSummaryMgr::Instance()->GetRoleSummary(m_roleID2);
	if (pSu)
	{
		pSu->FillWeddingRoleInfo(*data->mutable_role2());
	}
	data->set_state(m_state);
	data->set_lefttime(GetLeftTime());
}

void Wedding::FillInviteInfo(UINT64 roleId, KKSG::GetWeddingInviteInfoRes& res)
{
	WeddingRole* pWRole = WeddingMgr::Instance()->GetWeddingRole(roleId);
	if (pWRole == NULL)
	{
		return;
	}

	const std::vector<UINT64>& inviteList = pWRole->GetInviteList();
	for (auto it = inviteList.begin(); it != inviteList.end(); ++it)
	{
		CRoleSummary* pSu = CRoleSummaryMgr::Instance()->GetRoleSummary(*it);
		if (pSu == NULL)
		{
			continue;
		}
		pSu->FillWeddingRoleInfo(*res.add_invitelist());
		res.add_invite_enter(IsEntered(*it));
	}

	for (auto it = m_applyList.begin(); it != m_applyList.end(); ++it)
	{
		CRoleSummary* pSu = CRoleSummaryMgr::Instance()->GetRoleSummary(*it);
		if (pSu == NULL)
		{
			continue;
		}
		pSu->FillWeddingRoleInfo(*res.add_applylist());
	}
}

void Wedding::CreateWeddingScene()
{
	// 建立战场
	KKSG::CreateBattleParam param;
	KKSG::WeddingCreateInfo* info = param.mutable_weddinginfo();
	FillWeddingBrief(info->mutable_data());

	if (m_type == KKSG::WeddingType_Normal)
	{
		CSceneCreator::Instance()->CreateBattleScene(GetGlobalConfig().NormalWeddingSceneId, WeddingMgr::Instance(), param);
	}
	else
	{
		CSceneCreator::Instance()->CreateBattleScene(GetGlobalConfig().LuxuryWeddingSceneId, WeddingMgr::Instance(), param);
	}
}

bool Wedding::CanEnter(UINT64 roleId)
{
	if (roleId == m_roleID1 || roleId == m_roleID2)
	{
		return true;
	}
	return std::find(m_inviteList.begin(), m_inviteList.end(), roleId) != m_inviteList.end();
}

bool Wedding::InApplyList(UINT64 roleId)
{
	return std::find(m_applyList.begin(), m_applyList.end(), roleId) != m_applyList.end();
}

void Wedding::RemoveFromApplyList(UINT64 roleId)
{
	for (auto it = m_applyList.begin(); it != m_applyList.end(); ++it)
	{
		if (*it == roleId)
		{
			m_applyList.erase(it);
			break;
		}
	}
}

bool Wedding::InRefuseList(UINT64 roleId)
{
	return std::find(m_refuseList.begin(), m_refuseList.end(), roleId) != m_refuseList.end();
}

bool Wedding::IsEntered(UINT64 roleId)
{
	return std::find(m_enterList.begin(), m_enterList.end(), roleId) != m_enterList.end();
}

KKSG::ErrorCode Wedding::DoInvite(UINT64 operId, UINT64 roleId)
{
	CRoleSummary* pSu = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (pSu == NULL)
	{
		return KKSG::ERR_ROLE_NOTEXIST;
	}
	if (CanEnter(roleId))
	{
		return KKSG::ERR_WEDDING_ALREADY_INVITE;
	}
	WeddingRole* pWRole = WeddingMgr::Instance()->GetWeddingRole(operId);
	if (pWRole == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	if (pWRole->GetInviteNum() >= GetMaxInviteNum())
	{
		return KKSG::ERR_WEDDING_INVITE_OVERMAX;
	}
	pWRole->AddToInviteList(roleId);
	m_inviteList.push_back(roleId);
	RemoveFromApplyList(roleId);

	PushInviteNtf(KKSG::Wedding_Invite, roleId);
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode Wedding::DoAgreeApply(UINT64 operId, UINT64 roleId)
{
	if (CanEnter(roleId))
	{
		return KKSG::ERR_WEDDING_ALREADY_INVITE;
	}
	if (!InApplyList(roleId))
	{
		return KKSG::ERR_WEDDING_NOT_APPLY;
	}
	WeddingRole* pWRole = WeddingMgr::Instance()->GetWeddingRole(operId);
	if (pWRole == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	if (pWRole->GetInviteNum() >= GetMaxInviteNum())
	{
		return KKSG::ERR_WEDDING_INVITE_OVERMAX;
	}
	pWRole->AddToInviteList(roleId);
	m_inviteList.push_back(roleId);
	RemoveFromApplyList(roleId);

	PushInviteNtf(KKSG::Wedding_Invite, roleId);
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode Wedding::DoDisAgreeApply(UINT64 operId, UINT64 roleId)
{
	if (CanEnter(roleId))
	{
		return KKSG::ERR_WEDDING_ALREADY_INVITE;
	}
	RemoveFromApplyList(roleId);
	if (!InRefuseList(roleId))
	{
		m_refuseList.push_back(roleId);
	}

	PushInviteNtf(KKSG::Wedding_DisagreeApply, roleId);
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode Wedding::DoApply(UINT64 roleId)
{
	CRoleSummary* pSu = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (pSu == NULL)
	{
		return KKSG::ERR_ROLE_NOTEXIST;
	}
	if (CanEnter(roleId))
	{
		return KKSG::ERR_WEDDING_ALREADY_INVITE;
	}
	if (InRefuseList(roleId))
	{
		return KKSG::ERR_WEDDING_ALREADY_REFUSE;
	}
	if (InApplyList(roleId))
	{
		return KKSG::ERR_WEDDING_ALREADY_APPLY;
	}
	m_applyList.push_back(roleId);

	PtcM2C_WeddingInviteNtf ntf;
	ntf.m_Data.set_type(KKSG::Wedding_Apply);
	FillWeddingBrief(ntf.m_Data.mutable_weddinginfo());
	pSu->FillWeddingRoleInfo(*ntf.m_Data.mutable_applyer());
	
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_roleID1);
	if (pRole) pRole->Send(ntf);
	pRole = CRoleManager::Instance()->GetByRoleID(m_roleID2);
	if (pRole) pRole->Send(ntf);

	return KKSG::ERR_SUCCESS;
}

void Wedding::PushInviteNtf(KKSG::WeddingInviteOperType type, UINT64 roleId)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole == NULL)
	{
		return;
	}
	PtcM2C_WeddingInviteNtf ntf;
	ntf.m_Data.set_type(type);
	FillWeddingBrief(ntf.m_Data.mutable_weddinginfo());
	pRole->Send(ntf);
}

KKSG::ErrorCode Wedding::DoEnterWedding(CRole* pRole)
{
	UINT64 roleId = pRole->GetID();
	if (!CanEnter(roleId))
	{
		return KKSG::ERR_WEDDING_NOT_INVITE;
	}
	if (!IsEntered(roleId))
	{
		m_enterList.push_back(roleId);
	}
	if (m_sceneId == 0)
	{
		return KKSG::ERR_FAILED;
	}

	KKSG::SceneSwitchData switchData;
	CSceneSwitch::EnterScene(pRole, m_sceneId, switchData);

	DoTxLog(pRole->GetID());
	return KKSG::ERR_SUCCESS;
}

void Wedding::SetState(KKSG::WeddingState state, UINT32 startTime)
{
	m_state = state;	
	m_stateStartTime = startTime;
	if (m_state == KKSG::WeddingState_Prepare)
	{
		m_createTime = TimeUtil::GetTime();
	}
}

UINT32 Wedding::GetStateLeftTime()
{
	UINT32 now = GameTime::GetTime();
	UINT32 endTime = 0;
	if (m_state == KKSG::WeddingState_Prepare)
	{
		endTime = m_stateStartTime + GetGlobalConfig().WeddingPrepareTime;
	}
	else
	{
		endTime = m_stateStartTime + GetGlobalConfig().WeddingRunningTime;
	}

	return endTime > now ? (endTime - now) : 0;
}

UINT32 Wedding::GetLeftTime()
{
	UINT32 now = TimeUtil::GetTime();
	UINT32 endTime = m_createTime + GetGlobalConfig().WeddingPrepareTime + GetGlobalConfig().WeddingRunningTime;
	return endTime > now ? (endTime - now) : 0;
}

void Wedding::OnEnd()
{
	LogInfo("wedding[%llu] roleid1[%llu] roleid2[%llu] end", m_id, m_roleID1, m_roleID2);
	WeddingRole* pWRole1 = WeddingMgr::Instance()->GetWeddingRole(m_roleID1);
	WeddingRole* pWRole2 = WeddingMgr::Instance()->GetWeddingRole(m_roleID2);
	if (pWRole1) pWRole1->OnSelfWeddingEnd();
	if (pWRole2) pWRole2->OnSelfWeddingEnd();
}

UINT32 Wedding::GetMaxInviteNum()
{
	if (m_type == KKSG::WeddingType_Normal)
	{
		return GetGlobalConfig().NormalWeddingInviteNum;
	}
	return GetGlobalConfig().LuxuryWeddingInviteNum;
}

bool Wedding::CheckInvalidEnd()
{
	UINT32 now = TimeUtil::GetTime();
	if (now > m_createTime + GetGlobalConfig().WeddingPrepareTime + GetGlobalConfig().WeddingRunningTime + 60)
	{
		WeddingRole* pWRole1 = WeddingMgr::Instance()->GetWeddingRole(m_roleID1);
		WeddingRole* pWRole2 = WeddingMgr::Instance()->GetWeddingRole(m_roleID2);
		if (pWRole1) pWRole1->SetSelfWeddingId(0);
		if (pWRole2) pWRole2->SetSelfWeddingId(0);
		return true;
	}
	return false;
}

void Wedding::Update()
{
	UINT32 now = TimeUtil::GetTime();
	if (m_sendNotice && now >= m_sendNoticeTime)
	{
		PtcM2C_WeddingInviteNtf ntf;
		ntf.m_Data.set_type(KKSG::Wedding_Start);
		FillWeddingBrief(ntf.m_Data.mutable_weddinginfo());
		CRole* pRole1 = CRoleManager::Instance()->GetByRoleID(m_roleID1);
		CRole* pRole2 = CRoleManager::Instance()->GetByRoleID(m_roleID2);
		if (pRole1) pRole1->Send(ntf);
		if (pRole2) pRole2->Send(ntf);

		m_sendNotice = false;
	}
}

void Wedding::SetSendNotice(UINT32 after)
{
	m_sendNotice = true;
	m_sendNoticeTime = TimeUtil::GetTime() + after;
}

void Wedding::DoTxLog(UINT64 roleId)
{
	TWeddingFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = roleId;
	oLog.m_RoleID1 = GetRoleId1();
	oLog.m_RoleID2 = GetRoleId2();
	oLog.m_WeddingID = GetId();
	Marriage* pMarr = MarriageMgr::Instance()->GetMarriage(GetRoleId1());
	oLog.m_WeddingType = pMarr?pMarr->GetMarriageType():0;
	oLog.Do();
}
