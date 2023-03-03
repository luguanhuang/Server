#ifndef __RPCM2G_REQRECVFATIGUE_H__
#define __RPCM2G_REQRECVFATIGUE_H__

// generate by ProtoGen at date: 2016/9/19 10:23:40

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_REQRECVFATIGUE_TYPE 46479

class RpcM2G_ReqRecvFatigue : public CRpc
{
private:

	RpcM2G_ReqRecvFatigue():CRpc(RPCM2G_REQRECVFATIGUE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_ReqRecvFatigue *CreateRpc()
	{
		return new RpcM2G_ReqRecvFatigue;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqRecvFatigueArg &roArg, const ReqRecvFatigueRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqRecvFatigueArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqRecvFatigueArg &roArg, ReqRecvFatigueRes &roRes);
	void OnDelayReplyRpc(const ReqRecvFatigueArg &roArg, ReqRecvFatigueRes &roRes, const CUserData &roUserData);

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
	ReqRecvFatigueArg m_oArg;
	ReqRecvFatigueRes m_oRes;
};

#endif
