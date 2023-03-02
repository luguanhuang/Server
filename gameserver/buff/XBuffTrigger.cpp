#include "pch.h"
#include "XBuffTrigger.h"
#include "entity/XEntity.h"
#include "util/XRandom.h"
#include "timeutil.h"
#include "XBuffTemplateManager.h"
#include "component/XBuffComponent.h"
#include "util/XCommon.h"
#include "component/XAIComponent.h"
#include "XBuffTriggerByBeHit.h"
#include "XBuffTriggerByLife.h"
#include "XBuffTriggerAlways.h"
#include "XBuffTriggerByHit.h"
#include "XBuffTriggerByCombo.h"
#include "XBuffTriggerNearbyMates.h"
#include "XBuffTriggerByDeath.h"
#include "XBuffTriggerByQTE.h"
#include "XBuffTriggerWhenRemove.h"
#include "gamelogic/combateffect.h"
#include "XBuffTriggerByCastSkill.h"
#include <cmath>
#include "XBuffTriggerByStacking.h"

bool XBuffTrigger::TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff)
{
	if (pBuffInfo->BuffTriggerCond == 0)
		return false;

	switch(pBuffInfo->BuffTriggerCond)
	{
	case 1:
		pBuff->AddBuffEffect(new XBuffTriggerByBeHit(pBuff));
		break;
	case 2:
		pBuff->AddBuffEffect(new XBuffTriggerByLife(pBuff));
		break;
	case 3:
		pBuff->AddBuffEffect(new XBuffTriggerAlways(pBuff));
		break;
	case 4:
		pBuff->AddBuffEffect(new XBuffTriggerByHit(pBuff));
		break;
	case 5:
		pBuff->AddBuffEffect(new XBuffTriggerByCombo(pBuff));
		break;
	case 6:
		pBuff->AddBuffEffect(new XBuffTriggerNearbyMates(pBuff));
		break;
	case 7:
		pBuff->AddBuffEffect(new XBuffTriggerByDeath(pBuff));
		break;
	case 8:
		pBuff->AddBuffEffect(new XBuffTriggerByQTE(pBuff));
		break;
	case 9:
		pBuff->AddBuffEffect(new XBuffTriggerWhenRemove(pBuff));
		break;
	case 10:
		pBuff->AddBuffEffect(new XBuffTriggerByCastSkill(pBuff));
		break;
	case 11:
		pBuff->AddBuffEffect(new XBuffTriggerByStacking(pBuff));
		break;
	default:
		break;
	}

	return true;
}

XBuffTrigger::XBuffTrigger(XBuff *pBuff)
{
	m_triggerRate = pBuff->GetBuffInfo()->BuffTriggerRate;
	
	m_bRandomTriggerBuff = false;
	if (pBuff->GetBuffInfo()->BuffTriggerBuff.size() > 0)
	{
		const Sequence<int, 2>& data = pBuff->GetBuffInfo()->BuffTriggerBuff[0];
		if(data[0] == 0 && data[1] == 0)
		{
			m_bRandomTriggerBuff = true;
		}
	}

	m_triggerSkill = XCommon::XHash(pBuff->GetBuffInfo()->BuffTriggerSkill.c_str());

	m_entity = NULL;

	// 如果没有填_triggerRate,就认为100%触发
	if (m_triggerRate == 0.0)
		m_triggerRate = 1.0;

	m_pTriggerCondition = NULL;
	m_pActiveTriggerCondition = NULL;
	m_pBuff = pBuff;
	m_bSkillTriggered = false;
	m_bIsTriggerImm = pBuff->GetBuffInfo()->IsTriggerImm;
}

XBuffTrigger::~XBuffTrigger()
{
	if(m_pTriggerCondition != NULL)
		delete m_pTriggerCondition;
}

void XBuffTrigger::OnAdd(XEntity* pEntity, CombatEffectHelper* pEffectHelper)
{
	m_entity = pEntity;
	m_pTarget = pEntity;

	m_triggerRate += pEffectHelper->GetBuffTriggerRate();

	if(m_pBuff->GetBuffInfo()->IsGlobalTrigger)
	{
		m_pActiveTriggerCondition = m_entity->BuffComponent()->GetTriggerState(m_pBuff->GetBuffInfo());
	}
	else
	{
		m_pTriggerCondition = new XTriggerCondition(m_pBuff->GetBuffInfo());
		m_pActiveTriggerCondition = m_pTriggerCondition;
	}
	m_bSkillTriggered = false;
}

void XBuffTrigger::Trigger()
{
	if (m_pActiveTriggerCondition->CanTrigger())
	{
		if (CheckTriggerCondition())
		{
			double randVal = XRandom::randDouble(0, 1);
			if (randVal <= m_triggerRate)
			{
				m_pActiveTriggerCondition->OnTrigger();
				_OnTrigger();
				AddTriggerBuff();
				if(m_bIsTriggerImm)
					CastTriggerSkill();
				else
					m_bSkillTriggered = true;
				//m_lastTriggerTime = TimeUtil::GetMilliSecond();
			}
			else
				m_pActiveTriggerCondition->OnRandFail();
		}
	}
}

void XBuffTrigger::AddTriggerBuff()
{
	if(m_pBuff->GetBuffInfo()->BuffTriggerBuff.size() == 0)
		return;
	
	const vector<Sequence<int, 2>>& buffList = m_pBuff->GetBuffInfo()->BuffTriggerBuff;

	UINT32 startIndex = 0;
	UINT32 endIndex = buffList.size();

	if (m_bRandomTriggerBuff)
	{
		startIndex = XRandom::randInt(1, endIndex);
		endIndex = startIndex + 1;
	}

	XAddBuffActionArgs args;
	args.Caster = m_pBuff->GetCaster();		// 使用buff施放者作为该buff的施放者
	args.EffectImm = m_bIsTriggerImm;
	for(UINT32 i = startIndex; i < endIndex; ++i)
	{
		BuffDesc desc(buffList[i][0], buffList[i][1], m_pBuff->GetSkillID());
		desc.type = m_pBuff->GetType();
		args.vecBuffs.push_back(desc);
		//args.BuffIDs.push_back(buffList[i][0]);
		//args.BuffLevels.push_back(buffList[i][1]);
	}
	m_pTarget->DispatchAction(&args);
}

void XBuffTrigger::CastTriggerSkill()
{
	if(m_triggerSkill == 0)
		return;

	XAttackActionArgs attck_arg;
	attck_arg.Identify = m_triggerSkill;
	attck_arg.Target = m_entity->AIComponent() == NULL ? 0 : m_entity->AIComponent()->GetTarget();
	m_entity->DispatchAction(&attck_arg);
}

float XBuffTrigger::_GetTriggerParam(const BuffTable::RowData* buffInfo, int index)
{
	if (buffInfo == NULL || (int)buffInfo->BuffTriggerParam.size() <= index)
		return 0;
	return convert<float>(buffInfo->BuffTriggerParam[index]);
}

int XBuffTrigger::_GetTriggerParamInt(const BuffTable::RowData* buffInfo, int index)
{
	if (buffInfo == NULL || (int)buffInfo->BuffTriggerParam.size() <= index)
		return 0;
	return convert<int>(buffInfo->BuffTriggerParam[index]);
}

string XBuffTrigger::_GetTriggerParamStr(const BuffTable::RowData* buffInfo, int index)
{
	if (buffInfo == NULL || (int)buffInfo->BuffTriggerParam.size() <= index)
		return "";
	return buffInfo->BuffTriggerParam[index];
}

void XBuffTrigger::OnUpdate()
{
	if (m_bSkillTriggered)
	{
		m_bSkillTriggered = false;
		CastTriggerSkill();
	}
}


XTriggerCondition::XTriggerCondition(BuffTable::RowData* pInfo)
{
	m_lastTriggerTime = 0;
	m_triggerCD = (UINT64)std::abs((pInfo->BuffTriggerCD * 1000));
	m_triggerCount = pInfo->BuffTriggerCount;
	m_bCDWhenRandFail = pInfo->BuffTriggerCD < 0;
	if(m_triggerCount == 0)
		m_triggerCount = ~0;
}

bool XTriggerCondition::_IsTriggerBuffCD()
{
	return TimeUtil::GetMilliSecond() - m_lastTriggerTime > m_triggerCD;
}

bool XTriggerCondition::_HasTriggerCount()
{
	return m_triggerCount == ~0 || m_triggerCount > 0;
}

bool XTriggerCondition::CanTrigger()
{
	return (_IsTriggerBuffCD() && _HasTriggerCount());
}

void XTriggerCondition::OnTrigger()
{
	m_lastTriggerTime = TimeUtil::GetMilliSecond();

	if (m_triggerCount != ~0 && m_triggerCount > 0)
		--m_triggerCount;
}

void XTriggerCondition::OnRandFail()
{
	if (m_bCDWhenRandFail)
		m_lastTriggerTime = TimeUtil::GetMilliSecond();
}
