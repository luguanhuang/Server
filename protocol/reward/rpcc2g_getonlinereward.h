#ifndef __RPCC2G_GETONLINEREWARD_H__
#define __RPCC2G_GETONLINEREWARD_H__

// generate by ProtoGen at date: 2015/10/20 18:38:39

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETONLINEREWARD_TYPE 21137

class RpcC2G_GetOnlineReward : public CRpc
{
private:

	RpcC2G_GetOnlineReward():CRpc(RPCC2G_GETONLINEREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetOnlineReward *CreateRpc()
	{
		return new RpcC2G_GetOnlineReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetOnlineRewardArg &roArg, const GetOnlineRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetOnlineRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetOnlineRewardArg &roArg, GetOnlineRewardRes &roRes);
	void OnDelayReplyRpc(const GetOnlineRewardArg &roArg, GetOnlineRewardRes &roRes, const CUserData &roUserData);

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
	GetOnlineRewardArg m_oArg;
	GetOnlineRewardRes m_oRes;
};

#endif
