#ifndef __RPCT2G_RECONNECTGS_H__
#define __RPCT2G_RECONNECTGS_H__

// generate by ProtoGen at date: 2016/10/15 20:05:37

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCT2G_RECONNECTGS_TYPE 20847

class RpcT2G_ReconnectGs : public CRpc
{
private:

	RpcT2G_ReconnectGs():CRpc(RPCT2G_RECONNECTGS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcT2G_ReconnectGs *CreateRpc()
	{
		return new RpcT2G_ReconnectGs;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReconnectGsArg &roArg, const ReconnectGsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReconnectGsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReconnectGsArg &roArg, ReconnectGsRes &roRes);
	void OnDelayReplyRpc(const ReconnectGsArg &roArg, ReconnectGsRes &roRes, const CUserData &roUserData);

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
	ReconnectGsArg m_oArg;
	ReconnectGsRes m_oRes;
};

#endif
