#include "pch.h"
#include "XBuff.h"
#include "component/XBuffComponent.h"
#include "component/XComponent.h"
#include "entity/XEntity.h"
#include "XBuffChangeAttribute.h"
#include "XBuffSpecialState.h"
#include "XBuffRegenerate.h"
#include "XBuffLifeAddAttack.h"
#include "XBuffAuraCheck.h"
#include "XBuffReduceDamage.h"
#include "foreach.h"
#include "XBuffTrigger.h"
#include "XBuffClear.h"
#include "XBuffDamageReflection.h"
#include "XBuffMob.h"
#include "XBuffChangeFightGroup.h"
#include "XBuffLifeSteal.h"
#include "XBuffReduceSkillCD.h"
#include "util/XCommon.h"
#include "XBuffTargetLifeAddAttack.h"
#include "scene/securitybuffstatistics.h"
#include "gamelogic/combateffect.h"
#include "XBuffManipulate.h"
#include "XBuffSkillsReplace.h"
#include "XBuffDamageDistanceScale.h"
#include "XBuffKill.h"

int BuffEffectCompare( const void *pFirst, const void *pSecond)
{
	BuffEffect* pA = *(( BuffEffect **)pFirst);

	BuffEffect* pB = *(( BuffEffect **)pSecond);

	if(pA == NULL || pB == NULL)
	{
		if(pA == NULL && pB == NULL)
			return 0;
		if(pA == NULL)
			return 1;
		return -1;
	}

	XBuffEffectPrioriy prioA = pA->GetPriority();
	XBuffEffectPrioriy prioB = pB->GetPriority();
	return prioA - prioB;

}

XBuffEffectPrioriy XBuff::EffectEnumeratorPriorityCur = XBuffEffectPrioriy_START;
XBuffEffectPrioriy XBuff::EffectEnumeratorPriorityNext = XBuffEffectPrioriy_END;

XBuff::XBuff()
{
	m_valid = true;
	m_BuffInfo = NULL;
	m_Entity = NULL;
	m_TimeDuration = 0;
	m_OriginalDuration = 0;
	m_TimerToken = INVALID_HTIMER;
	m_Caster = 0;
	m_HP = 100;
	m_MaxHP = 100;
	m_StackCount = 1;
	m_Type = 0;
	memset(m_BuffEffect, 0, sizeof(m_BuffEffect));
	m_RelevantSkills = NULL;
	m_bMustNotify = false;
}

XBuff::~XBuff()
{
	if (m_RelevantSkills != NULL)
		delete m_RelevantSkills;
}


void XBuff::AddBuffTime(UINT32 extendTime)
{
	if (m_TimerToken == INVALID_HTIMER)
		return;
	UINT32 leftTime = CTimerMgr::Instance()->GetTimeLeft(m_TimerToken);
	m_TimeDuration = leftTime + extendTime;

	CTimerMgr::Instance()->KillTimer(m_TimerToken);
	XDelegate d = XDelegate::registerMethod<XBuff, &XBuff::OnBuffTimeEnd>(this);
	m_TimerToken = CTimerMgr::Instance()->SetTimer(d, NULL, NULL, m_TimeDuration, 1, __FILE__, __LINE__);
}


bool XBuff::ResetBuffTime()
{
	if (m_TimerToken == INVALID_HTIMER)
		return false;

	CTimerMgr::Instance()->KillTimer(m_TimerToken);
	if (m_TimeDuration > 0)
	{
		XDelegate d = XDelegate::registerMethod<XBuff, &XBuff::OnBuffTimeEnd>(this);
		m_TimerToken = CTimerMgr::Instance()->SetTimer(d, NULL, NULL, m_TimeDuration, 1, __FILE__, __LINE__);
	}

	OnResetTime();
	return true;
}

void XBuff::OnResetTime()
{
	for (int i = 0; i < MAX_BUFFEFFECT_COUNT; ++i)
	{
		if (m_BuffEffect[i] == NULL)
		{
			break;
		}

		m_BuffEffect[i]->OnResetTime();
	}
}

void XBuff::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHeler)
{
	m_Entity = pEntity;
	if (m_TimeDuration > 0)
	{
		XDelegate d = XDelegate::registerMethod<XBuff, &XBuff::OnBuffTimeEnd>(this);
		m_TimerToken = CTimerMgr::Instance()->SetTimer(d, NULL, NULL, m_TimeDuration, 1, __FILE__, __LINE__);
	}

	_AddBuffEffect(pEffectHeler);
	_SortEffect();

	for (int i = 0; i < MAX_BUFFEFFECT_COUNT; ++i)
	{
		if (m_BuffEffect[i] != NULL)
		{
			m_BuffEffect[i]->SetValid(true);
			m_BuffEffect[i]->OnAdd(pEntity, pEffectHeler);
		}
		else
		{
			break;
		}
	}
	_InitHP(pEffectHeler);
}

bool XBuff::Append(CombatEffectHelper* pEffectHeler)
{
	ResetBuffTime();

	m_HP = m_MaxHP;

	if(m_StackCount >= m_BuffInfo->StackMaxCount)
		return false;

	int i = 0;
	for (; i < MAX_BUFFEFFECT_COUNT; ++i)
	{
		if (m_BuffEffect[i] == NULL)
		{
			break;
		}
	}

	_AddBuffEffect(pEffectHeler, false);
	for (; i < MAX_BUFFEFFECT_COUNT; ++i)
	{
		if (m_BuffEffect[i] != NULL)
		{
			m_BuffEffect[i]->SetValid(true);
			m_BuffEffect[i]->OnAdd(m_Entity, pEffectHeler);
		}
		else
		{
			break;
		}
	}
	_SortEffect();
	++m_StackCount;
	return true;
}

void XBuff::OnAppend()
{
	for (int i = 0; i < MAX_BUFFEFFECT_COUNT; ++i)
	{
		if (m_BuffEffect[i] != NULL)
		{
			m_BuffEffect[i]->OnAppend(m_Entity);
		}
		else
		{
			break;
		}
	}
}

void XBuff::OnRemove(XBuffRemoveReason reason)
{
	if (m_valid)
	{
		m_valid = false;

		if (m_TimerToken != INVALID_HTIMER)
		{
			CTimerMgr::Instance()->KillTimer(m_TimerToken);
		}

		for (int i = 0; i < MAX_BUFFEFFECT_COUNT; ++i)
		{
			if (m_BuffEffect[i] != NULL)
			{
				m_BuffEffect[i]->SetValid(false);
				m_BuffEffect[i]->OnRemove(reason);
				delete m_BuffEffect[i];
				m_BuffEffect[i] = NULL;
			}
			else
			{
				break;
			}
		}

		m_SkillModify.OnRemove(this);
	}
}

bool XBuff::OnBuffTimeEnd(IArgs *args, void *parg)
{
	m_TimerToken = INVALID_HTIMER;
	XBuffComponent *pComponent = m_Entity->BuffComponent();
	pComponent->RemoveBuff(this, XBuffRemoveReason_Timeout, true);
	return true;
}


void XBuff::Init(const BuffDesc& buffDesc, CombatEffectHelper* pEffectHelper)
{
	m_BuffInfo = const_cast<BuffTable::RowData*>(pEffectHelper->GetBuffData());
	// 配置的是秒，转换成毫秒
	if(buffDesc.effecttime == (UINT32)(-1))
		m_OriginalDuration = (UINT32)(m_BuffInfo->BuffDuration > 0 ? m_BuffInfo->BuffDuration * 1000 : 0);
	else
		m_OriginalDuration = buffDesc.effecttime * 1000;
	
	///> 加上神器的延时效果
	m_OriginalDuration += pEffectHelper->GetSum(CET_Buff_Duration);

	m_TimeDuration = m_OriginalDuration;

	m_SkillID = buffDesc.skillID;
	m_Type = buffDesc.type;

	m_StartTime = TimeUtil::GetMilliSecond();

	if (m_BuffInfo->RelevantSkills.size() > 0)
	{
		if (m_RelevantSkills == NULL)
			m_RelevantSkills = new std::unordered_set<UINT32>();

		for (auto it = m_BuffInfo->RelevantSkills.begin(); it != m_BuffInfo->RelevantSkills.end(); ++it)
		{
			m_RelevantSkills->insert(XCommon::XHash(it->c_str()));
		}
	}

	m_Exclusive.Set(m_BuffInfo);

	m_bMustNotify = 
		m_BuffInfo->BuffFx.size() != 0 
		|| m_BuffInfo->BuffEffectFx.size() != 0 
		|| m_BuffInfo->BuffSpriteFx.size() != 0 
		|| m_BuffInfo->BuffIsVisible 
		|| HasTag(XBuffTag_SpriteEffect);
}

void XBuff::_AddBuffEffect(CombatEffectHelper* pEffectHeler, bool bIncludeTrigger)
{
	XBuffChangeAttribute::TryCreate(pEffectHeler, this);
	m_bMustNotify |= XBuffSpecialState::TryCreate(pEffectHeler->GetBuffData(), this);
	XBuffRegenerate::TryCreate(pEffectHeler, this);
	XBuffLifeAddAttack::TryCreate(pEffectHeler->GetBuffData(), this);
	XBuffDamageDistanceScale::TryCreate(pEffectHeler->GetBuffData(), this);
	XBuffAuraCheck::TryCreate(pEffectHeler->GetBuffData(), this);
	XBuffReduceDamage::TryCreate(pEffectHeler->GetBuffData(), this);
	//XBuffClear::TryCreate(pEffectHeler->GetBuffData(), this);
	XBuffDamageReflection::TryCreate(pEffectHeler->GetBuffData(), this);
	m_bMustNotify |= XBuffMob::TryCreate(pEffectHeler->GetBuffData(), this);
	XBuffChangeFightGroup::TryCreate(pEffectHeler->GetBuffData(), this);
	XBuffLifeSteal::TryCreate(pEffectHeler->GetBuffData(), this);
	m_bMustNotify |= XBuffReduceSkillCD::TryCreate(pEffectHeler, this);
	XBuffTargetLifeAddAttack::TryCreate(pEffectHeler->GetBuffData(), this);
	XBuffManipulate::TryCreate(pEffectHeler->GetBuffData(), this);
	m_bMustNotify |= XBuffSkillsReplace::TryCreate(pEffectHeler->GetBuffData(), this);
	XBuffKill::TryCreate(pEffectHeler->GetBuffData(), this);
	m_SkillModify.TryCreate(pEffectHeler, this);

	if (bIncludeTrigger)
		XBuffTrigger::TryCreate(pEffectHeler->GetBuffData(), this);
}

bool XBuff::OnBuffEffect(const HurtInfo &hurtInfo, ProjectDamageResult &result, BuffEffectCB func)
{
	if (EffectEnumeratorPriorityCur == XBuffEffectPrioriy_START)
		m_EffectEnumeratorIndex = 0;

	for (; m_EffectEnumeratorIndex < MAX_BUFFEFFECT_COUNT; ++m_EffectEnumeratorIndex)
	{
		if (m_BuffEffect[m_EffectEnumeratorIndex] != NULL)
		{
			XBuffEffectPrioriy curPrio = m_BuffEffect[m_EffectEnumeratorIndex]->GetPriority();
			if (curPrio > EffectEnumeratorPriorityCur)
			{
				if (curPrio < EffectEnumeratorPriorityNext)
					EffectEnumeratorPriorityNext = curPrio;
				break;
			}
			else
			{
				func(m_BuffEffect[m_EffectEnumeratorIndex], hurtInfo, result);
			}
		}
		else
		{
			break;
		}
	}

	return true;
}


void XBuff::OnHurt(BuffEffect* pBuffEffect, const HurtInfo& rHurtInfo, ProjectDamageResult& rResult)
{
	pBuffEffect->OnBuffEffect(rHurtInfo, rResult);
}

void XBuff::OnCastDamage(BuffEffect* pBuffEffect, const HurtInfo& rHurtInfo, ProjectDamageResult& rResult)
{
	pBuffEffect->OnCastDamage(rHurtInfo, rResult);
}

bool XBuff::OnCastSkill(const HurtInfo &hurtInfo)
{
	for (int i = 0; i < MAX_BUFFEFFECT_COUNT; ++i)
	{
		if (m_BuffEffect[i] != NULL)
		{
			m_BuffEffect[i]->OnCastSkill(hurtInfo);
		}
		else
		{
			break;
		}
	}
	return true;
}

bool XBuff::OnComboChange(UINT32 comboCount)
{
	for (int i = 0; i < MAX_BUFFEFFECT_COUNT; ++i)
	{
		if (m_BuffEffect[i] != NULL)
		{
			m_BuffEffect[i]->OnComboChange(comboCount);
		}
		else
		{
			break;
		}
	}
	return true;
}

bool XBuff::CanBuffAdd(BuffTable::RowData * pBuffData)
{
	return m_Exclusive.CanAdd(pBuffData->BuffClearType);
}

bool XBuff::CanBuffAdd(XEntity* pEntity, const BuffTable::RowData* pRowData)
{
	bool bRes = true;
	bRes &= XBuffSpecialState::CanBuffAdd(pEntity, pRowData);

	return bRes;
}

void XBuff::AddBuffEffect(BuffEffect *eff)
{
	if (eff == NULL)
		return;

	for (int i = 0; i < MAX_BUFFEFFECT_COUNT; ++i)
	{
		if (m_BuffEffect[i] == NULL)
		{
			m_BuffEffect[i] = eff;
			return;
		}
	}

	LogWarn("buff effect slot is full!");
}

void XBuff::OnAttributeChanged(CombatAttrDef attr)
{
	for (int i = 0; i < MAX_BUFFEFFECT_COUNT; ++i)
	{
		if (m_BuffEffect[i] != NULL)
		{
			m_BuffEffect[i]->OnAttributeChanged(attr);
		}
		else
		{
			break;
		}
	}
}

void XBuff::OnQTEStateChanged(const XOnQTEOnEventArgs* arg)
{
	for (int i = 0; i < MAX_BUFFEFFECT_COUNT; ++i)
	{
		if (m_BuffEffect[i] != NULL)
		{
			m_BuffEffect[i]->OnQTEStateChanged(arg);
		}
		else
		{
			break;
		}
	}
}
void XBuff::OnRealDead(const XRealDeadActionArgs *Args)
{
	for (int i = 0; i < MAX_BUFFEFFECT_COUNT; ++i)
	{
		if (m_BuffEffect[i] != NULL)
		{
			m_BuffEffect[i]->OnRealDead(Args);
		}
		else
		{
			break;
		}
	}
}

//double XBuff::ModifySkillCoolDown()
//{
//	return m_SkillModify.CoolDownModify;
//}

double XBuff::ModifySkillCost()
{
	return m_SkillModify.CostModify;
}

double XBuff::ModifySkillCastDamage()
{
	return m_SkillModify.CastDamageModify;
}

double XBuff::ModifyIncReceivedDamage()
{
	return m_SkillModify.IncReceivedDamageModify;
}
double XBuff::ModifyDecReceivedDamage()
{
	return m_SkillModify.DecReceivedDamageModify;
}
double XBuff::ChangeSkillDamage(UINT32 skillID)
{
	return m_SkillModify.GetChangeSkillDamage(skillID);
}

void XBuff::SetCaster(UINT64 caster)
{
	m_Caster = caster;
}

void XBuff::OnUpdate()
{
	for(int i = 0; i < MAX_BUFFEFFECT_COUNT; ++i)
	{
		if(m_BuffEffect[i] == NULL)
			break;

		m_BuffEffect[i]->OnUpdate();
	}
}

UINT32 XBuff::GetLeftTime() const
{
	return CTimerMgr::Instance()->GetTimeLeft(m_TimerToken);
}

void XBuff::_AppendHP(const Sequence<float, 2>& seq)
{
	if (seq[1] != 0.0f)
	{
		int attr = (int)seq[0];
		if (attr == 0)
		{
			m_MaxHP += seq[1];
		}
		else
		{
			Unit* pUnit = Unit::FindUnit(m_Caster);
			if (pUnit != NULL && !pUnit->IsDead())
			{
				m_MaxHP += pUnit->GetAttr((CombatAttrDef)attr) * seq[1] / 100;
			}
		}
	}
}

void XBuff::_InitHP(CombatEffectHelper* pEffectHeler)
{
	m_MaxHP = 0;
	_AppendHP(m_BuffInfo->BuffHP);
	if (pEffectHeler->HasEffect(CET_Buff_HP))
	{
		std::vector<Sequence<float, 2>> hpList;
		pEffectHeler->GetBuffHP(hpList);
		for (auto it = hpList.begin(); it != hpList.end(); ++it)
		{
			_AppendHP(*it);
		}
	}

	if (m_MaxHP == 0)
		m_MaxHP = 100;
	m_HP = m_MaxHP;
}

double XBuff::ChangeBuffHP(double deltaHP)
{
	m_HP += deltaHP;
	if (m_HP <= 0)
	{
		if(IsValid())
		{
			deltaHP -= m_HP;
			XBuffComponent *pComponent = m_Entity->BuffComponent();
			pComponent->RemoveBuff(this, XBuffRemoveReason_Timeout, true);
		}
	}
	else
	{
		XBuffComponent *pComponent = m_Entity->BuffComponent();
		pComponent->NotifyUpdateBuff(this);
	}
	return deltaHP;
}

bool XBuff::HasTag(XBuffTag tag)
{
	for(auto i = m_BuffInfo->Tags.begin(); i < m_BuffInfo->Tags.end(); ++i)
	{
		if (*i == tag)
			return true;
	}
	return false;
}

void XBuff::ReApplyChangeAttributeEffect(CombatEffectHelper* pEffectHeler)
{
	for (int i = 0; i < MAX_BUFFEFFECT_COUNT; ++i)
	{
		BuffEffect *pEffect = m_BuffEffect[i];

		if (pEffect != NULL)
		{
			XBuffChangeAttribute* pChangeAttrEffect = dynamic_cast<XBuffChangeAttribute*>(pEffect);
			if(pChangeAttrEffect != NULL)
				pChangeAttrEffect->OnAdd(m_Entity, pEffectHeler);
		}
		else
		{
			break;
		}
	}
}

void XBuff::_SortEffect()
{
	qsort(m_BuffEffect, MAX_BUFFEFFECT_COUNT, sizeof(BuffEffect*), BuffEffectCompare);
}

UINT32 XBuff::GetActualDuration()
{
	return (UINT32)(TimeUtil::GetMilliSecond() - m_StartTime);
}

void XBuff::OnBattleEnd()
{
	for (int i = 0; i < MAX_BUFFEFFECT_COUNT; ++i)
	{
		BuffEffect *pEffect = m_BuffEffect[i];

		if (pEffect != NULL)
		{
			pEffect->OnBattleEnd();
		}
		else
		{
			break;
		}
	}
	m_SkillModify.OnBattleEnd(this);
}

bool SkillCostModifyEffect::TryCreate(CombatEffectHelper* pEffectHeler, XBuff *pBuff)
{
	const BuffTable::RowData* pBuffInfo = pEffectHeler->GetBuffData();

	if (pBuffInfo->CostModify != 0)
	{
		CostModify += pBuffInfo->CostModify - 1.0l;
	}

	double castDamage = 0;
	double receiveDamage = 0;
	pEffectHeler->GetBuffChangeDamage(castDamage, receiveDamage);
	castDamage += pBuffInfo->ChangeDamage[0];
	receiveDamage += pBuffInfo->ChangeDamage[1];

	if (castDamage != 0)
	{
		CastDamageModify += castDamage - 1.0l;
	}
	if (receiveDamage != 0)
	{
		if(receiveDamage > 1.0)
			IncReceivedDamageModify += receiveDamage - 1.0l;
		else
			DecReceivedDamageModify += receiveDamage - 1.0l;
	}
	if(pBuffInfo->ChangeSkillDamage.size() > 0 || pEffectHeler->HasEffect(CET_Buff_ChangeSkillDamage))
	{
		SetChangeSkillDamage(pEffectHeler);
	}
	return true;
}


void SkillCostModifyEffect::SetChangeSkillDamage(CombatEffectHelper* pEffectHeler)
{
	const BuffTable::RowData* pBuffInfo = pEffectHeler->GetBuffData();

	for (auto it = pBuffInfo->ChangeSkillDamage.begin(); it != pBuffInfo->ChangeSkillDamage.end(); ++it)
	{
		const Sequence<string, 2>& data = *it;
		UINT32 skillID = XCommon::XHash(data[0].c_str());
		double ratio = convert<double>(data[1]) / 100;

		ChangeSkillDamage[skillID] += ratio;
	}

	if (pEffectHeler->HasEffect(CET_Buff_ChangeSkillDamage))
	{
		std::vector<Sequence<UINT32, 2>> datas;
		pEffectHeler->GetBuffChangeSkillDamage(datas);

		for (auto it = datas.begin(); it != datas.end(); ++it)
		{
			ChangeSkillDamage[(*it)[0]] += double((*it)[1]) / 100;
		}
	}
}

double SkillCostModifyEffect::GetChangeSkillDamage(UINT32 skillID)
{
	std::map<UINT32, double>::iterator it = ChangeSkillDamage.find(skillID);
	if(it == ChangeSkillDamage.end())
		return 0;
	return it->second;
}

void SkillCostModifyEffect::OnRemove(XBuff* pBuff)
{
	_RefreshSecurity(pBuff);
}

void SkillCostModifyEffect::OnBattleEnd(XBuff* pBuff)
{
	_RefreshSecurity(pBuff);
}

void SkillCostModifyEffect::_RefreshSecurity(XBuff* pBuff)
{
	if (pBuff == NULL || pBuff->GetEntity() == NULL)
		return;

	if (DecReceivedDamageModify == 0)
		return;

	XSecurityBuffStatistics* pBuffStatistics = XSecurityBuffStatistics::TryGetStatistics(pBuff->GetEntity()->GetUnit());
	if (pBuffStatistics == NULL)
		return;

	pBuffStatistics->OnReduceDamage(pBuff, -DecReceivedDamageModify);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void BuffExclusive::Set(const BuffTable::RowData* pRowData)
{
	///> ClearTypes
	if (pRowData->ClearTypes.size() > 0)
	{
		for (auto it = pRowData->ClearTypes.begin(); it != pRowData->ClearTypes.end(); ++it)
		{
			_ClearTypes.insert(*it);
		}
	}

	///> ExclusiveTypes
	//if (exclusiveTypes != NULL && exclusiveTypes->size() > 0)
	//{
	//	for (auto it = exclusiveTypes->begin(); it != exclusiveTypes->end(); ++it)
	//	{
	//		_ExclusiveTypes.insert(*it);
	//	}
	//}

	///> SingleEffects
	if (pRowData->BuffState.size() > 0)
	{
		for (auto it = pRowData->BuffState.begin(); it != pRowData->BuffState.end(); ++it)
		{
			switch(*it)
			{
			case XBuffType_Transform:
			case XBuffType_Scale:
				///> 先不用单独的枚举，今后有需要再改
				_SingleEffect.insert(*it);
				break;
			default:
				break;
			}
		}
	}

}

bool BuffExclusive::CanAdd(int clearType) const
{
	return _ClearTypes.count(clearType) == 0;
}

//bool BuffExclusive::IsExcluded(const BuffTable::RowData* pRowData) const
//{
//	//if (IsExcluded(pRowData->ClearTypes))
//	//	return true;
//
//	if (_ExclusiveTypes.size() == 0)
//		return false;
//
//	return false;
//}
//
//bool BuffExclusive::IsExcluded(const std::vector<int>& other) const
//{
//	for (auto it = other.begin(); it != other.end(); ++it)
//	{
//		if (_ExclusiveTypes.count(*it))
//			return true;
//	}
//	return false;
//}

bool BuffExclusive::IsSingleEffectConflict(const BuffExclusive& other) const
{
	return Overlaped(_SingleEffect, other._SingleEffect);
}

bool BuffExclusive::ShouldClear(int clearType) const
{
	return CanAdd(clearType);
}

bool BuffExclusive::Overlaped(const std::unordered_set<int>& left, const std::unordered_set<int>& right)
{
	if (left.size() == 0 || right.size() == 0)
		return false;

	const std::unordered_set<int>& smallone = left.size() < right.size() ? left : right;
	const std::unordered_set<int>& bigone = left.size() >= right.size() ? left : right;

	for (auto it = smallone.begin(); it != smallone.end(); ++it)
	{
		if (bigone.count(*it) > 0)
			return true;
	}

	return false;
}
