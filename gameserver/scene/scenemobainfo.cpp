#include "pch.h"
#include "scenemobainfo.h"
#include "scene.h"
#include "unit/role.h"
#include "scenevsbase.h"
#include "scenemobabattle.h"
#include "unit/attributewatcher.h"
#include "gamelogic/herobattlemgr.h"
#include "mobabattle/ptcg2c_mobasignalbroadcast.h"
#include "mobabattle/ptcg2c_mobaaddexpntf.h"
#include "scene/ptcg2c_mobahintntf.h"
#include "gamelogic/military_rank_record.h"

void MobaHintNotify::Notify(bool toother)
{
	if (0 == group)
	{
		PtcG2C_MobaHintNtf ntf;
		ntf.m_Data.set_index(index);
		scene->Broadcast(ntf);
	}
	else
	{
		if (group == MOBA_GROUP_1 || group == MOBA_GROUP_2)
		{
			PtcG2C_MobaHintNtf ntfself;
			ntfself.m_Data.set_index(index);
			scene->BroadCastSameGroup(group, ntfself);

			if (toother)
			{
				PtcG2C_MobaHintNtf ntfother;
				ntfother.m_Data.set_index(index + 1);
				scene->BroadCastSameGroup(group == MOBA_GROUP_1 ? MOBA_GROUP_2 : MOBA_GROUP_1, ntfother);
			}
		}
	}
}

MobaRole::MobaRole(const KKSG::PvpRoleInfo& _info, SceneMobaBattle* _pSceneMoba)
{
	m_pSceneMoba = _pSceneMoba;
	m_pRole = NULL;
	m_changeFlag = 0;
	m_info = _info;
	m_enterGameTime = 0;
	m_nextReviveTime = 0;
	m_heroid = 0;
	m_canChooseHero = false;
	m_exp = 0;
	m_level = 1;
	m_upgradeNum = 0;
	m_attackLevel = 0;
	m_defenseLevel = 0;
	m_firstSendSignalTime = 0;
	m_signalCount = 0;
	m_kda = 0.0;
	m_isEscape = false;
	m_escapeTime = 0;
}

void MobaRole::ToKKSG(KKSG::MobaRoleData& data, UINT32 field)
{
	field |= m_changeFlag;

	data.set_uid(GetRoleID());
	if (field == MobaRole_All)
	{
		data.set_name(m_info.name());
	}
	if (field & MobaRole_Exp)
	{
		data.set_exp(m_exp);
		data.set_level(m_level);	
		data.set_upgradenum(m_upgradeNum);
	}
	if (field & MobaRole_HeroID)
	{
		data.set_heroid(m_heroid);
	}
	if (field & MobaRole_KillStats)
	{
		CombatStatistics& stat = GetCombatStatistics();
		data.set_killnum(stat.GetKillCount());
		data.set_deathnum(stat.GetDeathCount());
		data.set_assistnum(stat.GetAssistNum());
	}
	if (field & MobaRole_AddAttr)
	{
		data.set_attacklevel(m_attackLevel);
		data.set_defenselevel(m_defenseLevel);
	}
	if (field & MobaRole_ReviveTime)
	{
		UINT64 nowTime = TimeUtil::GetMilliSecond();
		if (m_nextReviveTime != 0 && m_nextReviveTime > nowTime)
		{
			data.set_revivetime((UINT32)((m_nextReviveTime - nowTime) / 1000));
		}
		else
		{
			data.set_revivetime(0);
		}
	}
	m_changeFlag = 0;
}

CombatStatistics& MobaRole::GetCombatStatistics()
{
	if (m_pRole != NULL)
	{
		return m_pRole->Statistics();
	}
	return m_combatStatistics;
}

void MobaRole::OnEnterGame(Role* pRole)
{
	if (pRole == NULL)
	{
		return;
	}
	m_pRole = pRole;
	m_enterGameTime = TimeUtil::GetMilliSecond();	
	m_canChooseHero = true;
	m_freeWeekHero = m_pRole->Get<CHeroBattleRecord>()->GetFreeWeekHero();
	m_experienceHero = m_pRole->Get<CHeroBattleRecord>()->GetExperienceHero();
	m_strOpenID = m_pRole->GetAccount();
}

void MobaRole::OnLeaveGame(bool isIntentional)
{
	if (m_pRole == NULL)
	{
		return;
	}
	m_combatStatistics = m_pRole->GetStatistics();
	m_escapeTime = TimeUtil::GetMilliSecond();
	if (isIntentional)
	{
		m_pRole->Get<MilitaryRankRecord>()->ReduceBattleRecord(KKSG::SCENE_MOBA);
	}
	m_pRole = NULL;
}

void MobaRole::OnGameEnd(UINT64 nowTime, UINT32 timeSpan)
{
	CombatStatistics& stat = GetCombatStatistics();
	m_kda = (stat.GetKillCount() * 2 + stat.GetAssistNum()) / (stat.GetDeathCount() + 1.0f);

	if (m_enterGameTime == 0)
	{
		m_isEscape = true;
	}
	else if (m_escapeTime != 0)
	{
		if (m_pRole == NULL && nowTime > m_escapeTime)
		{
			UINT32 escapeTimeSpan = (UINT32)((nowTime - m_escapeTime) / 1000);
			if (timeSpan != 0)
			{
				double percent = 100.0 * escapeTimeSpan / timeSpan;
				m_isEscape = escapeTimeSpan >= GetGlobalConfig().EscapeMinTime && percent >= GetGlobalConfig().EscapeTimeRate;
				if (m_isEscape)
				{
					LogInfo("role [%llu] escape in MobaBattle, timeSpan [%u], escapeTimeSpan [%u], percent [%.2lf]", GetRoleID(), timeSpan, escapeTimeSpan, percent);
				}
			}
		}
	}
}

void MobaRole::AddExp(double addExp, UINT32 posxz)
{
	if (m_heroid == 0)
	{
		LogError("role [%llu] in MobaBattle, heroid is 0, but add exp", GetRoleID());
		return;
	}
	double realAddExp = 0.0;
	while (addExp > 0)
	{
		const MobaLevelConf* conf = MobaBattleMgr::Instance()->GetMobaLevelConf(m_level);
		if (conf == NULL)
		{
			LogError("level [%u], MobaLevelConf is NULL!!!!!", m_level);
			break;
		}
		if (conf->Exp < m_exp) m_exp = conf->Exp;
		if (addExp >= conf->Exp - m_exp)
		{
			addExp -= conf->Exp - m_exp;
			realAddExp += conf->Exp - m_exp;
			m_exp = conf->Exp;
			if (m_level == MobaBattleMgr::Instance()->GetMobaMaxLevel())
			{
				break;
			}
			else
			{
				m_level++;
				m_exp = 0;
				m_upgradeNum++;
				OnLevelUp();
			}
		}
		else
		{
			m_exp += addExp;
			realAddExp += addExp;
			break;
		}
	}
	MobaTeamInfo* teamInfo = m_pSceneMoba->GetMobaTeamInfo(GetTeamID());
	if (teamInfo)
	{
		teamInfo->allExp += realAddExp;
	}

	if (posxz != 0 && m_pRole != NULL && realAddExp > 0.0)
	{
		PtcG2C_MobaAddExpNtf oPtc;
		oPtc.m_Data.set_addexp(realAddExp);
		oPtc.m_Data.set_posxz(posxz);
		m_pRole->Send(oPtc);
	}

	m_changeFlag |= MobaRole_Exp;
}

void MobaRole::OnLevelUp()
{
	if (m_pRole == NULL)
	{
		return;
	}
	const MobaHeroConf* conf = MobaBattleMgr::Instance()->GetMobaHeroConf(m_heroid, m_level);
	if (conf == NULL)
	{
		LogError("heroid [%u], level [%u], MobaHeroConf is NULL!!!!!", m_heroid, m_level);
		return;
	}
	AttributeWatcher watcher(m_pRole);
	for (auto i = conf->AddAttr.begin(); i != conf->AddAttr.end(); ++i)
	{
		watcher.AddAttr((CombatAttrDef)(int)(*i)[0], (*i)[1]);
	}
	watcher.CheckChangedAttributeAndNotify(true);
}

void MobaRole::SetNextReviveTime(UINT64 nextRevive)
{
	m_nextReviveTime = nextRevive;
	m_changeFlag |= MobaRole_ReviveTime;
}

void MobaRole::CheckRevive(UINT64 nowTime, bool isBeRevive)
{
	if (m_nextReviveTime == 0)
	{
		return;
	}
	if (!isBeRevive)
	{
		if (m_nextReviveTime > nowTime)
		{
			return;
		}
	}
	m_nextReviveTime = 0;
	if (m_pRole != NULL && m_pRole->IsDead())
	{
		///> 拉位置
		if (!isBeRevive)
		{
			m_pSceneMoba->TranInitPosition(GetRoleID());
		}
		///> 复活
		m_pRole->Revive();
		///> 变身
		m_pRole->Transform(m_heroid, UseHeroType_MobaBattle);

		m_changeFlag |= MobaRole_ReviveTime;

		LogDebug("role [%llu] revive in MobaBattle", GetRoleID());
	}
}

void MobaRole::AddExperienceHero(UINT32 heroid, UINT32 endTime)
{
	m_experienceHero[heroid] = endTime;
}

KKSG::ErrorCode MobaRole::SetHero(UINT32 heroid)
{
	if (m_pRole == NULL)
	{
		LogError("role [%llu] is NULL", GetRoleID());
		return KKSG::ERR_UNKNOWN;
	}

	if (HeroBattleMgr::Instance()->GetHeroConf(heroid) == NULL)
	{
		LogWarn("heroid:%u in not in OverWatch.txt", heroid);
		return KKSG::ERR_HERO_INVALID;
	}

	if (!m_pRole->Get<CHeroBattleRecord>()->IsHaveHero(heroid))
	{
		bool isHave = false;	
		for (auto i = m_freeWeekHero.begin(); i != m_freeWeekHero.end(); ++i)
		{
			if (heroid == *i)
			{
				isHave = true;
				break;
			}
		}
		for (auto i = m_experienceHero.begin(); i != m_experienceHero.end(); ++i)
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

	if (m_canChooseHero)
	{
		m_heroid = heroid;
		m_canChooseHero = false;
		m_changeFlag |= MobaRole_HeroID;
		///> 变身
		m_pRole->Transform(heroid, UseHeroType_MobaBattle);
		///> 能动
		m_pRole->MakePuppet(false);
		///> 现身
		m_pRole->MakeInvisible(false);

		OnLevelUp();

		return KKSG::ERR_SUCCESS;
	}
	return KKSG::ERR_CANTCHOOSEHERO;
}

KKSG::ErrorCode MobaRole::Upgrade(UINT32 param, UINT32& nowParam)
{
	if (m_pRole == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	if (m_upgradeNum <= 0)
	{
		return KKSG::ERR_MOBA_UPGRADENUM_EMPTY;
	}
	switch (param)
	{
	case 0:
		{
			m_upgradeNum--;
			m_attackLevel++;
			nowParam = m_attackLevel;

			AttributeWatcher watcher(m_pRole);
			for (auto i = GetGlobalConfig().MobaUpgradeAttack.begin(); i != GetGlobalConfig().MobaUpgradeAttack.end(); ++i)
			{
				watcher.AddAttr((CombatAttrDef)(*i)[0], (*i)[1]);
			}
			watcher.CheckChangedAttributeAndNotify(true);

			m_changeFlag |= MobaRole_Exp | MobaRole_AddAttr;
			return KKSG::ERR_SUCCESS;
		}
	case 1:
		{
			m_upgradeNum--;
			m_defenseLevel++;
			nowParam = m_defenseLevel;

			AttributeWatcher watcher(m_pRole);
			for (auto i = GetGlobalConfig().MobaUpgradeDefense.begin(); i != GetGlobalConfig().MobaUpgradeDefense.end(); ++i)
			{
				watcher.AddAttr((CombatAttrDef)(*i)[0], (*i)[1]);
			}
			watcher.CheckChangedAttributeAndNotify(true);

			m_changeFlag |= MobaRole_Exp | MobaRole_AddAttr;
			return KKSG::ERR_SUCCESS;
		}
	case 2:
		{	
			KKSG::ErrorCode errorCode = m_pSceneMoba->AddGroupLevel(GetTeamID(), 1);
			if (errorCode == KKSG::ERR_SUCCESS)
			{
				m_upgradeNum--;
				nowParam = m_pSceneMoba->GetGroupLevel(GetTeamID());

				m_changeFlag |= MobaRole_Exp;
			}
			return errorCode;
		}
	default:
		{
			LogWarn("role [%llu], upgrade in MobaBattle, param [%d] is unknown", GetRoleID(), param);
			return KKSG::ERR_FAILED;
		}
	}
}

KKSG::ErrorCode MobaRole::SendSignal(UINT32 type)
{
	if (m_pRole == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	UINT64 nowTime = TimeUtil::GetMilliSecond();
	if (m_signalCount + 1 > GetGlobalConfig().MobaSignaTimelLimit[0])
	{
		if (m_firstSendSignalTime + GetGlobalConfig().MobaSignaTimelLimit[1] * 1000 >= nowTime)
		{
			return KKSG::ERR_MOBA_SIGNAL_TOOFAST;
		}
		m_signalCount = 1;
		m_firstSendSignalTime = nowTime;
	}
	else
	{
		++m_signalCount;
	}

	PtcG2C_MobaSignalBroadcast oPtc;
	oPtc.m_Data.set_type(type);
	oPtc.m_Data.set_uid(GetRoleID());
	const Vector3& vec3 = m_pRole->GetXObject()->GetPosition_p();
	UINT32 posxz = (int)(vec3.z * 100 + 0.5f);
	posxz |= ((int)(vec3.x * 100 + 0.5f) << 16);
	oPtc.m_Data.set_posxz(posxz);
	m_pSceneMoba->SendToTeamRole(GetTeamID(), oPtc);

	return KKSG::ERR_SUCCESS;
}

bool MobaRole::IsAlreadyReport(UINT64 roleid)
{
	return m_reportIDs.find(roleid) != m_reportIDs.end();
}

void MobaRole::AddReporter(UINT64 roleid)
{
	m_reportIDs.insert(roleid);
}

void MobaRole::FillTLog(TMobaBattleFlow& tlog)
{
	tlog.m_RoleId = GetRoleID();
	tlog.m_Level = m_info.level();
	tlog.m_Profession = m_info.profession();
	tlog.m_HeroID = m_heroid;
	tlog.m_TeamID = GetTeamID() == m_pSceneMoba->GetTeam1() ? 1 : 2;
	CombatStatistics& stat = GetCombatStatistics();
	tlog.m_DeathCount = stat.GetDeathCount();
	tlog.m_KillCount = stat.GetKillCount();
	tlog.m_AssistCount = stat.GetAssistNum();
	tlog.m_KillContinueCountMax = stat.GetKillContinueCountMax();
	tlog.m_MultiKillCountMax = stat.GetMultiKillCountMax();
	tlog.m_Damage = (UINT64)stat.GetTotalDamage();
	tlog.m_BeHitDamage = (UINT64)stat.GetBeHitDamage();
	tlog.m_IsRunAway = m_isEscape;
	tlog.m_MobaLevel = m_level;
	tlog.m_strOpenId = m_strOpenID;
}