#ifndef __RPCC2M_GCFCOMMONREQ_H__
#define __RPCC2M_GCFCOMMONREQ_H__

// generate by ProtoGen at date: 2016/12/28 19:16:51

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GCFCOMMONREQ_TYPE 28945

class RpcC2M_GCFCommonReq : public CRpc
{
private:

	RpcC2M_GCFCommonReq():CRpc(RPCC2M_GCFCOMMONREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GCFCommonReq *CreateRpc()
	{
		return new RpcC2M_GCFCommonReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GCFCommonArg &roArg, const GCFCommonRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GCFCommonArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GCFCommonArg &roArg, GCFCommonRes &roRes);
	void OnDelayReplyRpc(const GCFCommonArg &roArg, GCFCommonRes &roRes, const CUserData &roUserData);

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
	GCFCommonArg m_oArg;
	GCFCommonRes m_oRes;
};

#endif
