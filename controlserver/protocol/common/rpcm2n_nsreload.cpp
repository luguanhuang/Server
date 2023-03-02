#include "pch.h"
#include "common/rpcm2n_nsreload.h"
#include "config.h"
#include "queuing/queuingmgr.h"
#include "account/whitelistmgr.h"
#include "scene/sceneconfig.h"
#include "table/globalconfig.h"

// generate by ProtoGen at date: 2016/11/18 15:04:39

RPC_SERVER_IMPLEMETION(RpcM2N_NsReload, GMToolCommandArg, GMToolCommandRes)

void RpcM2N_NsReload::OnCall(const GMToolCommandArg &roArg, GMToolCommandRes &roRes)
{
	LogInfo("Start reload table %s", roArg.cmd().c_str());
	bool bRet = false;
	if(roArg.cmd().compare("usewhitelist") == 0)
	{
		bRet = ControlConfig::Instance()->LoadUseWhiteList();
	}
	else if (roArg.cmd().compare("reconnect") == 0)
	{
		bRet = true;
		ControlConfig::Instance()->LoadReconnect();
	}
	else if(roArg.cmd().compare("queuing") == 0)
	{
		bRet = QueuingMgr::Instance()->LoadFile();
	}
	else if(roArg.cmd().compare("whitelist") == 0)
	{
		bRet = CWhiteListMgr::Instance()->LoadWhiteList();
	}
	else if (roArg.cmd().compare("SceneList") == 0)
	{
		bRet = CSceneConfig::Instance()->LoadFile();
	}
	else if (roArg.cmd().compare("GlobalConfig") == 0)
	{
		bRet = GetGlobalConfig().LoadFile();
	}
	else
	{
		LogWarn("Invalid reload file: %s", roArg.cmd().c_str());
		roRes.set_result(false);
		roRes.set_errmsg("Invalid reload file: " + roArg.cmd());
		return;
	}

	roRes.set_result(bRet);
	if(bRet)
	{
		roRes.set_errmsg("Reload " + roArg.cmd() + " file succeed");
		LogInfo("%s", roRes.errmsg().c_str());
	}
	else
	{
		roRes.set_errmsg("Reload " + roArg.cmd() + " file failed");
		LogError("%s", roRes.errmsg().c_str());
	}
}

void RpcM2N_NsReload::OnDelayReplyRpc(const GMToolCommandArg &roArg, GMToolCommandRes &roRes, const CUserData &roUserData)
{
}
