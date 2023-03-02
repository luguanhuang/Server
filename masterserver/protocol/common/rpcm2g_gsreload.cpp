#include "pch.h"
#include "common/rpcm2g_gsreload.h"
#include "utility/reloadmgr.h"

// generate by ProtoGen at date: 2016/9/28 0:22:23

RPC_CLIENT_IMPLEMETION(RpcM2G_GsReload, GMToolCommandArg, GMToolCommandRes)

void RpcM2G_GsReload::OnReply(const GMToolCommandArg &roArg, const GMToolCommandRes &roRes, const CUserData &roUserData)
{
	GMToolCommandRes res = roRes;
	res.set_errmsg("[GsReload] " + res.errmsg());
	CReloadMgr::Instance()->OnReloadDelayReply(roArg.delayid(), res);
}

void RpcM2G_GsReload::OnTimeout(const GMToolCommandArg &roArg, const CUserData &roUserData)
{
	m_oRes.set_result(false);
	m_oRes.set_errmsg("[GsReload] Rpc timeout\n");
	CReloadMgr::Instance()->OnReloadDelayReply(roArg.delayid(), m_oRes);
}
