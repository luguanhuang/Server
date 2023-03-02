#include "pch.h"
#include "battlewatch/rpcg2m_getwatchinfobytype.h"
#include "live/livemanager.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2016/7/29 17:09:29

RPC_SERVER_IMPLEMETION(RpcG2M_GetWatchInfoByType, GetWatchInfoByTypeArg, GetWatchInfoByTypeRes)

void RpcG2M_GetWatchInfoByType::OnCall(const GetWatchInfoByTypeArg &roArg, GetWatchInfoByTypeRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		roRes.set_error((ErrorCode)ERR_UNKNOWN);
		return;
	}

	UINT32 error = LiveManager::Instance()->FillLiveInfo(role, roArg.livetype(), roRes);

	roRes.set_error((ErrorCode)error);
}

void RpcG2M_GetWatchInfoByType::OnDelayReplyRpc(const GetWatchInfoByTypeArg &roArg, GetWatchInfoByTypeRes &roRes, const CUserData &roUserData)
{
}
