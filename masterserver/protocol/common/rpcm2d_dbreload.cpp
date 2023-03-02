#include "pch.h"
#include "common/rpcm2d_dbreload.h"
#include "utility/reloadmgr.h"

// generate by ProtoGen at date: 2016/9/28 0:17:05

RPC_CLIENT_IMPLEMETION(RpcM2D_DbReload, GMToolCommandArg, GMToolCommandRes)

void RpcM2D_DbReload::OnReply(const GMToolCommandArg &roArg, const GMToolCommandRes &roRes, const CUserData &roUserData)
{
	GMToolCommandRes res = roRes;
	res.set_errmsg("[DbReload] " + res.errmsg());
	CReloadMgr::Instance()->OnReloadDelayReply(roArg.delayid(), res);
}

void RpcM2D_DbReload::OnTimeout(const GMToolCommandArg &roArg, const CUserData &roUserData)
{
	m_oRes.set_result(false);
	m_oRes.set_errmsg("[DbReload] Rpc timeout");
	CReloadMgr::Instance()->OnReloadDelayReply(roArg.delayid(), m_oRes);
}
