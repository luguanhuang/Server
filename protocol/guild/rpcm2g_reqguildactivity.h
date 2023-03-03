#ifndef __RPCM2G_REQGUILDACTIVITY_H__
#define __RPCM2G_REQGUILDACTIVITY_H__

// generate by ProtoGen at date: 2016/9/16 16:40:03

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_REQGUILDACTIVITY_TYPE 35709

class RpcM2G_ReqGuildActivity : public CRpc
{
private:

	RpcM2G_ReqGuildActivity():CRpc(RPCM2G_REQGUILDACTIVITY_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_ReqGuildActivity *CreateRpc()
	{
		return new RpcM2G_ReqGuildActivity;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReqGuildActivityAgr &roArg, const ReqGuildActivityRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReqGuildActivityAgr &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReqGuildActivityAgr &roArg, ReqGuildActivityRes &roRes);
	void OnDelayReplyRpc(const ReqGuildActivityAgr &roArg, ReqGuildActivityRes &roRes, const CUserData &roUserData);

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
	ReqGuildActivityAgr m_oArg;
	ReqGuildActivityRes m_oRes;
};

#endif
