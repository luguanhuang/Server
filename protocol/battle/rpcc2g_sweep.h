#ifndef __RPCC2G_SWEEP_H__
#define __RPCC2G_SWEEP_H__

// generate by ProtoGen at date: 2015/7/13 14:28:32

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_SWEEP_TYPE 6019

class RpcC2G_Sweep : public CRpc
{
private:

	RpcC2G_Sweep():CRpc(RPCC2G_SWEEP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_Sweep *CreateRpc()
	{
		return new RpcC2G_Sweep;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SweepArg &roArg, const SweepRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SweepArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SweepArg &roArg, SweepRes &roRes);
	void OnDelayReplyRpc(const SweepArg &roArg, SweepRes &roRes, const CUserData &roUserData);

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
	SweepArg m_oArg;
	SweepRes m_oRes;
};

#endif
