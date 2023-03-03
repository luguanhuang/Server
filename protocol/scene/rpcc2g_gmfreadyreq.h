#ifndef __RPCC2G_GMFREADYREQ_H__
#define __RPCC2G_GMFREADYREQ_H__

// generate by ProtoGen at date: 2016/10/1 16:23:39

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GMFREADYREQ_TYPE 12219

class RpcC2G_GmfReadyReq : public CRpc
{
private:

	RpcC2G_GmfReadyReq():CRpc(RPCC2G_GMFREADYREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GmfReadyReq *CreateRpc()
	{
		return new RpcC2G_GmfReadyReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GmfReadyArg &roArg, const GmfReadyRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GmfReadyArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GmfReadyArg &roArg, GmfReadyRes &roRes);
	void OnDelayReplyRpc(const GmfReadyArg &roArg, GmfReadyRes &roRes, const CUserData &roUserData);

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
	GmfReadyArg m_oArg;
	GmfReadyRes m_oRes;
};

#endif
