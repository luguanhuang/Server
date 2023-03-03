#ifndef __RPCC2M_KMATCHCOMMONREQ_H__
#define __RPCC2M_KMATCHCOMMONREQ_H__

// generate by ProtoGen at date: 2017/5/5 15:00:45

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_KMATCHCOMMONREQ_TYPE 57822

class RpcC2M_KMatchCommonReq : public CRpc
{
private:

	RpcC2M_KMatchCommonReq():CRpc(RPCC2M_KMATCHCOMMONREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_KMatchCommonReq *CreateRpc()
	{
		return new RpcC2M_KMatchCommonReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const KMatchCommonArg &roArg, const KMatchCommonRes &roRes, const CUserData &roUserData);
	void OnTimeout(const KMatchCommonArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const KMatchCommonArg &roArg, KMatchCommonRes &roRes);
	void OnDelayReplyRpc(const KMatchCommonArg &roArg, KMatchCommonRes &roRes, const CUserData &roUserData);

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
	KMatchCommonArg m_oArg;
	KMatchCommonRes m_oRes;
};

#endif
