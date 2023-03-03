#ifndef __RPCC2G_GETGODDESSTRIALREWARDS_H__
#define __RPCC2G_GETGODDESSTRIALREWARDS_H__

// generate by ProtoGen at date: 2016/7/18 20:04:49

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GETGODDESSTRIALREWARDS_TYPE 41420

class RpcC2G_GetGoddessTrialRewards : public CRpc
{
private:

	RpcC2G_GetGoddessTrialRewards():CRpc(RPCC2G_GETGODDESSTRIALREWARDS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GetGoddessTrialRewards *CreateRpc()
	{
		return new RpcC2G_GetGoddessTrialRewards;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGoddessTrialRewardsArg &roArg, const GetGoddessTrialRewardsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGoddessTrialRewardsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGoddessTrialRewardsArg &roArg, GetGoddessTrialRewardsRes &roRes);
	void OnDelayReplyRpc(const GetGoddessTrialRewardsArg &roArg, GetGoddessTrialRewardsRes &roRes, const CUserData &roUserData);

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
	GetGoddessTrialRewardsArg m_oArg;
	GetGoddessTrialRewardsRes m_oRes;
};

#endif
