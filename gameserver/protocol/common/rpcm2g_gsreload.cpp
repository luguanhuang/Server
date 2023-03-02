#include "pch.h"
#include "common/rpcm2g_gsreload.h"
#include "gamelogic/reloadmgr.h"
#include "config.h"
#include "util.h"

// generate by ProtoGen at date: 2016/9/28 0:22:23

RPC_SERVER_IMPLEMETION(RpcM2G_GsReload, GMToolCommandArg, GMToolCommandRes)

void RpcM2G_GsReload::OnCall(const GMToolCommandArg &roArg, GMToolCommandRes &roRes)
{
	LogInfo("Start reload table %s", roArg.cmd().c_str());
	bool result = ReloadMgr::Instance()->ReloadTable(roArg.type(), roArg.cmd(), *roRes.mutable_errmsg());

	if(result) LogInfo("%s", roRes.errmsg().c_str());
	else LogError("%s", roRes.errmsg().c_str());

	roRes.set_result(result);
	roRes.set_errmsg("line[" + ToString(GSConfig::Instance()->GetLine()) + "] " + roRes.errmsg());	
}

void RpcM2G_GsReload::OnDelayReplyRpc(const GMToolCommandArg &roArg, GMToolCommandRes &roRes, const CUserData &roUserData)
{
}
