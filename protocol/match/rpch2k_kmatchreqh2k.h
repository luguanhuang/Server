#ifndef __RPCH2K_KMATCHREQH2K_H__
#define __RPCH2K_KMATCHREQH2K_H__

// generate by ProtoGen at date: 2017/5/5 21:16:47

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCH2K_KMATCHREQH2K_TYPE 55697

class RpcH2K_KMatchReqh2k : public CRpc
{
private:

	RpcH2K_KMatchReqh2k():CRpc(RPCH2K_KMATCHREQH2K_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcH2K_KMatchReqh2k *CreateRpc()
	{
		return new RpcH2K_KMatchReqh2k;
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
