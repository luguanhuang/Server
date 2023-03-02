#include "pch.h"
#include "reddot/ptcc2g_cancelreddotreq.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolemisc.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2017/1/11 17:59:51

void PtcC2G_CancelRedDotReq::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(!pRole)
	{
		SSError << "Cannot Find Role, SessionID:" << m_sessionID << END; 
		return; 
	}
	RoleMisc* record = pRole->Get<RoleMisc>();
	if (!record)
	{
		return;
	}
	UINT32 nFlag = record->GetHintFlag();
	if (nFlag==1)
	{
		record->SetHintFlag(2);
		record->SetSaveFlag();
		pRole->HintNotify(SYS_QUESTIONNAIRE, true);
	}
}
