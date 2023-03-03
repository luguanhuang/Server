#ifndef __RPCC2M_GCFFIGHTINFOREQC2M_H__
#define __RPCC2M_GCFFIGHTINFOREQC2M_H__

// generate by ProtoGen at date: 2016/12/20 10:29:26

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_GCFFIGHTINFOREQC2M_TYPE 42852

class RpcC2M_GCFFightInfoReqC2M : public CRpc
{
private:

	RpcC2M_GCFFightInfoReqC2M():CRpc(RPCC2M_GCFFIGHTINFOREQC2M_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_GCFFightInfoReqC2M *CreateRpc()
	{
		return new RpcC2M_GCFFightInfoReqC2M;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GCFFightInfoArg &roArg, const GCFFightInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GCFFightInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GCFFightInfoArg &roArg, GCFFightInfoRes &roRes);
	void OnDelayReplyRpc(const GCFFightInfoArg &roArg, GCFFightInfoRes &roRes, const CUserData &roUserData);

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
	GCFFightInfoArg m_oArg;
	GCFFightInfoRes m_oRes;
};

#endif
