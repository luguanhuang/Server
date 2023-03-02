#include "pch.h"
#include "XBuffRegenerate.h"
#include "unit/unit.h"
#include "entity/XEntity.h"
#include "unit/combatattribute.h"
#include "unit/attributewatcher.h"
#include "foreach.h"
#include "unit/rolemanager.h"
#include "unit/enemymanager.h"
#include "XBuffSpecialState.h"
#include "gamelogic/killerwatcher.h"
#include "gamelogic/XCombat.h"
#include "globalconfig.h"
#include <cmath>

bool XBuffRegenerate::TryCreate(CombatEffectHelper* pEffectHelper, XBuff *pBuff)
{
	if (pEffectHelper->GetBuffData()->BuffDOT.empty() && !pEffectHelper->HasEffect(CET_Buff_DOTorHOT))
		return false;

	pBuff->AddBuffEffect(new XBuffRegenerate(pBuff));
	return true;
}

XBuffRegenerate::XBuffRegenerate(XBuff *pBuff)
{
	start_time = 0;
	m_pBuff = pBuff;
	m_TimerToken = INVALID_HTIMER;
}

XBuffRegenerate::~XBuffRegenerate()
{
	_KillTimer();
}

double _GetCasterAttr(Unit* pCaster, int attrID, int attrValue)
{
	if (pCaster != NULL)
		return pCaster->GetAttr((CombatAttrDef)attrID) * attrValue / 100;
	return 0.0;
}

void XBuffRegenerate::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper)
{
	//RegenBuffOP op;
	//op.buffPtr = this;
	//op.op = OPADD;
	//gBuffQueue.push_back(op);

	Unit* pUnit = NULL;

	const std::vector<Sequence<float, 3>>* dotData = NULL;
	const std::vector<Sequence<int, 2>>* dotFromCasterData = NULL;

	std::vector<Sequence<float, 3>> allDots;
	std::vector<Sequence<int, 2>> allFromCasters;

	if(pEffectHelper->HasEffect(CET_Buff_DOTorHOT))
	{
		for (int i = 0; i < m_pBuff->GetBuffInfo()->BuffDOT.size(); ++i)
		{
			allDots.push_back(m_pBuff->GetBuffInfo()->BuffDOT[i]);
		}
		for (int i = 0; i < m_pBuff->GetBuffInfo()->BuffDOTValueFromCaster.size(); ++i)
		{
			allFromCasters.push_back(m_pBuff->GetBuffInfo()->BuffDOTValueFromCaster[i]);
		}

		if(pEffectHelper->GetBuffRegenerate(allDots, allFromCasters))
		{
			dotData = &allDots;
			dotFromCasterData = &allFromCasters;
		}
		else
		{
			allDots.clear();
			allFromCasters.clear();

			dotData = &m_pBuff->GetBuffInfo()->BuffDOT;
			dotFromCasterData = &m_pBuff->GetBuffInfo()->BuffDOTValueFromCaster;
		}
	}
	else
	{
		dotData = &m_pBuff->GetBuffInfo()->BuffDOT;
		dotFromCasterData = &m_pBuff->GetBuffInfo()->BuffDOTValueFromCaster;
	}

	unsigned n = dotData->size();

	if(dotFromCasterData->size() == n)
	{
		pUnit = Unit::FindUnit(m_pBuff->GetCaster());
		if (!allDots.empty())
		{
			for(unsigned i = 0; i < n; ++i)
			{
				if (allFromCasters[i][0] != 0)
					allDots[i][1] = (float)_GetCasterAttr(pUnit, allFromCasters[i][0], allFromCasters[i][1]);
			}
		}
	}

	if (!allDots.empty())
	{
		pEffectHelper->Merge(allDots, 1);
		n = dotData->size();
	}

	UINT32 nextTime = ~0;

	for (unsigned i = 0; i < n; ++i)
	{
		m_dots.push_back(DotTimeInfo());

		DotTimeInfo &info = m_dots.back();
		info.attrID = (int)(*dotData)[i][0];
		info.attrValue = (*dotData)[i][1];

		info.interval = (int)std::max((GlobalConfig::GetInstance().BuffMinRegenerateInterval * 1000.0f), (*dotData)[i][2]);
		info.tickCount = 0;
		info.totalCount = (int)(m_pBuff->GetBuffInfo()->BuffDuration * 1000/(*dotData)[i][2]);
		info.nextTime = TimeUtil::GetMilliSecond() + info.interval;

		if (pUnit != NULL && allDots.empty())
		{
			if ((*dotFromCasterData)[i][0] != 0)
			{
				info.attrValue = _GetCasterAttr(pUnit, (*dotFromCasterData)[i][0] , (*dotFromCasterData)[i][1]);
			}
		}

		ConvertSpecialBuff(info);

		nextTime = std::min((UINT32)info.interval, nextTime);
	}
	start_time = TimeUtil::GetMilliSecond();
	m_TimerToken = XCommon::SetTimer(nextTime * 0.001f, DelegateCombination(XBuffRegenerate, OnTimer, this), __FILE__, __LINE__);
}

void XBuffRegenerate::OnRemove(XBuffRemoveReason reason)
{
	_KillTimer();
	//RegenBuffOP op;
	//op.buffPtr = this;
	//op.op = OPREM;
	//gBuffQueue.push_back(op);
	Unit *pUnit = m_pBuff->GetEntity()->GetUnit();
	if (pUnit->IsDead())
		return;

	bool bNotifyClient = (m_pBuff->GetBuffInfo()->DontShowText == false);
	for (auto i = m_dots.begin(); i != m_dots.end(); ++i)
	{
		DotTimeInfo &info = *i;

		if (start_time > 0)
		{
			int count = (int)((TimeUtil::GetMilliSecond() - start_time)/info.interval);
			for (int j = info.tickCount; j < count && j < info.totalCount; ++j)
			{

				AttributeWatcher watcher(pUnit);
				watcher.AddWatchedAttribute((CombatAttrDef)info.attrID);

				if((CombatAttrDef)info.attrID == BASIC_CurrentHP)
				{
					double damage = info.attrValue;
					if (damage < 0)
					{
						///> ¿ÛÑª
						XCombat::ProjectExternalDamage(-damage, m_pBuff->GetCaster(), m_pBuff->GetEntity());
					}
					else
					{
						if (!pUnit->IsBlockAddHP())
						{
							///> ¼ÓÑª
							pUnit->AddAttr((CombatAttrDef)info.attrID, damage);
							pUnit->CheckAttrState();

							if (m_pBuff->GetType() != 1)
							{	
								Unit* pCaster = Unit::FindUnit(m_pBuff->GetCaster());
								if(pCaster != NULL)
								{	
									pCaster->OnTreat(pUnit, damage);
								}
							}
						}
					}
				}
				else
				{
					pUnit->AddAttr((CombatAttrDef)info.attrID, info.attrValue);
					pUnit->CheckAttrState();
				}

				bool bNotifyClient = (m_pBuff->GetBuffInfo()->DontShowText == false);
				watcher.CheckChangedAttributeAndNotify(true, bNotifyClient, m_pBuff->GetCaster());
				//pUnit->CheckAttrState();
			}
		}
	}
}

//void XBuffRegenerate::OnUpdate()
bool XBuffRegenerate::OnTimer(IArgs*, void*)
{
	m_TimerToken = INVALID_HTIMER;

	bool changed = false;
	Unit *pUnit = m_pBuff->GetEntity()->GetUnit();
	UINT64 now = TimeUtil::GetMilliSecond();

	AttributeWatcher watcher(pUnit);

	UINT32 nextTime = ~0;

	for (auto i = m_dots.begin(); i != m_dots.end(); ++i)
	{
		DotTimeInfo &info = *i;

		watcher.AddWatchedAttribute((CombatAttrDef)info.attrID);

		while (info.nextTime <= now && info.tickCount < info.totalCount)
		{

			info.tickCount += 1;
			info.nextTime += info.interval;

			changed = true;

			if((CombatAttrDef)info.attrID == BASIC_CurrentHP)
			{
				double damage = info.attrValue;
				if (damage < 0)
				{
					///> ¿ÛÑª
					XCombat::ProjectExternalDamage(-damage, m_pBuff->GetCaster(), m_pBuff->GetEntity());
				}
				else
				{
					if (!pUnit->IsBlockAddHP())
					{
						///> ¼ÓÑª
						pUnit->AddAttr((CombatAttrDef)info.attrID, damage);
						pUnit->CheckAttrState();

						if (m_pBuff->GetType() != 1)
						{	
							Unit* pCaster = Unit::FindUnit(m_pBuff->GetCaster());
							if(pCaster != NULL)
							{	
								pCaster->OnTreat(pUnit, damage);
							}
						}
					}
				}
			}
			else
			{
				pUnit->AddAttr((CombatAttrDef)info.attrID, info.attrValue);
				pUnit->CheckAttrState();
			}

			//watcher.AddAttr((CombatAttrDef)info.attrID, info.attrValue);
		}

		if(info.nextTime > now)
		{
			nextTime = std::min(nextTime, (UINT32)(info.nextTime - now));
		}
	}
	
	if (changed)
	{
		bool bNotifyClient = (m_pBuff->GetBuffInfo()->DontShowText == false);
		watcher.CheckChangedAttributeAndNotify(true, bNotifyClient, m_pBuff->GetCaster());
	}

	if(GetValid() && nextTime != ~0 && nextTime > 0)
	{
		m_TimerToken = XCommon::SetTimer(nextTime * 0.001f, DelegateCombination(XBuffRegenerate, OnTimer, this), __FILE__, __LINE__);
	}

	return true;
}

void XBuffRegenerate::ConvertSpecialBuff(DotTimeInfo &info)
{
	if (info.attrID == PERCENT_MaxHP)
	{
		info.attrID = BASIC_CurrentHP;
		//if(!info.fromCaster)
		{
			Unit *pUnit = m_pBuff->GetEntity()->GetUnit();
			info.attrValue *= pUnit->GetAttr(TOTAL_MaxHP)/100.0;
		}
	}
}

void XBuffRegenerate::_KillTimer()
{
	if (m_TimerToken != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_TimerToken);
		m_TimerToken = INVALID_HTIMER;
	}
}

void XBuffRegenerate::OnResetTime()
{
	UINT32 nextTime = ~0;
	UINT64 now = TimeUtil::GetMilliSecond();

	for (auto it = m_dots.begin(); it != m_dots.end(); ++it)
	{
		it->tickCount = 0;
		if(m_TimerToken == INVALID_HTIMER && it->nextTime > now)
		{
			nextTime = std::min(nextTime, (UINT32)(it->nextTime - now));
		}
	}

	if(m_TimerToken == INVALID_HTIMER && GetValid() && nextTime != ~0 && nextTime > 0)
	{
		m_TimerToken = XCommon::SetTimer(nextTime * 0.001f, DelegateCombination(XBuffRegenerate, OnTimer, this), __FILE__, __LINE__);
	}

}
