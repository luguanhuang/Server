#ifndef __RPCG2M_MSGETFLWOERRANKREWARD_H__
#define __RPCG2M_MSGETFLWOERRANKREWARD_H__

// generate by ProtoGen at date: 2016/8/18 11:39:18

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_MSGETFLWOERRANKREWARD_TYPE 55481

class RpcG2M_MSGetFlwoerRankReward : public CRpc
{
private:

	RpcG2M_MSGetFlwoerRankReward():CRpc(RPCG2M_MSGETFLWOERRANKREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_MSGetFlwoerRankReward *CreateRpc()
	{
		return new RpcG2M_MSGetFlwoerRankReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const MSGetFlowerRewardArg &roArg, const MSGetFlowerRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const MSGetFlowerRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const MSGetFlowerRewardArg &roArg, MSGetFlowerRewardRes &roRes);
	void OnDelayReplyRpc(const MSGetFlowerRewardArg &roArg, MSGetFlowerRewardRes &roRes, const CUserData &roUserData);

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
	MSGetFlowerRewardArg m_oArg;
	MSGetFlowerRewardRes m_oRes;
};

#endif
