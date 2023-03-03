#ifndef __RPCC2G_GETSYSTEMREWARD_H__
#define __RPCC2G_GETSYSTEMREWARD_H__

// generate by ProtoGen at date: 2015/7/9 20:54:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETSYSTEMREWARD_TYPE 11595

class RpcC2G_GetSystemReward : public CRpc
{
private:

	RpcC2G_GetSystemReward():CRpc(RPCC2G_GETSYSTEMREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetSystemReward *CreateRpc()
	{
		return new RpcC2G_GetSystemReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetSystemRewardArg &roArg, const GetSystemRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetSystemRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetSystemRewardArg &roArg, GetSystemRewardRes &roRes);
	void OnDelayReplyRpc(const GetSystemRewardArg &roArg, GetSystemRewardRes &roRes, const CUserData &roUserData);

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
	GetSystemRewardArg m_oArg;
	GetSystemRewardRes m_oRes;
};

#endif
