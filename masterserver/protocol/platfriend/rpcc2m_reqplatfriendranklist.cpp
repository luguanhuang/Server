#include "pch.h"
#include "platfriend/rpcc2m_reqplatfriendranklist.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "platforminfo/platformfriendmgr.h"
#include "platfriend/rpcm2s_m2sreqplatuserinfo.h"
#include "network/cslink.h"
#include "pb/project.pb.h"

// generate by ProtoGen at date: 2016/10/26 12:04:08

RPC_SERVER_IMPLEMETION(RpcC2M_ReqPlatFriendRankList, ReqPlatFriendRankListArg, ReqPlatFriendRankListRes)

void RpcC2M_ReqPlatFriendRankList::OnCall(const ReqPlatFriendRankListArg &roArg, ReqPlatFriendRankListRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		return;
	}

	UINT32 error = CPlatformFriendMgr::Instance()->GetPlatFriendRankList2Client(role, roRes);
	if (error != KKSG::ERR_SUCCESS)
	{
		if (CPlatformFriendMgr::Instance()->IsClientReq2CSFull())
		{
			//繁忙
			roRes.set_error(KKSG::ERR_SUCCESS);
			return;
		}
		if (CSLink::Instance()->GetUsingCenterServerID() == INVALID_CONN_ID)
		{
			roRes.set_error(KKSG::ERR_SUCCESS);
			LogDebug("center server not open");
			return;
		}
		//send rpc
		PlatFriendList* list = CPlatformFriendMgr::Instance()->GetPlatFriendList(role);
		std::vector<std::string> platFriends;
		list->GetPlatFriends(platFriends);
		RpcM2S_M2SReqPlatUserInfo* rpc = RpcM2S_M2SReqPlatUserInfo::CreateRpc();
		rpc->m_sessionID = m_sessionID;
		rpc->m_oArg.set_sourceopenid(role->GetAccount());
		rpc->m_oArg.set_rpcid(CRpc::DelayRpc());
		for (auto iter = platFriends.begin(); iter != platFriends.end(); ++iter)
		{
			rpc->m_oArg.add_openid(*iter);
		}
		CSLink::Instance()->SendTo(*rpc);
		CPlatformFriendMgr::Instance()->AddClientReq2CS(role->GetAccount());

		LogDebug("%llu, %s request platfriendlist to cs", role->GetID(), role->GetAccount().c_str());
	}
}

void RpcC2M_ReqPlatFriendRankList::OnDelayReplyRpc(const ReqPlatFriendRankListArg &roArg, ReqPlatFriendRankListRes &roRes, const CUserData &roUserData)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!role)
	{
		return;
	}
	CPlatformFriendMgr::Instance()->RemoveClientReq2CS(role->GetAccount());
	UINT32 error = roUserData.m_dwUserData;
	roRes.set_error((KKSG::ErrorCode)error);
	if (error != KKSG::ERR_SUCCESS)
	{
		return;
	}

	if(!roUserData.m_pUserPtr)
		return;
	KKSG::M2SReqPlatUserInfoRes* result = (KKSG::M2SReqPlatUserInfoRes*)(roUserData.m_pUserPtr);
	CPlatformFriendMgr::Instance()->SyncPlatFriendListFromCSAndFillRank(role, *result, roRes);
	LogDebug("%llu, %s, got platfriendlist from cs, size:%u", role->GetID(), role->GetAccount().c_str(), (*result).userinfos_size());
}
