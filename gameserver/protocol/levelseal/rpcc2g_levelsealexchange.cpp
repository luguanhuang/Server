#include "pch.h"
#include "levelseal/rpcc2g_levelsealexchange.h"
#include "gamelogic/levelsealMgr.h"
#include "gamelogic/levelsealRecord.h"
#include "unit/rolemanager.h"
#include "gamelogic/levelsealMgr.h"

// generate by ProtoGen at date: 2016/9/7 20:10:58

RPC_SERVER_IMPLEMETION(RpcC2G_LevelSealExchange, LevelSealExchangeArg, LevelSealExchangeRes)

void RpcC2G_LevelSealExchange::OnCall(const LevelSealExchangeArg &roArg, LevelSealExchangeRes &roRes)
{
	roRes.set_errcode(ERR_UNKNOWN);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errcode(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	

	KKSG::ErrorCode errcode = (KKSG::ErrorCode)CLevelSealMgr::Instance()->ExchangeGift(pRole);
	roRes.set_errcode(errcode);
}

void RpcC2G_LevelSealExchange::OnDelayReplyRpc(const LevelSealExchangeArg &roArg, LevelSealExchangeRes &roRes, const CUserData &roUserData)
{
}
