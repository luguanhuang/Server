#include "pch.h"
#include "XBuffSpecialState.h"
#include "gamelogic/XCombat.h"
#include "component/XBuffComponent.h"
#include "table/SkillReader.h"
#include "entity/XEntity.h"
#include "component/XSkillComponent.h"
#include "state/XStateMachine.h"
#include "component/XBeHitComponent.h"
#include "utility/uidhelper.h"
#include "gamelogic/fightgroup.h"
#include "scene/securitybuffstatistics.h"
#include "buffrecord.h"
#include "unit/role.h"

XBuffSpecialState::Func XBuffSpecialState::sStateHandler[XBuffType_Max] = {NULL};

struct InitHelper 
{
	InitHelper() { XBuffSpecialState::InitHandlerFun(); }
};

static InitHelper sInitHelper;

void XBuffSpecialState::InitHandlerFun()
{
	sStateHandler[XBuffType_Immortal] = &XBuffSpecialState::OnImmortal;
	sStateHandler[XBuffType_Bati]     = &XBuffSpecialState::OnBati;
	sStateHandler[XBuffType_CantDie]     = &XBuffSpecialState::OnCantDie;
	sStateHandler[XBuffType_Shield]     = &XBuffSpecialState::OnShield;
	sStateHandler[XBuffType_Trapped]     = &XBuffSpecialState::OnTrapped;
}

Role* CheckChangeOutlookRole(XEntity* pEntity, int buffID)
{
	if (GetGlobalConfig().TransformBuffsChangeOutlook.count(buffID) == 0)
		return NULL;
	
	if (pEntity->GetUnit()->IsRole())
		return (Role*)pEntity->GetUnit();

	return NULL;
}


bool XBuffSpecialState::TryCreate(const BuffTable::RowData *pBuffInfo, XBuff *pBuff)
{
	if (pBuffInfo->BuffState.empty())
		return false;

	for (unsigned i = 0; i < pBuffInfo->BuffState.size(); ++i)
	{
		pBuff->AddBuffEffect(new XBuffSpecialState(pBuff, pBuffInfo->BuffState[i]));
	}
	return true;
}

bool XBuffSpecialState::CanBuffAdd(XEntity* pEntity, const BuffTable::RowData* pRowData)
{
	if (pEntity == NULL || pRowData == NULL)
		return false;

	Role* pRole = CheckChangeOutlookRole(pEntity, pRowData->BuffID);
	if (pRole && pRowData->BuffState.size() > 0)
	{
		for (auto it = pRowData->BuffState.begin(); it != pRowData->BuffState.end(); ++it)
		{
			switch(*it)
			{
			case XBuffType_Transform:
				if (pRole->Get<BuffRecord>()->CanTrans(1) != 0)
					return false;
				break;
			case XBuffType_Scale:
				if (pRole->Get<BuffRecord>()->CanTrans(2) != 0)
					return false;
				break;
			default:
				break;
			}
		}
	}

	return true;
}

XBuffSpecialState::XBuffSpecialState(XBuff *pBuff, int buffState)
{
	m_pBuff = pBuff;
	m_nBuffType = (XBuffType)buffState;
	m_bUpdated = false;
	m_pEntity = NULL;
}

XBuffSpecialState::~XBuffSpecialState()
{

}

XBuffEffectPrioriy XBuffSpecialState::GetPriority()
{
	switch(m_nBuffType)
	{
	case XBuffType_CantDie:
		return XBuffEffectPrioriy_SpecialState_CantDie;
		break;
	case XBuffType_Immortal:
		return XBuffEffectPrioriy_SpecialState_Immortal;
		break;
	case XBuffType_Shield:
		return XBuffEffectPrioriy_SpecialState_Shield;
		break;
	case XBuffType_Trapped:
		return XBuffEffectPrioriy_SpecialState_Trapped;
		break;
	default:
		return XBuffEffectPrioriy_Default;
		break;
	}
}

void XBuffSpecialState::OnAdd(XEntity*pEntity, CombatEffectHelper* pEffectHelper)
{
	m_pEntity = pEntity;

	switch(m_nBuffType)
	{
	case XBuffType_Transform:
	case XBuffType_Scale:
		m_OldStateParam = m_pEntity->BuffComponent()->GetStateParam(m_nBuffType);
		m_MyStateParam = m_pBuff->GetID();
		break;
	default:
		m_MyStateParam = m_pBuff->GetBuffInfo()->StateParam;
		break;
	}

	pEntity->BuffComponent()->IncBuffState(m_nBuffType, m_MyStateParam);

	switch(m_nBuffType)
	{
	case XBuffType_Freeze:
		{
			XFreezeActionArgs args;
			args.HitFrom = m_pBuff->GetCaster();
			m_nToken = UIDHelper::CreateUID(UID_Common);
			args.SetToken(m_nToken);
			args.Duration = (float)(m_pBuff->GetDuration()) / 1000;
			args.Present = false;
			pEntity->DispatchAction(&args);

		}
		break;
	case XBuffType_Scale:
		{
			m_pEntity->BuffComponent()->MakeSingleEffect(m_pBuff);

			Role* pRole = CheckChangeOutlookRole(pEntity, m_pBuff->GetID());
			if (pRole)
			{
				pRole->Get<BuffRecord>()->OnAddTransBuff(m_pBuff->GetID(), m_pBuff->GetLevel(), m_pBuff->GetLeftTime() / 1000, m_pBuff->GetCaster());
			}
		}
		break;
	default:
		break;
	}

}

void XBuffSpecialState::OnUpdate()
{
	switch(m_nBuffType)
	{
	case XBuffType_Transform:
		{
			if (!m_bUpdated)
			{
				Role* pRole = CheckChangeOutlookRole(m_pEntity, m_pBuff->GetID());
				if (pRole)
				{
					///> 当前状态不可变身，回到变身前状态 并删除此buff
					if(pRole->Get<BuffRecord>()->CanTrans() != 0)
					{
						RestoreStateParam();

						XRemoveBuffActionArgs arg;
						arg.nBuffId = m_pBuff->GetID();
						arg.Caster = m_pEntity->GetID();
						m_pEntity->DispatchAction(&arg);
						return;
					}
				}

				if (TryTransform(m_pEntity, m_pBuff->GetID(), (UINT32)std::abs(m_pBuff->GetBuffInfo()->StateParam), true) == TR_Success)
				{
					m_pEntity->BuffComponent()->MakeSingleEffect(m_pBuff);

					if (pRole)
					{
						pRole->Get<BuffRecord>()->OnAddTransBuff(m_pBuff->GetID(), m_pBuff->GetLevel(), m_pBuff->GetLeftTime() / 1000, m_pBuff->GetCaster());
					}

					m_pBuff->GetEffectData()->TransformID = m_pBuff->GetBuffInfo()->StateParam;

					XBuffComponent *pComponent = m_pEntity->BuffComponent();
					if (pComponent != NULL)
						pComponent->NotifyUpdateBuff(m_pBuff);

				}
				else
					RestoreStateParam();
			}
		}
		break;
	default:
		break;
	}
	m_bUpdated = true;
}

void XBuffSpecialState::RestoreStateParam()
{
	if (m_pEntity->BuffComponent()->GetStateParam(m_nBuffType) == m_MyStateParam)
		m_pEntity->BuffComponent()->SetStateParam(m_nBuffType, m_OldStateParam);
}

void XBuffSpecialState::OnRemove(XBuffRemoveReason reason)
{
	switch(m_nBuffType)
	{
	case XBuffType_Freeze:
		{
			if(reason != XBuffRemoveReason_Replace)
			{
				Unit* pCaster = Unit::FindUnit(m_pBuff->GetCaster());
				XSecurityBuffStatistics* pBuffStatistics = XSecurityBuffStatistics::TryGetStatistics(pCaster);
				if (pBuffStatistics != NULL)
					pBuffStatistics->OnFreeze(m_pBuff);
				if (m_pBuff->GetEntity()->StateMachine()->ActionToken() == m_nToken)
				{
					m_pBuff->GetEntity()->StateMachine()->ForceToDefaultState(false);
				}
			}
		}
		break;
	case XBuffType_Transform:
		{
			bool bRealNeedDeleteRecord = reason == XBuffRemoveReason_Replace;
			if(reason != XBuffRemoveReason_Replace)
			{
				if (m_pBuff->GetBuffInfo()->StateParam >= 0 || !m_pBuff->GetEntity()->IsDead())
					bRealNeedDeleteRecord = TryTransform(m_pBuff->GetEntity(), m_pBuff->GetID(), (UINT32)std::abs(m_pBuff->GetBuffInfo()->StateParam), false) == TR_Success;
			}

			if (bRealNeedDeleteRecord)
			{
				Role* pRole = CheckChangeOutlookRole(m_pEntity, m_pBuff->GetID());
				if (pRole)
				{
					pRole->Get<BuffRecord>()->OnDelTransBuff(m_pBuff->GetID(), reason != XBuffRemoveReason_Manual);
				}
			}
		}
		break;
	case XBuffType_Scale:
		{
			Role* pRole = CheckChangeOutlookRole(m_pEntity, m_pBuff->GetID());
			if (pRole)
			{
				pRole->Get<BuffRecord>()->OnDelTransBuff(m_pBuff->GetID(), reason != XBuffRemoveReason_Manual);
			}
		}
		break;
	case XBuffType_Immortal:
		{
			if(reason != XBuffRemoveReason_Replace)
			{
				XSecurityBuffStatistics* pBuffStatistics = XSecurityBuffStatistics::TryGetStatistics(m_pBuff->GetEntity()->GetUnit());
				if (pBuffStatistics != NULL)
					pBuffStatistics->OnImmortal(m_pBuff);
			}
		}
		break;
	default:
		break;
	}

	switch(m_nBuffType)
	{
	case XBuffType_Transform:
	case XBuffType_Scale:
		m_pBuff->GetEntity()->BuffComponent()->DecBuffState(m_nBuffType, m_pBuff->GetID());
		break;
	default:
		m_pBuff->GetEntity()->BuffComponent()->DecBuffState(m_nBuffType, m_pBuff->GetBuffInfo()->StateParam);
		break;
	}
}

void XBuffSpecialState::OnBattleEnd()
{
	switch(m_nBuffType)
	{
	case XBuffType_Freeze:
		{
			Unit* pCaster = Unit::FindUnit(m_pBuff->GetCaster());
			XSecurityBuffStatistics* pBuffStatistics = XSecurityBuffStatistics::TryGetStatistics(pCaster);
			if (pBuffStatistics != NULL)
				pBuffStatistics->OnFreeze(m_pBuff);
		}
		break;
	case XBuffType_Immortal:
		{
			XSecurityBuffStatistics* pBuffStatistics = XSecurityBuffStatistics::TryGetStatistics(m_pBuff->GetEntity()->GetUnit());
			if (pBuffStatistics != NULL)
				pBuffStatistics->OnImmortal(m_pBuff);
		}
		break;
	default:
		break;
	}
}


void XBuffSpecialState::OnResetTime()
{
	switch(m_nBuffType)
	{
	case XBuffType_Transform:
	case XBuffType_Scale:
		{
			Role* pRole = CheckChangeOutlookRole(m_pEntity, m_pBuff->GetID());
			if (pRole)
			{
				pRole->Get<BuffRecord>()->OnAddTransBuff(m_pBuff->GetID(), m_pBuff->GetLevel(), m_pBuff->GetLeftTime() / 1000, m_pBuff->GetCaster());
			}
		}
		break;
	default:
		break;
	}
}

bool XBuffSpecialState::OnBuffEffect(const HurtInfo &hurtInfo, ProjectDamageResult &result)
{
	if (sStateHandler[m_nBuffType])
	{
		Func f = sStateHandler[m_nBuffType];
		(this->*f)(hurtInfo, result);
	}

	return true;
}

void XBuffSpecialState::OnBati(const HurtInfo &hurtInfo, ProjectDamageResult &result)
{
	result.SetResult(PJRES_BATI);
}


void XBuffSpecialState::OnImmortal(const HurtInfo &hurtInfo, ProjectDamageResult &result)
{
	result.SetResult(PJRES_IMMORTAL);
	result.SetValue(0);
}

void XBuffSpecialState::OnCantDie(const HurtInfo &hurtInfo, ProjectDamageResult &result)
{
	result.SetValue(-GetCantDieDamage(-result.GetValue(), hurtInfo.m_target));
}

void XBuffSpecialState::OnShield(const HurtInfo &hurtInfo, ProjectDamageResult &result)
{
	if (result.GetValue() > 0)
	{
		double absorb = -m_pBuff->ChangeBuffHP(-result.GetValue());
		result.SetAbsorbDamage(absorb);
	}
}

void XBuffSpecialState::OnTrapped(const HurtInfo &hurtInfo, ProjectDamageResult &result)
{
	if (result.GetValue() > 0)
	{
		if(hurtInfo.m_caster != NULL && hurtInfo.m_caster->GetUnit() != NULL)
		{
			if(!FightGroupMgr::Instance()->CanAttack(hurtInfo.m_caster->GetUnit()->GetFightGroup(), hurtInfo.m_target->GetUnit()->GetFightGroup()))
			{
				double absorb = -m_pBuff->ChangeBuffHP(-result.GetValue());
				//result.SetAbsorbDamage(absorb);
				result.SetValue(0);
			}
		}
	}
}

double XBuffSpecialState::GetCantDieDamage(double originalDeltaValue, XObject* pEntity)
{
	double leftHP = pEntity->GetUnit()->GetAttr(BASIC_CurrentHP);
	if (leftHP + originalDeltaValue <= 0)
	{
		return std::min(-(leftHP - 1), 0.0);
	}

	return originalDeltaValue;
}

double XBuffSpecialState::GetActualChangeAttr(CombatAttrDef attr, double deltaValue, XObject* pEntity, bool bIgnoreImmortal, bool bForceCantDie)
{
	if (attr == BASIC_CurrentHP && deltaValue < 0)
	{
		// 判断无敌情况
		if (!bIgnoreImmortal && pEntity->BuffComponent()->IsBuffStateOn(XBuffType_Immortal))
			return 0.0;

		// 判断不死情况
		if (pEntity->BuffComponent()->IsBuffStateOn(XBuffType_CantDie) || bForceCantDie)
		{
			return XBuffSpecialState::GetCantDieDamage(deltaValue, pEntity);
		}

		return deltaValue;
	}

	return deltaValue;
}

XBuffSpecialState::TransformResult XBuffSpecialState::TryTransform(XEntity* pEntity, int buffID, UINT32 transformID, bool bTransform)
{
	if (pEntity == NULL || transformID == 0 || pEntity->BuffComponent() == NULL)
		return TR_Error;

	///> 加了别的变身buff，当前变身buff已不是此buff，不回退参数
	if (pEntity->BuffComponent()->GetStateParam(XBuffType_Transform) != buffID)
		return TR_NoEffect;

	bool bRes;
	if (bTransform)
	{
		bRes = pEntity->OnTransform(transformID);
	}
	else
	{
		bRes = pEntity->OnTransform(0);
	}

	if (bRes)
		return TR_Success;
	return TR_NoEffect;
}
