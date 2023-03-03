#ifndef __RPCC2G_DHRREQC2G_H__
#define __RPCC2G_DHRREQC2G_H__

// generate by ProtoGen at date: 2017/6/14 21:17:07

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_DHRREQC2G_TYPE 12451

class RpcC2G_DHRReqC2G : public CRpc
{
private:

	RpcC2G_DHRReqC2G():CRpc(RPCC2G_DHRREQC2G_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_DHRReqC2G *CreateRpc()
	{
		return new RpcC2G_DHRReqC2G;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const DHRArg &roArg, const DHRRes &roRes, const CUserData &roUserData);
	void OnTimeout(const DHRArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const DHRArg &roArg, DHRRes &roRes);
	void OnDelayReplyRpc(const DHRArg &roArg, DHRRes &roRes, const CUserData &roUserData);

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
	DHRArg m_oArg;
	DHRRes m_oRes;
};

#endif
