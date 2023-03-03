#ifndef __RPCC2G_DEPROGRESSREQ_H__
#define __RPCC2G_DEPROGRESSREQ_H__

// generate by ProtoGen at date: 2016/9/3 17:29:16

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_DEPROGRESSREQ_TYPE 5238

class RpcC2G_DEProgressReq : public CRpc
{
private:

	RpcC2G_DEProgressReq():CRpc(RPCC2G_DEPROGRESSREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_DEProgressReq *CreateRpc()
	{
		return new RpcC2G_DEProgressReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DEProgressArg &roArg, const DEProgressRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DEProgressArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DEProgressArg &roArg, DEProgressRes &roRes);
	void OnDelayReplyRpc(const DEProgressArg &roArg, DEProgressRes &roRes, const CUserData &roUserData);

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
	DEProgressArg m_oArg;
	DEProgressRes m_oRes;
};

#endif
