#include "pch.h"
#include "levelseal/rpcc2g_levelsealbuttonstatus.h"
#include "gamelogic/levelsealMgr.h"
#include "gamelogic/levelsealRecord.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2016/6/3 11:53:53

RPC_SERVER_IMPLEMETION(RpcC2G_LevelSealButtonStatus, LevelSealOverExpArg, LevelSealOverExpRes)

void RpcC2G_LevelSealButtonStatus::OnCall(const LevelSealOverExpArg &roArg, LevelSealOverExpRes &roRes)
{
	roRes.set_errorcode(ERR_UNKNOWN);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	CLevelSealRecord* record = pRole->Get<CLevelSealRecord>();
	if( NULL == record )
	{
		roRes.set_errorcode(ERR_SUCCESS);
		return; 
	}
	if (record->GetButtonStatus() == BUTTON_STATUS_LOCKED && pRole->GetLevelSealStatus() == true)
	{
		//玩家是封印未点击状态
		record->SetButtonStatus(BUTTON_STATUS_NONE);
	}
	else if(record->GetButtonStatus() == BUTTON_STATUS_UNLOCKED && pRole->GetLevelSealStatus() == false)
	{
		record->SetButtonStatus(BUTTON_STATUS_NONE);
	}
	else
	{
		roRes.set_errorcode(ERR_SUCCESS);
		return; 
	}
	roRes.set_errorcode(ERR_SUCCESS);
	roRes.set_m_ustatus(record->GetButtonStatus());
}

void RpcC2G_LevelSealButtonStatus::OnDelayReplyRpc(const LevelSealOverExpArg &roArg, LevelSealOverExpRes &roRes, const CUserData &roUserData)
{
}
