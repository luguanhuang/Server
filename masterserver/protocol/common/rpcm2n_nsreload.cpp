#include "pch.h"
#include "common/rpcm2n_nsreload.h"
#include "utility/reloadmgr.h"

// generate by ProtoGen at date: 2016/11/18 15:04:39

RPC_CLIENT_IMPLEMETION(RpcM2N_NsReload, GMToolCommandArg, GMToolCommandRes)

void RpcM2N_NsReload::OnReply(const GMToolCommandArg &roArg, const GMToolCommandRes &roRes, const CUserData &roUserData)
{
	GMToolCommandRes res = roRes;
	res.set_errmsg("[NsReload] " + res.errmsg());
	CReloadMgr::Instance()->OnReloadDelayReply(roArg.delayid(), res);
}

void RpcM2N_NsReload::OnTimeout(const GMToolCommandArg &roArg, const CUserData &roUserData)
{
	m_oRes.set_result(false);
	m_oRes.set_errmsg("[NsReload] Rpc timeout");
	CReloadMgr::Instance()->OnReloadDelayReply(roArg.delayid(), m_oRes);
}
