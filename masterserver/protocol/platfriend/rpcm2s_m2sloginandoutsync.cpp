#include "pch.h"
#include "platfriend/rpcm2s_m2sloginandoutsync.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "platforminfo/platformfriendmgr.h"

// generate by ProtoGen at date: 2016/10/26 15:44:39

RPC_CLIENT_IMPLEMETION(RpcM2S_M2SLoginAndoutSync, M2SLoginAndoutSyncArg, M2SLoginAndoutSyncRes)

void RpcM2S_M2SLoginAndoutSync::OnReply(const M2SLoginAndoutSyncArg &roArg, const M2SLoginAndoutSyncRes &roRes, const CUserData &roUserData)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		return;
	}

	CPlatformFriendMgr::Instance()->LoginAndOutSyncFromCS(role, roRes);
}

void RpcM2S_M2SLoginAndoutSync::OnTimeout(const M2SLoginAndoutSyncArg &roArg, const CUserData &roUserData)
{
}
