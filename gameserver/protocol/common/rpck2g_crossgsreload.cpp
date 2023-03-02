#include "pch.h"
#include "common/rpck2g_crossgsreload.h"
#include "gamelogic/reloadmgr.h"
#include "config.h"
#include "util.h"

// generate by ProtoGen at date: 2017/2/16 20:26:50

RPC_SERVER_IMPLEMETION(RpcK2G_CrossGsReload, GMToolCommandArg, GMToolCommandRes)

void RpcK2G_CrossGsReload::OnCall(const GMToolCommandArg &roArg, GMToolCommandRes &roRes)
{
	LogInfo("Start reload table %s", roArg.cmd().c_str());
	bool result = ReloadMgr::Instance()->ReloadTable(roArg.type(), roArg.cmd(), *roRes.mutable_errmsg());

	if(result) LogInfo("%s", roRes.errmsg().c_str());
	else LogError("%s", roRes.errmsg().c_str());

	roRes.set_result(result);
	roRes.set_errmsg("line[" + ToString(GSConfig::Instance()->GetLine()) + "] " + roRes.errmsg());
}

void RpcK2G_CrossGsReload::OnDelayReplyRpc(const GMToolCommandArg &roArg, GMToolCommandRes &roRes, const CUserData &roUserData)
{
}
