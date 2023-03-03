#ifndef __RPCC2G_REFRESHSWEEPREWARD_H__
#define __RPCC2G_REFRESHSWEEPREWARD_H__

// generate by ProtoGen at date: 2016/9/12 11:51:44

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_REFRESHSWEEPREWARD_TYPE 38012

class RpcC2G_RefreshSweepReward : public CRpc
{
private:

	RpcC2G_RefreshSweepReward():CRpc(RPCC2G_REFRESHSWEEPREWARD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_RefreshSweepReward *CreateRpc()
	{
		return new RpcC2G_RefreshSweepReward;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RefreshSweepRewardArg &roArg, const RefreshSweepRewardRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RefreshSweepRewardArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RefreshSweepRewardArg &roArg, RefreshSweepRewardRes &roRes);
	void OnDelayReplyRpc(const RefreshSweepRewardArg &roArg, RefreshSweepRewardRes &roRes, const CUserData &roUserData);

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
	RefreshSweepRewardArg m_oArg;
	RefreshSweepRewardRes m_oRes;
};

#endif
