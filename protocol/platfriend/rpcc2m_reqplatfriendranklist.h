#ifndef __RPCC2M_REQPLATFRIENDRANKLIST_H__
#define __RPCC2M_REQPLATFRIENDRANKLIST_H__

// generate by ProtoGen at date: 2016/10/26 12:04:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_REQPLATFRIENDRANKLIST_TYPE 43806

class RpcC2M_ReqPlatFriendRankList : public CRpc
{
private:

	RpcC2M_ReqPlatFriendRankList():CRpc(RPCC2M_REQPLATFRIENDRANKLIST_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_ReqPlatFriendRankList *CreateRpc()
	{
		return new RpcC2M_ReqPlatFriendRankList;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqPlatFriendRankListArg &roArg, const ReqPlatFriendRankListRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqPlatFriendRankListArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqPlatFriendRankListArg &roArg, ReqPlatFriendRankListRes &roRes);
	void OnDelayReplyRpc(const ReqPlatFriendRankListArg &roArg, ReqPlatFriendRankListRes &roRes, const CUserData &roUserData);

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
	ReqPlatFriendRankListArg m_oArg;
	ReqPlatFriendRankListRes m_oRes;
};

#endif
