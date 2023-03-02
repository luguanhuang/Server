#include "pch.h"
#include "scene/rpcg2n_changesceneverify.h"
#include "logutil.h"
#include "config.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2016/7/14 19:52:27

RPC_CLIENT_IMPLEMETION(RpcG2N_ChangeSceneVerify, ChangeSceneVerifyArg, ChangeSceneVerifyRes)

void ClearRoleVerifyFlag(const ChangeSceneVerifyArg &roArg)
{
	for(INT32 i = 0; i < roArg.rolelist_size(); ++i)
	{
		UINT64 qwRoleID = roArg.rolelist(i);
		Role* poRole = RoleManager::Instance()->FindByRoleID(qwRoleID);
		if(poRole != NULL)
		{
			poRole->SetFlag(RSF_ISSceneSwitch_Verifying, false);
		}
	}
}

void RpcG2N_ChangeSceneVerify::OnReply(const ChangeSceneVerifyArg &roArg, const ChangeSceneVerifyRes &roRes, const CUserData &roUserData)
{
	ClearRoleVerifyFlag(roArg);

	if(roRes.result() != KKSG::ERR_SUCCESS)
	{
		LogError("Role [%s] change scene verify failed, error [%d]", Uint64List2Str(roArg.rolelist()).c_str(), roRes.result());
		return;
	}

	LogDebug("Role [%s] change scene verify success, dest line [%u]", Uint64List2Str(roArg.rolelist()).c_str(), roRes.destline());

	//同一个GS上的，则直接切换
	//if(GSConfig::Instance()->GetLine() == roRes.destline())
	{


	}
}

void RpcG2N_ChangeSceneVerify::OnTimeout(const ChangeSceneVerifyArg &roArg, const CUserData &roUserData)
{
	ClearRoleVerifyFlag(roArg);
	LogWarn("Role [%s] change scene verify timeout", Uint64List2Str(roArg.rolelist()).c_str());
}
