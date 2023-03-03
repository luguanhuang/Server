#ifndef __RPCC2G_USESUPPLEMENT_H__
#define __RPCC2G_USESUPPLEMENT_H__

// generate by ProtoGen at date: 2016/2/22 16:49:39

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_USESUPPLEMENT_TYPE 20068

class RpcC2G_UseSupplement : public CRpc
{
private:

	RpcC2G_UseSupplement():CRpc(RPCC2G_USESUPPLEMENT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_UseSupplement *CreateRpc()
	{
		return new RpcC2G_UseSupplement;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const UseSupplementReq &roArg, const UseSupplementRes &roRes, const CUserData &roUserData);
	void OnTimeout(const UseSupplementReq &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const UseSupplementReq &roArg, UseSupplementRes &roRes);
	void OnDelayReplyRpc(const UseSupplementReq &roArg, UseSupplementRes &roRes, const CUserData &roUserData);

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
	UseSupplementReq m_oArg;
	UseSupplementRes m_oRes;
};

#endif
