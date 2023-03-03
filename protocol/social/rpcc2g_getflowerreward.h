#ifndef __RPCC2G_GETFLOWERREWARD_H__
#define __RPCC2G_GETFLOWERREWARD_H__

// generate by ProtoGen at date: 2016/6/25 17:04:39

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETFLOWERREWARD_TYPE 65090

class RpcC2G_GetFlowerReward : public CRpc
{
private:

	RpcC2G_GetFlowerReward():CRpc(RPCC2G_GETFLOWERREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetFlowerReward *CreateRpc()
	{
		return new RpcC2G_GetFlowerReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetFlowerRewardArg &roArg, const GetFlowerRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetFlowerRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetFlowerRewardArg &roArg, GetFlowerRewardRes &roRes);
	void OnDelayReplyRpc(const GetFlowerRewardArg &roArg, GetFlowerRewardRes &roRes, const CUserData &roUserData);

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
	GetFlowerRewardArg m_oArg;
	GetFlowerRewardRes m_oRes;
};

#endif
