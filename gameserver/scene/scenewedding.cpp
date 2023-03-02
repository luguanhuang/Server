#include "pch.h"
#include "scenewedding.h"
#include "scene.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "util/gametime.h"
#include "wedding/ptcg2c_weddingstatentf.h"
#include "wedding/ptcg2c_weddingloadinfontf.h"
#include "wedding/ptcg2c_weddingeventntf.h"
#include "table/globalconfig.h"
#include "unit/roleoutlook.h"
#include "common/ptcg2c_outlookchangentf.h"
#include "wedding/ptcg2m_updateweddingstateg2m.h"
#include "network/mslink.h"
#include "table/globalconfig.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/rolemisc.h"
#include "mail/mailconfig.h"
#include "mail/mailsender.h"
#include "define/maildef.h"
#include "gamelogic/notice.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "table/CutSceneReader.h"
#include "timeutil.h"
#include "unit/roleview.h"

SceneWedding::SceneWedding(Scene* scene)
:m_pScene(scene)
{
	m_state = KKSG::WeddingState_Prepare;
	m_stateStartTime = 0;
	m_happyness = 0;
	m_timer = INVALID_HTIMER;
	m_isEnd = false;
	m_prepareVows = false;
	m_vows = false;
	m_vowsEnd = false;
	m_vowsTime = 0;
	m_vowsEndTime = 0;
	m_lastPushStateTime = 0;
}

SceneWedding::~SceneWedding()
{
	if (INVALID_HTIMER != m_timer)
	{
		CTimerMgr::Instance()->KillTimer(m_timer);
		m_timer = INVALID_HTIMER;
	}
}

void SceneWedding::Init(const KKSG::WeddingCreateInfo& info)
{
	m_brief = info.data();
	m_weddingId = m_brief.weddingid();
	m_type = m_brief.type();
	m_roleId1 = m_brief.role1().roleid();
	m_roleId2 = m_brief.role2().roleid();

	m_state = KKSG::WeddingState_Prepare;
	m_stateStartTime = TimeUtil::GetTime();
	PushWeddingStateToMs();

	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
}

void SceneWedding::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (m_isEnd)
	{
		return;
	}
	UpdateState();
	UpdateHappyness();
	UpdateVows();
}

void SceneWedding::UpdateState()
{
	if (GetStateLeftTime() == 0)
	{
		if (m_state == KKSG::WeddingState_Prepare)
		{
			m_state = KKSG::WeddingState_Running;
			m_stateStartTime = TimeUtil::GetTime();
			m_lastHappynessTime = m_stateStartTime;
			PushWeddingState();
			PushWeddingStateToMs();
		}
		else
		{
			OnEnd();
			PushWeddingStateToMs();
			m_pScene->SetDontDestory(false);
		}
	}
	UINT32 now = TimeUtil::GetTime();
	if (now > m_lastPushStateTime + 10)
	{
		PushWeddingState();
	}
}

void SceneWedding::UpdateHappyness()
{
	if (m_state != KKSG::WeddingState_Running)
	{
		return;
	}
	if (m_happyness == GetGlobalConfig().WeddingMaxHappyness)
	{
		return;
	}
	UINT32 now = TimeUtil::GetTime();
	if (now >= m_lastHappynessTime + GetGlobalConfig().WeddingHappynessInterval)
	{
		AddHappyness(GetGlobalConfig().WeddingHappynessOneTime);
		m_lastHappynessTime = now;
	}
}

void SceneWedding::UpdateVows()
{
	if (m_vows == false && m_vowsTime != 0)
	{
		if (TimeUtil::GetTime() >= m_vowsTime)
		{
			m_vows = true;
			DoVows();
			m_vowsEndTime = TimeUtil::GetTime() + CutSceneReader::LoadCutSceneTime((GetGlobalConfig().WeddingCutscenePath+ ".txt").c_str()) / 1000;
		}
	}
	else if (m_vows == true && m_vowsEnd == false)
	{
		if (TimeUtil::GetTime() >= m_vowsEndTime)
		{
			DoVowsEnd();
			m_vowsEnd = true;
		}
	}
}

void SceneWedding::DoVows()
{
	LogInfo("Vows");
	Vector3 pos;
	float face=0.0;
	Role* pRole1 = m_pScene->FindSceneRole(m_roleId1);
	if (pRole1)
	{
		pos.x = GetGlobalConfig().WeddingVowsHusBandPos[0];
		pos.y = GetGlobalConfig().WeddingVowsHusBandPos[1];
		pos.z = GetGlobalConfig().WeddingVowsHusBandPos[2];
		face  = GetGlobalConfig().WeddingVowsHusBandPos[3];
		XEntity* entity = dynamic_cast<XEntity*>(pRole1->GetXObject());
		if(entity)
		{
			m_rolePos1 = entity->GetPosition();
			m_roleFace1 = entity->GetFaceDegree();
			entity->CorrectLocation(pos, face, true);
		}
		pRole1->MakePuppet(true, true);
	}
	Role* pRole2 = m_pScene->FindSceneRole(m_roleId2);
	if (pRole2)
	{
		pos.x = GetGlobalConfig().WeddingVowsWifePos[0];
		pos.y = GetGlobalConfig().WeddingVowsWifePos[1];
		pos.z = GetGlobalConfig().WeddingVowsWifePos[2];
		face  = GetGlobalConfig().WeddingVowsWifePos[3];
		XEntity* entity = dynamic_cast<XEntity*>(pRole2->GetXObject());
		if(entity)
		{
			m_rolePos2 = entity->GetPosition();
			m_roleFace2 = entity->GetFaceDegree();
			entity->CorrectLocation(pos, face, true);
		}
		pRole2->MakePuppet(true, true);
	}

	PushWeddingEvent(NULL, KKSG::WeddingOper_VowsStart);
	PushWeddingState();
}

void SceneWedding::DoVowsEnd()
{
	LogInfo("VowsEnd");
	Role* pRole1 = m_pScene->FindSceneRole(m_roleId1);
	if (pRole1)
	{
		XEntity* entity = dynamic_cast<XEntity*>(pRole1->GetXObject());
		if(entity && !m_rolePos1.IsZero())
		{
			entity->CorrectLocation(m_rolePos1, m_roleFace1, true);
		}
		pRole1->MakePuppet(false, true);
	}

	Role* pRole2 = m_pScene->FindSceneRole(m_roleId2);
	if (pRole2)
	{
		XEntity* entity = dynamic_cast<XEntity*>(pRole2->GetXObject());
		if(entity && !m_rolePos2.IsZero())
		{
			entity->CorrectLocation(m_rolePos2, m_roleFace2, true);
		}
		pRole2->MakePuppet(false, true);
	}
}

void SceneWedding::OnEnd()
{
	LogInfo("OnEnd");
	m_isEnd = true;
	m_pScene->KickAllRoleOut();
	if (m_type == KKSG::WeddingType_Luxury)
	{
		SendMail();
	}
}

void SceneWedding::PrepareVows()
{
	m_prepareVows = true;
	m_vows = false;
	m_vowsTime = TimeUtil::GetTime() + 10;
}

void SceneWedding::AddHappyness(UINT32 value)
{
	if (m_happyness >= GetGlobalConfig().WeddingMaxHappyness)
	{
		return;
	}
	m_happyness += value;
	if (m_happyness > GetGlobalConfig().WeddingMaxHappyness)
	{
		m_happyness = GetGlobalConfig().WeddingMaxHappyness;
	}
	PushWeddingState();
}

bool SceneWedding::IsOwner(UINT64 roleId)
{
	return roleId == m_roleId1 || roleId == m_roleId2;
}

WeddingRoleData* SceneWedding::GetRoleData(UINT64 roleId)
{
	auto it = m_data.find(roleId);
	return it == m_data.end() ? NULL : &it->second;
}

KKSG::ErrorCode SceneWedding::DoFlower(Role* pRole)
{
	WeddingRoleData* pData = GetRoleData(pRole->GetID());
	if (pData == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	UINT32 now = TimeUtil::GetTime();
	if (now < pData->lastFlowTime + GetGlobalConfig().WeddingFlowCD)
	{
		return KKSG::ERR_WEDDING_OPER_CD;
	}
	pData->lastFlowTime = now;

	if (GetGlobalConfig().WeddingFlowerCost[0] != 0)
	{
		BagTakeItemTransition trans(pRole);
		trans.SetReason(ItemFlow_Wedding, ItemFlow_WeddingFlow);
		if (!trans.TakeItem(GetGlobalConfig().WeddingFlowerCost[0], GetGlobalConfig().WeddingFlowerCost[1] * m_pScene->GetRoleCount()))
		{
			trans.RollBack();
			return GetErrorCode(GetGlobalConfig().WeddingFlowerCost[0]);
		}
		trans.NotifyClient();
	}

	GiveFlowerReward();
	PushWeddingEvent(pRole, KKSG::WeddingOper_Flower);
	AddHappyness(GetGlobalConfig().WeddingFlowerAddHp);

	Notice notice(WeddingFlowerNotice);
	notice.Add(pRole);
	for (auto it = m_data.begin(); it != m_data.end(); ++it)	
	{
		Role* role = m_pScene->FindSceneRole(it->first);
		if (role)
		{
			notice.SendSelf(role);
		}
	}

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode SceneWedding::DoFireworks(Role* pRole)
{
	WeddingRoleData* pData = GetRoleData(pRole->GetID());
	if (pData == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	UINT32 now = TimeUtil::GetTime();
	if (now < pData->lastFireworksTime + GetGlobalConfig().WeddingFlowCD)
	{
		return KKSG::ERR_WEDDING_OPER_CD;
	}
	pData->lastFireworksTime = now;

	if (GetGlobalConfig().WeddingFireworksCost[0] != 0)
	{
		BagTakeItemTransition trans(pRole);
		trans.SetReason(ItemFlow_Wedding, ItemFlow_WeddingFireworks);
		if (!trans.TakeItem(GetGlobalConfig().WeddingFireworksCost[0], GetGlobalConfig().WeddingFireworksCost[1] * m_pScene->GetRoleCount()))
		{
			trans.RollBack();
			return GetErrorCode(GetGlobalConfig().WeddingFireworksCost[0]);
		}
		trans.NotifyClient();
	}


	GiveFireworksReward();
	PushWeddingEvent(pRole, KKSG::WeddingOper_Fireworks);
	AddHappyness(GetGlobalConfig().WeddingFireworksAddHp);

	Notice notice(WeddingFireworksNotice);
	notice.Add(pRole);
	for (auto it = m_data.begin(); it != m_data.end(); ++it)	
	{
		Role* role = m_pScene->FindSceneRole(it->first);
		if (role)
		{
			notice.SendSelf(role);
		}
	}

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode SceneWedding::DoApplyVows(Role* pRole)
{
	if (!IsOwner(pRole->GetID()))
	{
		return KKSG::ERR_WEDDING_NOT_OWNER;
	}
	if (m_state != KKSG::WeddingState_Running)
	{
		return KKSG::ERR_WEDDING_NOT_RUNNING;
	}
	if (m_happyness < GetGlobalConfig().WeddingMaxHappyness)
	{
		return KKSG::ERR_WEDDING_HP_NOT_ENOUGH;
	}
	if (m_prepareVows)
	{
		return KKSG::ERR_WEDDING_ALREADY_VOWS;
	}

	WeddingRoleData* pSelfData = GetRoleData(pRole->GetID());
	UINT64 otherRoleId = pRole->GetID() == m_roleId1 ? m_roleId2 : m_roleId1;
	WeddingRoleData* pOtherData = GetRoleData(otherRoleId);
	if (pSelfData == NULL || pOtherData == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	pSelfData->applyVows = true;
	pSelfData->applyVowsTime = TimeUtil::GetTime();

	if (pOtherData->IsApplyVows())	
	{
		PrepareVows();
		PushWeddingEvent(pRole, KKSG::WeddingOper_VowsPrepare);
	}
	else
	{
		Role* pOther = m_pScene->FindSceneRole(otherRoleId);
		if (pOther) PushWeddingEvent(pRole, KKSG::WeddingOper_ApplyVows, pOther);
	}

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode SceneWedding::DoAgreeVows(Role* pRole)
{
	if (!IsOwner(pRole->GetID()))
	{
		return KKSG::ERR_WEDDING_NOT_OWNER;
	}
	if (m_happyness < GetGlobalConfig().WeddingMaxHappyness)
	{
		return KKSG::ERR_WEDDING_HP_NOT_ENOUGH;
	}
	if (m_prepareVows)
	{
		return KKSG::ERR_WEDDING_ALREADY_VOWS;
	}

	WeddingRoleData* pSelfData = GetRoleData(pRole->GetID());
	UINT64 otherRoleId = pRole->GetID() == m_roleId1 ? m_roleId2 : m_roleId1;
	WeddingRoleData* pOtherData = GetRoleData(otherRoleId);
	if (pSelfData == NULL || pOtherData == NULL)
	{
		return KKSG::ERR_FAILED;
	}

	if (!pOtherData->IsApplyVows())
	{
		return KKSG::ERR_FAILED;
	}
	pSelfData->applyVows = true;
	pSelfData->applyVowsTime = TimeUtil::GetTime();

	PrepareVows();
	PushWeddingEvent(pRole, KKSG::WeddingOper_VowsPrepare);
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode SceneWedding::DoDisagreeVows(Role* pRole)
{
	if (!IsOwner(pRole->GetID()))
	{
		return KKSG::ERR_WEDDING_NOT_OWNER;
	}
	if (m_happyness < GetGlobalConfig().WeddingMaxHappyness)
	{
		return KKSG::ERR_WEDDING_HP_NOT_ENOUGH;
	}
	if (m_prepareVows)
	{
		return KKSG::ERR_WEDDING_ALREADY_VOWS;
	}

	WeddingRoleData* pSelfData = GetRoleData(pRole->GetID());
	UINT64 otherRoleId = pRole->GetID() == m_roleId1 ? m_roleId2 : m_roleId1;
	WeddingRoleData* pOtherData = GetRoleData(otherRoleId);
	if (pSelfData == NULL || pOtherData == NULL)
	{
		return KKSG::ERR_FAILED;
	}

	if (!pOtherData->IsApplyVows())
	{
		return KKSG::ERR_FAILED;
	}
	pOtherData->applyVows = false;

	Role* pOther = m_pScene->FindSceneRole(otherRoleId);
	if (pOther) PushWeddingEvent(pRole, KKSG::WeddingOper_DisAgreeVows, pOther);

	return KKSG::ERR_SUCCESS;
}

void SceneWedding::OnPreEnterScene(Scene *pScene, Role* role)
{
	PushLoadingInfo(role);
	if (IsOwner(role->GetID()))
	{
		role->SetUseWeddingFashion(true);
		PtcG2C_OutLookChangeNtf m_ntf;
		RoleOutLook outlook(role);
		outlook.FillWeddingDisplayFash();
		outlook.SendToSelf();
	}
	if (role->GetID() != m_roleId1) role->GetRoleView()->SetMustSee(m_roleId1);
	if (role->GetID() != m_roleId2) role->GetRoleView()->SetMustSee(m_roleId2);
}

void SceneWedding::OnLeaveScene(Scene *pScene, Role* role)
{
	PushWeddingEvent(NULL, KKSG::WeddingOper_RoleNum);
	if (IsOwner(role->GetID()))
	{
		role->SetUseWeddingFashion(false);
		PtcG2C_OutLookChangeNtf m_ntf;
		RoleOutLook outlook(role);
		outlook.FillDisplayFashion();
		outlook.SendToSelf();
	}
}

void SceneWedding::OnAddNonRole(Scene *pScene, Unit* unit)
{
	UINT32 templateID = unit->GetTemplateID();
	bool isCandyNpc = false;
	if (m_type == KKSG::WeddingType_Normal)
	{
		auto& temp = GetGlobalConfig().NormalWeddingNpcIDs;
		if (std::find(temp.begin(), temp.end(), templateID) != temp.end())
		{
			isCandyNpc = true;
		}
	}
	else
	{
		auto& temp = GetGlobalConfig().LuxuryWeddingNpcIDs;
		if (std::find(temp.begin(), temp.end(), templateID) != temp.end())
		{
			isCandyNpc = true;
		}
	}
	if (isCandyNpc == false)
	{
		return;
	}

	PushWeddingEvent(NULL, KKSG::WeddingOper_Candy);


	auto pConf = XEntityInfoLibrary::Instance()->TemplateID2Data_Enemy(templateID);
	if (pConf == NULL)
	{
		return;
	}
	Notice notice(WeddingNpcEnterNotice);
	notice.Replace("$C", pConf->Name);
	notice.Replace("$R", m_brief.role1().name());
	notice.Replace("$R", m_brief.role2().name());
	for (auto it = m_data.begin(); it != m_data.end(); ++it)	
	{
		Role* pRole = m_pScene->FindSceneRole(it->first);
		if (pRole)
		{
			notice.SendSelf(pRole);
		}
	}
}

void SceneWedding::OnEnterSceneFinally(Scene *pScene, Role* role)
{
	PushWeddingState(role);
	PushWeddingEvent(NULL, KKSG::WeddingOper_RoleNum);
	WeddingRoleData* pData = GetRoleData(role->GetID());
	if (pData == NULL)
	{
		WeddingRoleData data;
		data.roleId = role->GetID();
		m_data.insert(std::make_pair(role->GetID(), data));
	}
}

void SceneWedding::OnRoleReconnect(Role* role)
{
	PushWeddingState(role);
}

UINT32 SceneWedding::GetStateLeftTime()
{
	UINT32 now = TimeUtil::GetTime();
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

void SceneWedding::GMSetStateEnd()
{
	LogInfo("GMStateEnd");
	m_stateStartTime = 0;
}

void SceneWedding::PushLoadingInfo(Role* pRole)
{
	PtcG2C_WeddingLoadInfoNtf ntf;	
	ntf.m_Data.mutable_info()->CopyFrom(m_brief);
	pRole->Send(ntf);
}

void SceneWedding::PushWeddingState(Role* pRole)
{
	PtcG2C_WeddingStateNtf ntf;
	ntf.m_Data.set_state(m_state);
	ntf.m_Data.set_lefttime(GetStateLeftTime());
	ntf.m_Data.set_happyness(m_happyness);
	ntf.m_Data.set_vows(m_vows);
	if (pRole)
	{
		pRole->Send(ntf);
	}
	else
	{
		m_pScene->Broadcast(ntf);
		m_lastPushStateTime = TimeUtil::GetTime();
	}
}

void SceneWedding::PushWeddingEvent(Role* pEventRole, KKSG::WeddingOperType type, Role* pRole)
{
	PtcG2C_WeddingEventNtf ntf;	
	ntf.m_Data.set_type(type);
	if (pEventRole)
	{
		ntf.m_Data.set_roleid(pEventRole->GetID());
		ntf.m_Data.set_rolename(pEventRole->GetName());
	}
	ntf.m_Data.set_total_num(m_pScene->GetRoleCount());

	if (pRole)
	{
		pRole->Send(ntf);
	}
	else
	{
		m_pScene->Broadcast(ntf);
	}
}

void SceneWedding::PushWeddingStateToMs()
{
	PtcG2M_UpdateWeddingStateG2M ntf;
	ntf.m_Data.set_weddingid(m_weddingId);
	ntf.m_Data.set_state(m_state);
	ntf.m_Data.set_statestarttime(m_stateStartTime);
	ntf.m_Data.set_is_end(m_isEnd);
	MSLink::Instance()->SendTo(ntf);
}

void SceneWedding::GiveFlowerReward()
{
	const std::list<Role*>& allRoles = m_pScene->GetAllRoles();
	for (auto it = allRoles.begin(); it != allRoles.end(); ++it)
	{
		Role* pRole = *it;
		if (pRole->Get<RoleMisc>()->CheckAndAddWeddingFlowerCount())
		{
			BagGiveItemTransition trans(pRole);
			trans.SetReason(ItemFlow_Wedding, ItemFlow_WeddingFlow);
			for (size_t i = 0; i < GetGlobalConfig().WeddingFlowerReward.size(); ++i)
			{
				auto& item = GetGlobalConfig().WeddingFlowerReward[i];
				trans.GiveItem(item[0], item[1]);
			}
			trans.NotifyClient();
		}
		else
		{
			//PushWeddingEvent(NULL, KKSG::WeddingOper_FlowerRewardOverMax, pRole);
		}
	}
}

void SceneWedding::GiveFireworksReward()
{
	const std::list<Role*>& allRoles = m_pScene->GetAllRoles();
	for (auto it = allRoles.begin(); it != allRoles.end(); ++it)
	{
		Role* pRole = *it;
		if (pRole->Get<RoleMisc>()->CheckAndAddWeddingFireworksCount())
		{
			BagGiveItemTransition trans(pRole);
			trans.SetReason(ItemFlow_Wedding, ItemFlow_WeddingFireworks);
			for (size_t i = 0; i < GetGlobalConfig().WeddingFireworksReward.size(); ++i)
			{
				auto& item = GetGlobalConfig().WeddingFireworksReward[i];
				trans.GiveItem(item[0], item[1]);
			}
			trans.NotifyClient();
		}
		else
		{
			//PushWeddingEvent(NULL, KKSG::WeddingOper_FireworksRewardOverMax, pRole);
		}
	}
}

void SceneWedding::SendMail()
{
	const MailConf* pConf = MailConfig::Instance()->GetMailConf(MailConf_WeddingThanks);
	if (pConf == NULL)
	{
		return;
	}
	std::string titleTemp = MailConfig::Instance()->ReplaceString("%s1", m_brief.role1().name(), pConf->m_title);
	std::string title = MailConfig::Instance()->ReplaceString("%s2", m_brief.role2().name(), titleTemp);
	MailSender sender;
	sender.SetReason(ItemFlow_Wedding, ItemFlow_WeddingThanks);
	sender.SetContent(Mail_System, title, pConf->m_content);
	sender.AddConfigItem(pConf);

	for (auto it = m_data.begin(); it != m_data.end(); ++it)
	{
		UINT64 roleId = it->second.roleId;
		if (IsOwner(roleId))
		{
			continue;
		}
		sender.Send(roleId);
	}
}


