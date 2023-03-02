#include "pch.h"
#include "common/rpco2v_gmtoolcommand2version.h"
#include "reload/reloadmgr.h"

// generate by ProtoGen at date: 2017/2/6 11:01:29

RPC_SERVER_IMPLEMETION(RpcO2V_GMToolCommand2Version, GMToolCommandArg, GMToolCommandRes)

void RpcO2V_GMToolCommand2Version::OnCall(const GMToolCommandArg &roArg, GMToolCommandRes &roRes)
{
	bool bRet = false;
	if(roArg.cmd().compare("reload") == 0)
	{
		bRet = CReloadMgr::Instance()->Reload(roArg.args(), *roRes.mutable_errmsg());
	}
	else
	{
		LogWarn("[Version] Invalid gmtool command: %s\n", roArg.cmd().c_str());
	}

	roRes.set_result(bRet);
}

void RpcO2V_GMToolCommand2Version::OnDelayReplyRpc(const GMToolCommandArg &roArg, GMToolCommandRes &roRes, const CUserData &roUserData)
{
}
