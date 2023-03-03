#ifndef __RPCC2G_GETPAYREWARD_H__
#define __RPCC2G_GETPAYREWARD_H__

// generate by ProtoGen at date: 2017/2/7 19:31:27

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETPAYREWARD_TYPE 63038

class RpcC2G_GetPayReward : public CRpc
{
private:

	RpcC2G_GetPayReward():CRpc(RPCC2G_GETPAYREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetPayReward *CreateRpc()
	{
		return new RpcC2G_GetPayReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetPayRewardArg &roArg, const GetPayRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetPayRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetPayRewardArg &roArg, GetPayRewardRes &roRes);
	void OnDelayReplyRpc(const GetPayRewardArg &roArg, GetPayRewardRes &roRes, const CUserData &roUserData);

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
	GetPayRewardArg m_oArg;
	GetPayRewardRes m_oRes;
};

#endif
