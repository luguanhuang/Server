#ifndef __RPCM2G_WEDDINGOPERM2G_H__
#define __RPCM2G_WEDDINGOPERM2G_H__

// generate by ProtoGen at date: 2017/8/1 0:23:57

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_WEDDINGOPERM2G_TYPE 64766

class RpcM2G_WeddingOperM2G : public CRpc
{
private:

	RpcM2G_WeddingOperM2G():CRpc(RPCM2G_WEDDINGOPERM2G_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_WeddingOperM2G *CreateRpc()
	{
		return new RpcM2G_WeddingOperM2G;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const WeddingOperM2GArg &roArg, const WeddingOperM2GRes &roRes, const CUserData &roUserData);
	void OnTimeout(const WeddingOperM2GArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const WeddingOperM2GArg &roArg, WeddingOperM2GRes &roRes);
	void OnDelayReplyRpc(const WeddingOperM2GArg &roArg, WeddingOperM2GRes &roRes, const CUserData &roUserData);

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
	WeddingOperM2GArg m_oArg;
	WeddingOperM2GRes m_oRes;
};

#endif
