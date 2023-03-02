#include "pch.h"
#include "XBuffTriggerByQTE.h"
#include "timermgr.h"
#include "entity/XObject.h"
#include "XBuffTrigger.h"
#include "state/XStateMachine.h"
#include "entity/XEntity.h"


XBuffTriggerByQTE::XBuffTriggerByQTE(XBuff *pBuff) : XBuffTrigger(pBuff)
{
	for(int i = 0; ; ++i)
	{
		UINT32 state = _GetTriggerParamInt(pBuff->GetBuffInfo(), i);
		if(state == 0)
			break;
		m_QTEStates.insert(state);
	}
	m_PreToken = 0;
	///>  这里客户端没有强行改true，所以先不统一走配置
	m_bIsTriggerImm = true;
}

XBuffTriggerByQTE::~XBuffTriggerByQTE()
{
}

bool XBuffTriggerByQTE::CheckTriggerCondition()
{
	if(GetEntity()->StateMachine() == NULL)
		return false;

	if(GetEntity()->StateMachine()->ActionToken() == m_PreToken)
		return false;
	m_PreToken = GetEntity()->StateMachine()->ActionToken();

	if (GetEntity()->StateMachine()->GetCurrentState() != XState_BeHit && GetEntity()->StateMachine()->GetCurrentState() != XState_Freeze)
	{
		return false;
	}

	KKSG::XQTEState state = GetEntity()->GetQTESpecificPhase();
	return m_QTEStates.count((UINT32)state) > 0;
}

void XBuffTriggerByQTE::OnQTEStateChanged(const XOnQTEOnEventArgs* arg)
{
	if(m_QTEStates.count((UINT32)arg->State) > 0)
	{
		Trigger();
	}
}
