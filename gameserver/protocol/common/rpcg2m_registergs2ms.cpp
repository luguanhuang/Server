#include "pch.h"
#include "common/rpcg2m_registergs2ms.h"
#include "network/mslink.h"
#include "reconnetmgr.h"
#include "network/gatelink.h"
#include "config.h"
#include "spactivity/spactivitymgr.h"
#include "table/OpenSystemMgr.h"
#include "gamelogic/MultActivityMgr.h"

// generate by ProtoGen at date: 2016/7/5 20:50:53

RPC_CLIENT_IMPLEMETION(RpcG2M_RegisterGs2Ms, RegisterGs2MsArg, RegisterGs2MsRes)

void RpcG2M_RegisterGs2Ms::OnReply(const RegisterGs2MsArg &roArg, const RegisterGs2MsRes &roRes, const CUserData &roUserData)
{
	if(roRes.errorcode() == KKSG::ERR_SUCCESS)
	{
		LogInfo("Game server line [%u] register to ms success", roArg.line());
		MSLink::Instance()->SetRegistered(true);
		MSLink::Instance()->AddLinkTimes();

		GSConfig::Instance()->SetAppType(roRes.apptype());
		GSConfig::Instance()->SetPlatType(roRes.plat());
		GSConfig::Instance()->SetGameServerOpenTimeStamp(roRes.openservertime());
		GSConfig::Instance()->SetCombineServerTime(roRes.combineservertime());
		GSConfig::Instance()->SetBackFlowLevel(roRes.backflowlevel());

		MultActivityMgr::Instance()->SetOpenTime(roRes.openservertime());
		SpActivityMgr::Instance()->OnGetGameServerOpenTime();
		OpenSystemMgr::Instance()->OnGetGameServerOpenTime();
		//SpActivityMgr::Instance()->OnGetBackFlowLevel();
	}
	else
	{
		LogError("Gate server line [%u] register to ms failed", roArg.line());
	}
}

void RpcG2M_RegisterGs2Ms::OnTimeout(const RegisterGs2MsArg &roArg, const CUserData &roUserData)
{
	LogError("Game server line [%u] register to ms timeout", roArg.line());
}
