#include "pch.h"
#include "combatstatistics.h"
#include "timeutil.h"
#include "table/globalconfig.h"
#include "entity/XActionDefine.h"
#include "entity/XRole.h"
#include "role.h"
#include "unit/enemy.h"
#include "scene/scene.h"
#include "unit.h"
#include "utility/UtilMath.h"
#include "battle/ptcg2c_dpsnotify.h"
#include "unit/dummyrole.h"
#include "rolemanager.h"
#include "dummyrolemanager.h"

CombatStatistics::CombatStatistics()
{
	m_pUnit = NULL;
	Reset();
}

CombatStatistics::~CombatStatistics()
{

}

void CombatStatistics::AddComboCount()
{
	UINT64 now = TimeUtil::GetMilliSecond();
	UINT32 interval = (UINT32)(now - m_lastComboTime);

	UINT32 comboInterval = GetGlobalConfig().ComboInterval;
	if (interval < comboInterval)
	{
		++m_comboCount;
	}
	else
	{
		m_comboCount = 1;
	}

	if (m_comboCount > m_comboMax)
	{
		m_comboMax = m_comboCount;
	}

	if(m_pUnit->GetCurrScene() != NULL && m_pUnit->GetCurrScene()->GetSceneInfo()->m_pConf->HasComboBuff)
	{
		for (size_t i = 0; i <  GetGlobalConfig().ComboBuff.size(); ++i)
		{
			if (m_comboCount ==  GetGlobalConfig().ComboBuff[i][0])
			{
				XAddBuffActionArgs args;
				args.singleBuff.buffID =  GetGlobalConfig().ComboBuff[i][1];
				args.singleBuff.BuffLevel =  GetGlobalConfig().ComboBuff[i][2];
				args.Caster = m_pUnit->GetID();
				m_pUnit->GetXObject()->DispatchAction(&args);
			}
		}
	}

	XComboChangeArgs comboArg(m_comboCount);
	m_pUnit->GetXObject()->DispatchAction(&comboArg);

	m_lastComboTime = now;
}

void CombatStatistics::UpdateDPS()
{
	UINT64 nowtime = TimeUtil::GetMilliSecond();
	if (nowtime > m_lastdpstime + 200)
	{
		///> 计算dps
		if (!UtilMath::IsFloatZero(m_timeTotal))
		{
			float tempdps = (float)m_totalDamage/m_timeTotal;
			if (m_dps != tempdps)
			{
				m_dps = tempdps;
				PtcG2C_DPSNotify ntf;
				ntf.m_Data.set_dps(m_dps);
				//SSInfo<<"dps:"<<m_dps<<END;
				m_pUnit->Send(ntf);
			}
			m_lastdpstime = nowtime;
		}
	}
}

void CombatStatistics::Update()
{
	UINT64 nowTime = TimeUtil::GetMilliSecond();
	UpdateAssit(nowTime);
}

void CombatStatistics::UpdateAssit(UINT64 nowTime)
{
	if (nowTime < m_lastAssitTime + 500)
	{
		return;
	}
	m_lastAssitTime = nowTime;
	for (auto it = m_HurtRoleID.begin(); it != m_HurtRoleID.end();)
	{
		// 助攻过期
		if (nowTime >= it->second)
		{
			m_HurtRoleID.erase(it++);
		}
		else
		{
			it++;
		}
	}
}

void CombatStatistics::AddAttackInfo(Unit *pTargetUnit, double value)
{
	if (value <= 0)
	{
		return;
	}
	m_totalDamage += value;

	if (NULL == pTargetUnit)//可能为NULL
	{
		return ;
	}

	if (pTargetUnit->IsEnemy() && ((Enemy*)pTargetUnit)->IsBoss())
	{
		m_bossdamage += value;
	}
}

void CombatStatistics::AddTreatInfo(Unit *pTargetUnit, double value)
{
	if (value > 0)
	{
		m_totalTreat += value;
		return;
	}
}

void CombatStatistics::Reset()
{	
	m_lastdpstime = 0;
	m_lastAssitTime = 0;
	m_dps = 0;

	m_lastComboTime = 0;
	m_totalDamage = 0;
	m_bossdamage = 0;
	m_totalTreat = 0;
	m_beHitDamage = 0;
	m_deathCount = 0;
	m_comboCount = 0;
	m_comboMax = 0;
	m_behitCount = 0;
	m_openChestCount = 0;
	
	m_killCount = 0;
	m_killContinueCount = 0;
	m_killContinueCountMax = 0;
	m_assistNum = 0;
	m_multiKillCount = 0;
	m_multiKillCountMax = 0;
	m_lastKillTime = 0;
	
	m_timeTotal = 0;
	m_timeBase = 0;
	m_timeEnd = 0;

	m_killEnemyCount.clear();	
	m_HurtRoleID.clear();
	m_assistIDs.clear();
}

void CombatStatistics::BeHit()
{
	++m_behitCount;
}

void CombatStatistics::BeHurt(Unit* pCastTarget, double value)
{
	m_beHitDamage += value;
	if (pCastTarget == NULL)
	{
		return;
	}
	switch (Unit::GetUnitType(pCastTarget->GetID()))
	{
	case KKSG::Category_Role:
	case KKSG::Category_DummyRole:
		{
			UINT64 t = TimeUtil::GetMilliSecond();
			m_HurtRoleID[pCastTarget->GetID()] = t + GetGlobalConfig().AssistDamageTime * 1000;
			break;
		}
	default:
		{
			break;
		}
	}
}

void CombatStatistics::AddOpenChest()
{
	++m_openChestCount;
}

void CombatStatistics::SetUnit(Unit *pUnit)
{
	m_pUnit = pUnit;
}

void CombatStatistics::AddDeathCount()
{
	++m_deathCount;
	m_killContinueCount = 0;
	m_multiKillCount = 0;

	UINT64 killerID = m_pUnit->GetKillerID();

	std::set<std::pair<UINT64, UINT64>> hurtUnitSet;
	for (auto i = m_HurtRoleID.begin(); i != m_HurtRoleID.end(); i++)
	{
		hurtUnitSet.insert(std::make_pair(i->second, i->first));
	}
	m_HurtRoleID.clear();

	m_assistIDs.clear();
	for (auto i = hurtUnitSet.begin(); i != hurtUnitSet.end(); i++)
	{
		if (i->second == killerID || i->second == m_pUnit->GetID())
		{
			continue;
		}
		switch (Unit::GetUnitType(i->second))
		{
		case KKSG::Category_Role:
			{
				Role* pRole = RoleManager::Instance()->FindByRoleID(i->second);
				if (pRole != NULL)
				{
					pRole->Statistics().AddAssistNum();
				}
				m_assistIDs.push_back(i->second);
				break;
			}
		case KKSG::Category_DummyRole:
			{
				DummyRole* pDummyRole = DummyRoleManager::Instance()->Find(i->second);
				if (pDummyRole != NULL)
				{
					pDummyRole->Statistics().AddAssistNum();
				}
				m_assistIDs.push_back(i->second);
				break;
			}
		default:
			{
				break;
			}
		}
	}	
}

UINT64 CombatStatistics::GetLastAssist()
{
	if (m_assistIDs.empty())
	{
		return 0;
	}
	auto iter = m_assistIDs.end();
	iter--;
	UINT64 ret = *iter;
	m_assistIDs.pop_back();
	return ret;
}

void CombatStatistics::UpdateMultiKillCount()
{
	if (m_multiKillCount == 0)
	{
		return;
	}
	UINT64 nowTime = TimeUtil::GetMilliSecond();
	UINT32 pos = m_multiKillCount;
	if (pos > GetGlobalConfig().MultiKillTime.size())
	{
		pos = GetGlobalConfig().MultiKillTime.size();
	}
	if (m_lastKillTime + GetGlobalConfig().MultiKillTime[pos - 1] * 1000 <= nowTime)
	{
		m_multiKillCount = 0;
	}
}

UINT32 CombatStatistics::GetMultiKillCount()
{
	UpdateMultiKillCount();
	return m_multiKillCount;
}

void CombatStatistics::AddKillCount()
{
	++m_killCount;
	++m_killContinueCount;
	m_killContinueCountMax = std::max(m_killContinueCountMax, m_killContinueCount);

	UpdateMultiKillCount();
	++m_multiKillCount;
	m_multiKillCountMax = std::max(m_multiKillCountMax, m_multiKillCount);
	m_lastKillTime = TimeUtil::GetMilliSecond();
}

int CombatStatistics::GetKillCount() const
{
	return m_killCount;
}

int CombatStatistics::GetKillContinueCount()
{
	return m_killContinueCount;
}

int CombatStatistics::GetKillContinueCountMax() const
{
	return m_killContinueCountMax;
}

void CombatStatistics::AddKillEnemyCount(UINT32 enemyTemplateID)
{
	++m_killEnemyCount[enemyTemplateID];
}

std::map<UINT32, UINT32> CombatStatistics::GetKillEnemyCount() const
{
	return m_killEnemyCount;
}

int CombatStatistics::GetKillEnemyScore() const
{
	UINT32 sceneID = m_pUnit->GetCurrScene() ? m_pUnit->GetCurrScene()->GetSceneTemplateID() : 0;
	if (!sceneID)
	{
		return 0;
	}

	return SceneConfig::Instance()->CalcKillMonsterSocre(sceneID, m_killEnemyCount);
}

void CombatStatistics::AppendTime()
{
	float now = TimeUtil::GetMilliSecond() / 1000.0f;

	m_timeTotal += (now - m_timeBase);
	m_timeBase = now;
}

void CombatStatistics::MarkTimeBaseLine()
{
	m_timeBase = TimeUtil::GetMilliSecond() / 1000.0f - m_timeEnd;
}

void CombatStatistics::MarkTimEndLine()
{
	m_timeEnd = TimeUtil::GetMilliSecond() / 1000.0f - m_timeBase;
}

void CombatStatistics::StartPrintDamage(float time)
{
	m_startPrintDamage = m_totalDamage;
}

double CombatStatistics::StopPrintDamage()
{
	return m_totalDamage - m_startPrintDamage;
}
