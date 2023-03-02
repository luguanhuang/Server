#include "pch.h"
#include "scene/rpcm2n_changescenefromms.h"
#include "logutil.h"

// generate by ProtoGen at date: 2016/11/4 18:05:42

RPC_CLIENT_IMPLEMETION(RpcM2N_ChangeSceneFromMs, ChangeSceneFromMsArg, ChangeSceneFromMsRes)

void RpcM2N_ChangeSceneFromMs::OnReply(const ChangeSceneFromMsArg &roArg, const ChangeSceneFromMsRes &roRes, const CUserData &roUserData)
{
	if(roRes.result() == KKSG::ERR_SUCCESS)
	{
		LogInfo("Role [%s] begin to change scene to tid [%u] sid [%u] success", Uint64List2Str(roArg.rolelist()).c_str(), roArg.mapid(), roArg.destsceneid());
	}
	else
	{
		LogError("Role [%s] begin to change scene to tid [%u] sid [%u] failed, error [%d]", Uint64List2Str(roArg.rolelist()).c_str(), roArg.mapid(), roArg.destsceneid(), roRes.result());
	}
}

void RpcM2N_ChangeSceneFromMs::OnTimeout(const ChangeSceneFromMsArg &roArg, const CUserData &roUserData)
{
}
