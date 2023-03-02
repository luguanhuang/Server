#include "pch.h"
#include "scene/scenemorphinfo.h"
#include "unit/rolemanager.h"
#include "gamelogic/herobattlerecord.h"
#include "gamelogic/bagtransition.h"
#include "config/itemdropconfig.h"
#include "live/livemanager.h"
#include "gamelogic/notice.h"
#include "gamelogic/noticemgr.h"
#include <cmath>
#include "util/gametime.h"
#include "mail/mailsender.h"
#include "mail/mailconfig.h"
#include "gamelogic/XLevelWave.h"
#include "scene/scene.h"
#include "herobattle/ptcg2c_closechooseherontf.h"
#include "herobattle/ptcg2c_herobattleteamrolentf.h"
#include "herobattle//ptcg2c_herobattlecanusehero.h"
#include "gamelogic/herobattlemgr.h"
#include "gamelogic/herobattlerecord.h"
#include "servermgr/servercrossdatamgr.h"

SceneMorphInfo::SceneMorphInfo(Scene* pScene)
{
	m_pScene = pScene;
	m_sceneMorphTime = SceneConfig::Instance()->GetMorphTime(m_pScene->GetSceneTemplateID());
	m_state = MORPH_CHOOSE;
	m_timerHandler = INVALID_HTIMER;
	if (m_sceneMorphTime && m_pScene->GetSpawner())
	{
		m_pScene->GetSpawner()->AddRestTime(m_sceneMorphTime);
	}
	if (m_sceneMorphTime)
	{
		m_timerHandler = CTimerMgr::Instance()->SetTimer(this, 0, 1 * 1000, -1, __FILE__, __LINE__);
	}
}

SceneMorphInfo::~SceneMorphInfo()
{
	if (m_timerHandler)
	{
		CTimerMgr::Instance()->KillTimer(m_timerHandler);
		m_timerHandler = INVALID_HTIMER;
	}
}

bool SceneMorphInfo::Init(const KKSG::SceneSwitchTeamData& data)
{
	if (!_CheckCrossServerInfo(data))
	{
		if (m_sceneMorphTime && m_pScene->GetSpawner())
		{
			m_pScene->GetSpawner()->ResetRestTime();
			m_sceneMorphTime = 0;
		}
		if (m_timerHandler)
		{
			CTimerMgr::Instance()->KillTimer(m_timerHandler);
			m_timerHandler = INVALID_HTIMER;
		}
	}
	if (!CanMorph())
	{
		return true;
	}
	if (!data.has_teamsyndata())
	{
		return false;
	}
	for (int i = 0; i < data.teamsyndata().members_size(); ++i)
	{
		MorphRoleData info;
		info.roleid = data.teamsyndata().members(i).roleid();
		m_roleData[info.roleid] = info;
	}
	if (GSConfig::Instance()->IsCrossGS())
	{
		m_pScene->AddEventListener(this);
	}
	return true;
}

void SceneMorphInfo::OnPreEnterSceneFinally(Scene *pScene, Role* role)
{
	if (m_state == MORPH_CHOOSE)
	{
		///> 隐身
		role->MakeInvisible(true, false);
		role->MakePuppet(true);
	}
}

void SceneMorphInfo::OnEnterSceneFinally(Scene *pScene, Role* role)
{
	if(role->IsWatcher())
	{
		_SendTeamRoleToClient(role);
		return;
	}

	auto& data = m_roleData[role->GetID()];
	data.freeWeekHero = role->Get<CHeroBattleRecord>()->GetFreeWeekHero();
	data.experienceHero = role->Get<CHeroBattleRecord>()->GetExperienceHero();

	if (m_state == MORPH_CHOOSE)
	{
		_SendCanUseHeroToClient(role);
		_SendTeamRoleToClient();
	}
	else if (m_state == MORPH_CHOOSEOVER)
	{
		_CheckAutoChooseHero();
	}
}

void SceneMorphInfo::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (GetLeftMorphTime() == 0)
	{
		if (m_state == MORPH_CHOOSE)
		{
			PtcG2C_CloseChooseHeroNtf ntf;
			m_pScene->Broadcast(ntf);

			_CheckAutoChooseHero();

			m_state = MORPH_CHOOSEOVER;

			if (m_timerHandler)
			{
				CTimerMgr::Instance()->KillTimer(m_timerHandler);
				m_timerHandler = INVALID_HTIMER;
			}
		}
	}
}

bool SceneMorphInfo::CanMorph()
{
	return m_sceneMorphTime > 0;
}

UINT32 SceneMorphInfo::_CheckSetHero(Role* role, UINT32 heroid)
{
	UINT64 roleid = role->GetID();
	if (m_roleData.find(roleid) == m_roleData.end())
	{
		LogError("role [%llu] not in roleData", roleid);
		return KKSG::ERR_UNKNOWN;
	}

	if (HeroBattleMgr::Instance()->GetHeroConf(heroid) == NULL)
	{
		LogWarn("heroid:%u in not exist", heroid);
		return KKSG::ERR_HERO_INVALID;
	}

	auto& data = m_roleData[roleid];
	if (data.heroid)
	{
		LogError("role [%llu] chosen", roleid);
		return KKSG::ERR_HERO_ALREADY_CHOSEN;
	}

	if (!role->Get<CHeroBattleRecord>()->IsHaveHero(heroid))
	{
		bool isHave = false;
		for (auto i = data.freeWeekHero.begin(); i != data.freeWeekHero.end(); i++)
		{
			if (heroid == *i)
			{
				isHave = true;
				break;
			}
		}
		for (auto i = data.experienceHero.begin(); i != data.experienceHero.end(); i++)
		{
			if (heroid == i->first)
			{
				isHave = true;
				break;
			}
		}
		if (!isHave)
		{
			return KKSG::ERR_HERO_INVALID;
		}
	}

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode SceneMorphInfo::SetHero(UINT64 roleid, UINT32 heroid, bool checktime)
{	
	if (checktime && GetLeftMorphTime() == 0)
	{
		return KKSG::ERR_CANTCHOOSEHERO;
	}

	Role* pRole = m_pScene->FindSceneRole(roleid);
	if (pRole == NULL)
	{
		LogError("role [%llu] is NULL", roleid);
		return KKSG::ERR_UNKNOWN;
	}

	UINT32 error = _CheckSetHero(pRole, heroid);
	if (error != KKSG::ERR_SUCCESS)
	{
		return (KKSG::ErrorCode)error;
	}

	auto& data = m_roleData[roleid];
	data.heroid = heroid;

	///> 变身
	pRole->Transform(data.heroid, UseHeroType_MorphInfo);
	///> 能动
	pRole->MakePuppet(false);
	///> 现身
	pRole->MakeInvisible(false);

	_SendTeamRoleToClient();

	_CheckAllChoose();

	return KKSG::ERR_SUCCESS;
}

void SceneMorphInfo::AddExperienceHero(UINT64 roleid, UINT32 heroid, UINT32 endTime)
{
	auto iter = m_roleData.find(roleid);
	if (iter == m_roleData.end())
	{
		return;
	}
	iter->second.experienceHero[heroid] = endTime;
}

void SceneMorphInfo::OnRoleReconnect(Role* pRole)
{
	switch(m_state)
	{
	case MORPH_CHOOSE:
		{
			_SendCanUseHeroToClient(pRole);
		}
		break;
	case MORPH_CHOOSEOVER:
		{
			PtcG2C_CloseChooseHeroNtf oPtc;
			pRole->Send(oPtc);
		}
		break;
	default:
		break;
	}

	_SendTeamRoleToClient(pRole);
}

UINT32 SceneMorphInfo::GetLeftMorphTime()
{
	int costTime = m_pScene->CostTime();
	if (costTime < 0)
	{
		return -1;
	}

	if (costTime >= m_sceneMorphTime)
	{
		return 0;
	}

	return m_sceneMorphTime - costTime;
}

void SceneMorphInfo::OnRevive(Role* role)
{
	if (!CanMorph())
	{
		return;
	}
	
	if (!role)
	{
		return;
	}

	auto find = m_roleData.find(role->GetID());
	if (find == m_roleData.end())
	{
		return;
	}
	if (!find->second.heroid)
	{
		return;
	}
	role->Transform(find->second.heroid, UseHeroType_MorphInfo);
}

void SceneMorphInfo::_SendTeamRoleToClient(Role* pRole)
{
	PtcG2C_HeroBattleTeamRoleNtf oPtc;
	oPtc.m_Data.set_team1(1);
	for (auto i = m_roleData.begin(); i != m_roleData.end(); i++)
	{
		Role* pRole = m_pScene->FindSceneRole(i->first);
		if (pRole == NULL)
		{
			continue;
		}
		KKSG::HeroBattleTeamMember* member = oPtc.m_Data.add_members1();
		member->set_uid(i->first);
		member->set_heroid(i->second.heroid);
		member->set_name(pRole->GetName().c_str());
	}

	if (pRole == NULL) 
		m_pScene->Broadcast(oPtc);
	else 
		pRole->Send(oPtc);
}

void SceneMorphInfo::_CheckAllChoose()
{
	bool allChoose = true;
	for (auto i = m_roleData.begin(); i != m_roleData.end(); i++)
	{
		if (!i->second.heroid)
		{
			allChoose = false;
			break;
		}
	}

	if (allChoose && GetLeftMorphTime())
	{
		if (m_pScene->GetSpawner())
		{
			m_pScene->GetSpawner()->ResetRestTime();
		}
	}
}

void SceneMorphInfo::_SendCanUseHeroToClient(Role* pRole)
{
	if (pRole == NULL || m_roleData.find(pRole->GetID()) == m_roleData.end())
	{
		return;
	}
	PtcG2C_HeroBattleCanUseHero oPtc;

	const std::unordered_set<UINT32>& hero = pRole->Get<CHeroBattleRecord>()->GetHero();
	for (auto i = hero.begin(); i != hero.end(); i++)
	{
		oPtc.m_Data.add_havehero(*i);
	}
	
	MorphRoleData& data = m_roleData[pRole->GetID()];
	for (auto i = data.freeWeekHero.begin(); i != data.freeWeekHero.end(); i++)
	{
		oPtc.m_Data.add_freehero(*i);
	}
	for (auto i = data.experienceHero.begin(); i != data.experienceHero.end(); i++)
	{
		oPtc.m_Data.add_experiencehero(i->first);
	}

	oPtc.m_Data.set_leftchoosetime(GetLeftMorphTime());

	pRole->Send(oPtc);
}

void SceneMorphInfo::_CheckAutoChooseHero()
{
	if (GetLeftMorphTime())
	{
		return;
	}
	for (auto i = m_roleData.begin(); i != m_roleData.end(); i++)
	{
		if (i->second.heroid)
		{
			continue;
		}
		///> 进入选英雄阶段，一段时间没选英雄，随机选择英雄
		Role* pRole = m_pScene->FindSceneRole(i->first);
		if (pRole == NULL)
		{
			LogError("role [%llu] is null", i->first);
			continue;
		}

		std::set<UINT32> exceptHero;
		KKSG::ErrorCode errorCode = SetHero(pRole->GetID(), pRole->Get<CHeroBattleRecord>()->GetRandomHero(i->second.freeWeekHero, i->second.experienceHero, exceptHero), false);
		if (errorCode != KKSG::ERR_SUCCESS)
		{
			LogError("role [%llu] set hero error [%d]", pRole->GetID(), errorCode);
			continue;
		}
		PtcG2C_CloseChooseHeroNtf oPtc;
		pRole->Send(oPtc);
	}
}

bool SceneMorphInfo::_CheckCrossServerInfo(const KKSG::SceneSwitchTeamData& data)
{
	if (!GSConfig::Instance()->IsCrossGS())
	{
		return true;
	}

	if (!data.teamsyndata().members_size())
	{
		return true;
	}
	UINT64 memberID = data.teamsyndata().members(0).roleid();
	auto pCrossData = ServerCrossDataMgr::Instance()->GetDataByRoleID(memberID);
	if (pCrossData && pCrossData->sealtype() >= GetGlobalConfig().PVEMorphLevelSealType)
	{
		return true;
	}

	return false;
}
