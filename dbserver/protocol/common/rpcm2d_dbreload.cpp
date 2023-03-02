#include "pch.h"
#include "common/rpcm2d_dbreload.h"
#include "config.h"

// generate by ProtoGen at date: 2016/9/28 0:17:05

RPC_SERVER_IMPLEMETION(RpcM2D_DbReload, GMToolCommandArg, GMToolCommandRes)

void RpcM2D_DbReload::OnCall(const GMToolCommandArg &roArg, GMToolCommandRes &roRes)
{
	LogInfo("Start reload table %s", roArg.cmd().c_str());
	bool bRet = false;
	if(roArg.cmd().compare("dbloginlimit") == 0)
	{
		bRet = DBConfig::Instance()->ReLoadLoinLimitFile();
		if(bRet)
		{
			roRes.set_errmsg("Reload login limit file succeed");
			LogInfo("%s", roRes.errmsg().c_str());
		}
		else
		{
			roRes.set_errmsg("Reload login limit file failed");
			LogWarn("%s", roRes.errmsg().c_str());
		}
	}
	else if(roArg.cmd().compare("maxregisternum") == 0)
	{
		bRet = DBConfig::Instance()->ReLoadMaxRegisterNum();
		if(bRet)
		{
			roRes.set_errmsg("Reload MaxRegisterNum succeed");
			LogInfo("%s", roRes.errmsg().c_str());
		}
		else
		{
			roRes.set_errmsg("Reload MaxRegisterNum failed");
			LogWarn("%s", roRes.errmsg().c_str());
		}
	}
	else
	{
		LogWarn("Invalid reload file: %s", roArg.cmd().c_str());
		roRes.set_errmsg("Invalid reload file: " + roArg.cmd());
	}

	roRes.set_result(bRet);
}

void RpcM2D_DbReload::OnDelayReplyRpc(const GMToolCommandArg &roArg, GMToolCommandRes &roRes, const CUserData &roUserData)
{
}
