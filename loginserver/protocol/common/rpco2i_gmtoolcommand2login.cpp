#include "pch.h"
#include "common/rpco2i_gmtoolcommand2login.h"
#include "gmmgr/reloadmgr.h"

// generate by ProtoGen at date: 2016/8/27 10:56:29

RPC_SERVER_IMPLEMETION(RpcO2I_GMToolCommand2Login, GMToolCommandArg, GMToolCommandRes)

void RpcO2I_GMToolCommand2Login::OnCall(const GMToolCommandArg &roArg, GMToolCommandRes &roRes)
{
	bool bRet = false;
	if(roArg.cmd().compare("reload") == 0)
	{
		bRet = CReloadMgr::Instance()->Reload(roArg.args(), *roRes.mutable_errmsg());
	}
	else
	{
		LogWarn("[Login] Invalid gmtool command: %s\n", roArg.cmd().c_str());
	}

	roRes.set_result(bRet);
}

void RpcO2I_GMToolCommand2Login::OnDelayReplyRpc(const GMToolCommandArg &roArg, GMToolCommandRes &roRes, const CUserData &roUserData)
{
}
