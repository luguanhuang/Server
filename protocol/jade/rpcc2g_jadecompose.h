#ifndef __RPCC2G_JADECOMPOSE_H__
#define __RPCC2G_JADECOMPOSE_H__

// generate by ProtoGen at date: 2015/6/17 16:35:01

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_JADECOMPOSE_TYPE 35616

class RpcC2G_JadeCompose : public CRpc
{
private:

	RpcC2G_JadeCompose():CRpc(RPCC2G_JADECOMPOSE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_JadeCompose *CreateRpc()
	{
		return new RpcC2G_JadeCompose;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const JadeComposeArg &roArg, const JadeComposeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const JadeComposeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const JadeComposeArg &roArg, JadeComposeRes &roRes);
	void OnDelayReplyRpc(const JadeComposeArg &roArg, JadeComposeRes &roRes, const CUserData &roUserData);

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
	JadeComposeArg m_oArg;
	JadeComposeRes m_oRes;
};

#endif
