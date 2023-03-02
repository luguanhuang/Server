#include "pch.h"
#include "securitystatistics.h"
#include "entity/XEntity.h"
#include "securityattributestatistics.h"
#include "securityskillstatistics.h"
#include "securitybuffstatistics.h"
#include "securitydamagestatistics.h"
#include "unit/enemy.h"
#include "gamelogic/XCombat.h"
#include "unit/unit.h"
#include "scene.h"
#include "securityscenestatistics.h"
#include "securityaistatistics.h"
#include "scenestatistics.h"
#include "unit/role.h"
#include "securitymobstatistics.h"


void XSecurityStatistics::OnAttach()
{
	if (m_pUnit != NULL)
	{
		if (m_pUnit->IsRole())
		{
			if (_AttributeInfo == NULL)
			{
				_AttributeInfo = new XSecurityAttributeStatistics();
				_AttributeInfo->Reset();
			}
			if (_SkillInfo == NULL)
			{
				_SkillInfo = new XSecuritySkillStatistics();
				_SkillInfo->Reset();
			}
			if (_BuffInfo == NULL)
			{
				_BuffInfo = new XSecurityBuffStatistics();
				_BuffInfo->Reset();
			}
			if (_MobInfo == NULL)
			{
				_MobInfo = new XSecurityMobStatistics();
				_MobInfo->Reset();
			}
			_SkillInfo->OnAttach(m_pUnit);
		}
		else if(m_pUnit->IsEnemy())
		{
			if (_AIInfo == NULL)
			{	
				_AIInfo = new XSecurityAIStatistics();
				_AIInfo->Reset();
			}

		}
		if (_DamageInfo == NULL)
		{
			_DamageInfo = new XSecurityDamageStatistics();
			_DamageInfo->Reset();
		}
	}
}

void XSecurityStatistics::Dump(XSecuritySceneStatistics* pSceneStatistics)
{
	if (m_pUnit == NULL)
		return;

	if (!_bInited)
		return;

	if(m_pUnit->IsRole())
		return;

	OnEnd();

	if(m_pUnit->IsEnemy() && pSceneStatistics != NULL)
	{
		MergeToScene(pSceneStatistics);
	}
}

void XSecurityStatistics::MergeToScene(XSecuritySceneStatistics* sss)
{
	if (!m_pUnit->IsEnemy())
		return;
	Enemy* pEnemy = static_cast<Enemy*>(m_pUnit);

	if(pEnemy->IsBoss())
	{
		sss->_BossDamageInfo->Merge(*_DamageInfo);
		sss->_BossAIInfo->Merge(*_AIInfo);
		sss->_BossHPInfo->Merge(_InitHp);

		sss->_BossMoveDistance += _Distance;
	}
	else if(pEnemy->GetFightGroup() != KKSG::FightRole && pEnemy->GetFightGroup() != KKSG::FightNeutral)
	{
		sss->_MonsterDamageInfo->Merge(*_DamageInfo);
		sss->_MonsterAIInfo->Merge(*_AIInfo);
		sss->_MonsterHPInfo->Merge(_InitHp);

		sss->_MonsterMoveDistance += _Distance;
	}
}

void XSecurityStatistics::Reset()
{
	if (_DamageInfo != NULL)
		_DamageInfo->Reset();

	if (_AttributeInfo != NULL)
		_AttributeInfo->Reset();

	if (_SkillInfo != NULL)
		_SkillInfo->Reset();

	if (_BuffInfo != NULL)
		_BuffInfo->Reset();

	if (_AIInfo != NULL)
		_AIInfo->Reset();

	if (_MobInfo != NULL)
		_MobInfo->Reset();

	_InitHp = -1;
	_InitMp = -1;
	_FinalHp = 0;
	_FinalMp = 0;

	_Distance = 0;

	_bInited = true;
}

void XSecurityStatistics::OnStart()
{
	Reset();

	OnAttach();

	if (m_pUnit != NULL)
	{
		_InitHp = (float)m_pUnit->GetAttr(BASIC_CurrentHP);
		_InitMp = (float)m_pUnit->GetAttr(BASIC_CurrentMP);
	}
}

void XSecurityStatistics::OnEnd()
{
	if (!_bInited)
		return;

	if(m_pUnit != NULL)
	{
		_FinalHp = (float)m_pUnit->GetAttr(BASIC_CurrentHP);
		_FinalMp = (float)(float)m_pUnit->GetAttr(BASIC_CurrentMP);

		if (_SkillInfo != NULL)
			_SkillInfo->OnEnd(m_pUnit);

		///> 召唤物的信息 加到主人上
		if (m_pUnit->IsEnemy() && m_pUnit->GetFinalHostId() != 0)
		{
			Unit* pHost = m_pUnit->GetFinalHostUnit();
			XSecurityMobStatistics* pMobInfo = XSecurityMobStatistics::TryGetStatistics(pHost);
			if (pMobInfo != NULL)
			{
				pMobInfo->Append(m_pUnit);
			}
		}
	}

	_bInited = false;
}

void XSecurityStatistics::OnCastDamage(const HurtInfo& rawInput, const ProjectDamageResult& result)
{
	if (!result.m_Accept)
		return;

	if (NULL != _DamageInfo)
	{
		_DamageInfo->OnCastDamage(rawInput, result);
	}
	if (_SkillInfo != NULL)
		_SkillInfo->OnCastDamage(rawInput, result);
}

void XSecurityStatistics::OnReceiveDamage(const HurtInfo& rawInput, const ProjectDamageResult& result)
{
	if (!result.m_Accept)
		return;

	if (NULL != _DamageInfo)
	{
		_DamageInfo->OnReceiveDamage(rawInput, result);
	}
}

void XSecurityStatistics::OnAttributeChange(CombatAttrDef attr, double delta)
{
	if (m_pUnit && m_pUnit->IsBlockAddHP())
		return;

	if (delta < 0.00001 && delta > -0.0001) 
		return;
	if (_AttributeInfo != NULL && _AttributeInfo->IsUsefulAttr(attr))
		_AttributeInfo->OnAttributeChange(attr, delta);
}

bool XSecurityStatistics::IsUsefulAttr(CombatAttrDef attr)
{
	return ((_AttributeInfo != NULL && _AttributeInfo->IsUsefulAttr(attr)));
}

void XSecurityStatistics::OnMove(float distance)
{
	_Distance += distance;
}

XSecurityStatistics::XSecurityStatistics(Unit* pUnit)
{
	m_pUnit = pUnit;
	_DamageInfo = NULL;
	_AttributeInfo = NULL;
	_BuffInfo = NULL;
	_SkillInfo = NULL;
	_AIInfo = NULL;
	_MobInfo = NULL;
}

XSecurityStatistics::~XSecurityStatistics()
{
	if (_DamageInfo != NULL)
		delete _DamageInfo;
	if (_AttributeInfo != NULL)
		delete _AttributeInfo;
	if (_BuffInfo != NULL)
		delete _BuffInfo;
	if (_SkillInfo != NULL)
		delete _SkillInfo;
	if (_AIInfo != NULL)
		delete _AIInfo;
	if (_MobInfo != NULL)
		delete _MobInfo;
}

void XSecurityStatistics::Append(const string& key, const string& value, std::map<string, string>& target)
{
	target[key] = value;
}

void XSecurityStatistics::Append(const string& key, double value, std::map<string, string>& target)
{
	Append(key, ToString((INT64)value), target);
}

void XSecurityStatistics::Append(const string& key, int value, std::map<string, string>& target)
{
	Append(key, ToString(value), target);
}

void XSecurityStatistics::SendRoleData(Scene* pScene, RoleBattleRecord* pBattleRecord)
{
	if (m_pUnit == NULL || !m_pUnit->IsRole())
		return;

	if (!_bInited)
		return;

	XBattleEndArgs endArg;
	m_pUnit->GetXObject()->DispatchAction(&endArg);

	OnEnd();

	Append("PlayerInitHP", (double)_InitHp, pBattleRecord->end.m_data);
	Append("PlayerInitMP", (double)_InitMp, pBattleRecord->end.m_data);
	Append("PlayerEndHP", (double)_FinalHp, pBattleRecord->end.m_data);
	Append("PlayerEndMP", (double)_FinalMp, pBattleRecord->end.m_data);
	Append("MoveTotal", (double)_Distance, pBattleRecord->end.m_data);

	if (_DamageInfo != NULL)
		_DamageInfo->SendRoleData(pBattleRecord->end);

	if (_SkillInfo != NULL)
		_SkillInfo->SendRoleData(static_cast<Role*>(m_pUnit), pBattleRecord->end);

	if (_BuffInfo != NULL)
		_BuffInfo->SendRoleData(pBattleRecord->endcount);

	if (_AttributeInfo != NULL)
		_AttributeInfo->SendData(pBattleRecord->end);

	if (pScene->Statistics() == NULL)
		return;
	XSecuritySceneStatistics* pSecuritySceneStatistics = pScene->Statistics()->GetSecurityStatistics();
	if(pSecuritySceneStatistics != NULL)
	{
		pSecuritySceneStatistics->SendCurrentData(pScene, pBattleRecord);
	}
}
