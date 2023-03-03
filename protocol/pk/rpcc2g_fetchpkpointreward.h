#ifndef __RPCC2G_FETCHPKPOINTREWARD_H__
#define __RPCC2G_FETCHPKPOINTREWARD_H__

// generate by ProtoGen at date: 2015/12/16 12:20:51

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_FETCHPKPOINTREWARD_TYPE 44397

class RpcC2G_FetchPkPointReward : public CRpc
{
private:

	RpcC2G_FetchPkPointReward():CRpc(RPCC2G_FETCHPKPOINTREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_FetchPkPointReward *CreateRpc()
	{
		return new RpcC2G_FetchPkPointReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FetchPkPointRewardReq &roArg, const FetchPkPointRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FetchPkPointRewardReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FetchPkPointRewardReq &roArg, FetchPkPointRewardRes &roRes);
	void OnDelayReplyRpc(const FetchPkPointRewardReq &roArg, FetchPkPointRewardRes &roRes, const CUserData &roUserData);

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
	FetchPkPointRewardReq m_oArg;
	FetchPkPointRewardRes m_oRes;
};

#endif
