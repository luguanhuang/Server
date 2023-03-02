#include "pch.h"
#include "common/rpcg2m_registergs2ms.h"
#include "network/gslink.h"
#include "network/gatelink.h"
#include "config.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/briefranklist.h"
#include "team/teammgr.h"
#include "global/GlobalConfigDBBuild.h"

// generate by ProtoGen at date: 2016/7/5 20:50:53

RPC_SERVER_IMPLEMETION(RpcG2M_RegisterGs2Ms, RegisterGs2MsArg, RegisterGs2MsRes)

void RpcG2M_RegisterGs2Ms::OnCall(const RegisterGs2MsArg &roArg, RegisterGs2MsRes &roRes)
{
	KKSG::ErrorCode nErrCode = GSLink::Instance()->AddGameServer(GetConnID(), roArg.serverid(), roArg.line());
	roRes.set_errorcode(nErrCode);
	roRes.set_apptype(MSConfig::Instance()->GetAppType());
	roRes.set_plat(MSConfig::Instance()->GetPlatType());
	roRes.set_openservertime(MSConfig::Instance()->GetGameServerOpenTimeStamp());
	roRes.set_combineservertime(convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eCombineServerTime)));
	roRes.set_backflowlevel(MSConfig::Instance()->GetBackFlowLevel());

	CBriefRankList* levellist = (CBriefRankList*)CRankListMgr::Instance()->GetRankList(KKSG::LevelRank);
	levellist->OnGSConnected(roArg.line());

	TeamMgr::Instance()->OnGsRegister(roArg.line());	
}

void RpcG2M_RegisterGs2Ms::OnDelayReplyRpc(const RegisterGs2MsArg &roArg, RegisterGs2MsRes &roRes, const CUserData &roUserData)
{
}
