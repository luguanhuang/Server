#ifndef __RPCM2G_HOLDWEDDINGCHECKM2G_H__
#define __RPCM2G_HOLDWEDDINGCHECKM2G_H__

// generate by ProtoGen at date: 2017/8/4 15:21:51

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_HOLDWEDDINGCHECKM2G_TYPE 37621

class RpcM2G_Holdweddingcheckm2g : public CRpc
{
private:

	RpcM2G_Holdweddingcheckm2g():CRpc(RPCM2G_HOLDWEDDINGCHECKM2G_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_Holdweddingcheckm2g *CreateRpc()
	{
		return new RpcM2G_Holdweddingcheckm2g;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const Holdweddingcheckm2gArg &roArg, const Holdweddingcheckm2gRes &roRes, const CUserData &roUserData);
	void OnTimeout(const Holdweddingcheckm2gArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const Holdweddingcheckm2gArg &roArg, Holdweddingcheckm2gRes &roRes);
	void OnDelayReplyRpc(const Holdweddingcheckm2gArg &roArg, Holdweddingcheckm2gRes &roRes, const CUserData &roUserData);

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
	Holdweddingcheckm2gArg m_oArg;
	Holdweddingcheckm2gRes m_oRes;
};

#endif
