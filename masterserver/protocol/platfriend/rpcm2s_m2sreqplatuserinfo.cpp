#include "pch.h"
#include "platfriend/rpcm2s_m2sreqplatuserinfo.h"
#include "platforminfo/platformfriendmgr.h"

// generate by ProtoGen at date: 2016/10/26 15:02:26

RPC_CLIENT_IMPLEMETION(RpcM2S_M2SReqPlatUserInfo, M2SReqPlatUserInfoArg, M2SReqPlatUserInfoRes)

void RpcM2S_M2SReqPlatUserInfo::OnReply(const M2SReqPlatUserInfoArg &roArg, const M2SReqPlatUserInfoRes &roRes, const CUserData &roUserData)
{
	if (roArg.rpcid())
	{
		CUserData userData;
		userData.m_dwUserData = roRes.error();
		userData.m_pUserPtr = (void*)&roRes;

		CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
	}
	else
	{
		PlatFriendList* list = CPlatformFriendMgr::Instance()->GetPlatFriendListByOpenID(roArg.sourceopenid());

		list->SyncPlatFriendListFromCS(roRes);

		CPlatformFriendMgr::Instance()->RemoveClientReq2CS(roArg.sourceopenid());

		CPlatformFriendMgr::Instance()->OnLoginReceiveDetailPlatInfoFromCs(roArg.sourceopenid());

		LogDebug("%s query platfriendinfo from cs", roArg.sourceopenid().c_str());
	}
}

void RpcM2S_M2SReqPlatUserInfo::OnTimeout(const M2SReqPlatUserInfoArg &roArg, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = KKSG::ERR_SUCCESS;

	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}
