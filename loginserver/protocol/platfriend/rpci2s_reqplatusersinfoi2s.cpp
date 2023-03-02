#include "pch.h"
#include "platfriend/rpci2s_reqplatusersinfoi2s.h"
#include "verify/logincontrol.h"
#include "verify/TXPlatFriendTask.h"

// generate by ProtoGen at date: 2017/6/2 21:05:27

RPC_CLIENT_IMPLEMETION(RpcI2S_ReqPlatUsersInfoI2S, M2SReqPlatUserInfoArg, M2SReqPlatUserInfoRes)

template<class T>
struct Compare
{
	bool operator()(const KKSG::PlatUserAllInfo& aObj, const KKSG::PlatUserAllInfo& bObj) const
	{
		const KKSG::PlatUserAllInfo* a = &aObj;
		const KKSG::PlatUserAllInfo* b = &bObj;
		if (strcmp(a->openid().c_str(), b->openid().c_str()) == 0)
		{
			return false;
		}
		if (a->platuserdetailinfo().level() > b->platuserdetailinfo().level())
		{
			return true;
		}
		else if (a->platuserdetailinfo().level() == b->platuserdetailinfo().level())
		{
			if (a->platuserdetailinfo().roleid() < b->platuserdetailinfo().roleid())
			{
				return true;
			}
		}

		return false;
	}
};

void RpcI2S_ReqPlatUsersInfoI2S::OnReply(const M2SReqPlatUserInfoArg &roArg, const M2SReqPlatUserInfoRes &roRes, const CUserData &roUserData)
{
	LoginInfo& info = CLoginControl::Instance()->GetLoginInfo(roArg.sourceopenid());
	if (roRes.error() == KKSG::ERR_SUCCESS)
	{
		std::set<KKSG::PlatUserAllInfo, Compare<KKSG::PlatUserAllInfo>> tempSet;
		for (int i = 0; i < roRes.userinfos_size(); ++i)
		{
			tempSet.insert(roRes.userinfos(i));
		}

		int num = 0;
		for (auto iter = tempSet.begin(); iter != tempSet.end() && num < MAX_FRIEND_CLIENTNUM; ++iter, ++num)
		{
			const KKSG::PlatUserAllInfo& userInfo = *iter;
			KKSG::PlatFriendServer serverInfo;
			serverInfo.set_openid(userInfo.openid());
			serverInfo.set_serverid(userInfo.platuserdetailinfo().serverid());
			serverInfo.set_rolename(userInfo.platuserdetailinfo().rolename());
			serverInfo.set_level(userInfo.platuserdetailinfo().level());

			info.userGateInfo.friendServer.push_back(serverInfo);
		}
		info.SetFinish(LoginFlowTask_PlatFriend, TaskStatus_Success);
	}
	else
	{
		info.SetFinish(LoginFlowTask_PlatFriend, TaskStatus_Failed);
	}

}

void RpcI2S_ReqPlatUsersInfoI2S::OnTimeout(const M2SReqPlatUserInfoArg &roArg, const CUserData &roUserData)
{
	CLoginControl::Instance()->GetLoginInfo(roArg.sourceopenid()).SetFinish(LoginFlowTask_PlatFriend, TaskStatus_Failed);
}
