#include "pch.h"
#include "XComponent.h"
#include "XSuperArmorComponent.h"
#include "table/XEntityStatistics.h"
#include "entity/XObject.h"
#include "unit/attributewatcher.h"
#include "unit/unit.h"
#include "unit/enemy.h"
#include "table/CombatParamMgr.h"
#include "state/XStateMachine.h"
#include "entity/XEntity.h"

XSuperArmorComponent::XSuperArmorComponent()
{
	m_WoozyOn = false;
	m_handler = INVALID_HTIMER;
	m_pEnemy = NULL;
}

XSuperArmorComponent::~XSuperArmorComponent()
{
	if (m_handler != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = INVALID_HTIMER;
	}
}

void XSuperArmorComponent::OnDetachFromHost()
{
	if (m_handler != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = INVALID_HTIMER;
	}

	XComponent::OnDetachFromHost();
}

void XSuperArmorComponent::Attached()
{
	m_pEnemy = dynamic_cast<Enemy*>(GetHost()->GetUnit());
	SetRecoveryTimeLimit(m_pEnemy->GetTemplate()->SuperArmorRecoveryMax);
}

void XSuperArmorComponent::Update(float fDeltaT)
{
	if (m_pEnemy->IsDead())
	{
		return;
	}

	if (m_pEnemy->IsArmorBroken())
	{
		RecoverySuperArmor(fDeltaT);
	}
	else
	{
		CheckSuperArmorBroken();
	}
}

void XSuperArmorComponent::CheckSuperArmorBroken()
{
	if (m_pEnemy->GetAttr(BASIC_CurrentSuperArmor) <= 0 &&
		m_pEnemy->GetAttr(BASIC_CurrentHP) > 0)
	{
		// 设置虚弱状态和破甲状态
		m_WoozyOn = false;
		m_pEnemy->SetArmorBroken(true);

		// 设置破甲最大回满时间
		if (m_handler != INVALID_HTIMER)
		{
			CTimerMgr::Instance()->KillTimer(m_handler);
			m_handler = INVALID_HTIMER;
		}
		
		// 通知其他组件破甲
		XArmorBrokenArgs args;
		GetHost()->DispatchAction(&args);

		// 破甲后获得特殊buff
		auto pTemplate = m_pEnemy->GetTemplate();
		if (pTemplate->SuperArmorBrokenBuff[0] != 0 && pTemplate->SuperArmorBrokenBuff[1] != 0)
		{
			XAddBuffActionArgs args;
			args.singleBuff.buffID = pTemplate->SuperArmorBrokenBuff[0];
			args.singleBuff.BuffLevel = pTemplate->SuperArmorBrokenBuff[1];
			GetHost()->DispatchAction(&args);
		}
	}
}

void XSuperArmorComponent::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	// 回满超级盔甲
	AttributeWatcher wather(m_pEnemy);
	double superArmorGen = m_pEnemy->GetAttr(TOTAL_MaxSuperArmor) - m_pEnemy->GetAttr(BASIC_CurrentSuperArmor) + 1;
	wather.AddAttr(BASIC_CurrentSuperArmor, superArmorGen);
	m_pEnemy->CheckAttrState();
	wather.CheckChangedAttributeAndNotify(true);

	// 取消破甲状态和虚弱状态
	m_pEnemy->SetArmorBroken(false);
	m_WoozyOn = false;

	// 通知其他组件取消破甲状态
	XArmorRecoverArgs args;
	GetHost()->DispatchAction(&args);

	m_handler = INVALID_HTIMER;
	return;
}

double XSuperArmorComponent::GetRateByState()
{
	KKSG::XQTEState qte = GetEntity()->GetQTESpecificPhase();

	if (qte != KKSG::QTE_None)
	{
		auto *pConf = CombatParamMgr::Instance()->GetSuperArmorCoff(m_pEnemy->GetTemplate()->Type, qte);
		if (pConf == NULL)
		{
			return 1.0;
		}

		return pConf->SupRecoveryChange;
	}

	return 1.0;
}

void XSuperArmorComponent::RecoverySuperArmor(float fDeltaT)
{
	if (m_WoozyOn)
	{
		auto pTemplate = m_pEnemy->GetTemplate();
		double inc = m_pEnemy->GetAttr(TOTAL_MaxSuperArmor) * pTemplate->SuperArmorRecoveryValue / 100;
		double rate = GetRateByState();
		AttributeWatcher wather(m_pEnemy);
		double superArmorGen = inc * rate * fDeltaT;
		wather.AddAttr(BASIC_CurrentSuperArmor, superArmorGen);
		m_pEnemy->CheckAttrState();
		wather.CheckChangedAttributeAndNotify(true);

		if (m_pEnemy->GetAttr(TOTAL_MaxSuperArmor) <= m_pEnemy->GetAttr(BASIC_CurrentSuperArmor) && 
			m_pEnemy->GetAttr(BASIC_CurrentHP) > 0)
		{
			m_pEnemy->SetArmorBroken(false);
			m_WoozyOn = false;

			CTimerMgr::Instance()->KillTimer(m_handler);
			m_handler = INVALID_HTIMER;

			XArmorRecoverArgs args;
			GetHost()->DispatchAction(&args);
		}
	}
}

bool XSuperArmorComponent::OnWoozyOffEvent(IArgs* pargs, void *)
{
	auto pTemplate = m_pEnemy->GetTemplate();
	if (pTemplate->SuperArmorBrokenBuff[0] != 0 && pTemplate->SuperArmorBrokenBuff[1] != 0)
	{
		XRemoveBuffActionArgs args;
		args.nBuffId = pTemplate->SuperArmorBrokenBuff[0];
		GetHost()->DispatchAction(&args);
	}

	m_WoozyOn = false;

	return true;
}

bool XSuperArmorComponent::OnWoozyEvent(IArgs* pargs, void *)
{
	m_WoozyOn = true;

	// 设置破甲最大回满时间
	if (m_handler != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = INVALID_HTIMER;
	}

	if (m_recoveryTimeLimit > 0)
	{
		m_handler = CTimerMgr::Instance()->SetTimer(this, 0, (int)(m_recoveryTimeLimit * 1000), 1, __FILE__, __LINE__);
	}

	return true;
}

void XSuperArmorComponent::ActionSubscribe()
{
	RegisterAction(XAction_WoozyOn, DelegateCombination(XSuperArmorComponent, OnWoozyEvent, this));
	RegisterAction(XAction_WoozyOff, DelegateCombination(XSuperArmorComponent, OnWoozyOffEvent, this));
}

