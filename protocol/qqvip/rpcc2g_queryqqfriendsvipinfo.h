#ifndef __RPCC2G_QUERYQQFRIENDSVIPINFO_H__
#define __RPCC2G_QUERYQQFRIENDSVIPINFO_H__

// generate by ProtoGen at date: 2016/12/4 20:26:56

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_QUERYQQFRIENDSVIPINFO_TYPE 11531

class RpcC2G_QueryQQFriendsVipInfo : public CRpc
{
private:

	RpcC2G_QueryQQFriendsVipInfo():CRpc(RPCC2G_QUERYQQFRIENDSVIPINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_QueryQQFriendsVipInfo *CreateRpc()
	{
		return new RpcC2G_QueryQQFriendsVipInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryQQFriendsVipInfoArg &roArg, const QueryQQFriendsVipInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryQQFriendsVipInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryQQFriendsVipInfoArg &roArg, QueryQQFriendsVipInfoRes &roRes);
	void OnDelayReplyRpc(const QueryQQFriendsVipInfoArg &roArg, QueryQQFriendsVipInfoRes &roRes, const CUserData &roUserData);

	virtual void OnReplyTimeout() 
	{ 
		OnTimeout(m_oArg, m_oUser); 
	}

	virtual void OnDelayRpc(const CUserData &userData)
	{
		OnDelayReplyRpc( m_oArg, m_oRes, userData);
	}

	virtual CRpc *NewRpc() 
	{
		return CreateRpc();
	}

public:
	QueryQQFriendsVipInfoArg m_oArg;
	QueryQQFriendsVipInfoRes m_oRes;
};

#endif
