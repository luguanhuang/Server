#ifndef __RPCM2G_GCFCOMMONREQM2G_H__
#define __RPCM2G_GCFCOMMONREQM2G_H__

// generate by ProtoGen at date: 2016/12/31 22:32:09

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_GCFCOMMONREQM2G_TYPE 155

class RpcM2G_GCFCommonReqM2G : public CRpc
{
private:

	RpcM2G_GCFCommonReqM2G():CRpc(RPCM2G_GCFCOMMONREQM2G_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_GCFCommonReqM2G *CreateRpc()
	{
		return new RpcM2G_GCFCommonReqM2G;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GCFCommonArgM2G &roArg, const GCFCommonResM2G &roRes, const CUserData &roUserData);
	void OnTimeout(const GCFCommonArgM2G &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GCFCommonArgM2G &roArg, GCFCommonResM2G &roRes);
	void OnDelayReplyRpc(const GCFCommonArgM2G &roArg, GCFCommonResM2G &roRes, const CUserData &roUserData);

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
	GCFCommonArgM2G m_oArg;
	GCFCommonResM2G m_oRes;
};

#endif
