#ifndef __RPCM2K_KMATCHREQM2K_H__
#define __RPCM2K_KMATCHREQM2K_H__

// generate by ProtoGen at date: 2017/5/5 21:17:38

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2K_KMATCHREQM2K_TYPE 21913

class RpcM2K_KMatchReqm2k : public CRpc
{
private:

	RpcM2K_KMatchReqm2k():CRpc(RPCM2K_KMATCHREQM2K_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2K_KMatchReqm2k *CreateRpc()
	{
		return new RpcM2K_KMatchReqm2k;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const KMatchTransArg &roArg, const KMatchCommonRes &roRes, const CUserData &roUserData);
	void OnTimeout(const KMatchTransArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const KMatchTransArg &roArg, KMatchCommonRes &roRes);
	void OnDelayReplyRpc(const KMatchTransArg &roArg, KMatchCommonRes &roRes, const CUserData &roUserData);

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
	KMatchTransArg m_oArg;
	KMatchCommonRes m_oRes;
};

#endif
