#include "pch.h"
#include "garden/rpcm2d_querygardeninfo.h"
#include "garden/garden_manager.h"
// generate by ProtoGen at date: 2017/4/27 10:59:37

RPC_CLIENT_IMPLEMETION(RpcM2D_QueryGardenInfo, QueryGardenInfoArg, QueryGardenInfoRes)

void RpcM2D_QueryGardenInfo::OnReply(const QueryGardenInfoArg &roArg, const QueryGardenInfoRes &roRes, const CUserData &roUserData)
{
	if (KKSG::ERR_SUCCESS == roRes.result())
	{		
		GardenManager::Instance()->LoadFromDB(roRes.garden_info());
	}
	else
	{
		SSWarn<<"read garden failed, roleid"<<roArg.role_id()<<END;
	}

}

void RpcM2D_QueryGardenInfo::OnTimeout(const QueryGardenInfoArg &roArg, const CUserData &roUserData)
{
}
