#ifndef __RPCC2G_MULACTIVITYREQ_H__
#define __RPCC2G_MULACTIVITYREQ_H__

// generate by ProtoGen at date: 2016/7/9 14:32:04

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_MULACTIVITYREQ_TYPE 22806

class RpcC2G_MulActivityReq : public CRpc
{
private:

	RpcC2G_MulActivityReq():CRpc(RPCC2G_MULACTIVITYREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_MulActivityReq *CreateRpc()
	{
		return new RpcC2G_MulActivityReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const MulActivityArg &roArg, const MulActivityRes &roRes, const CUserData &roUserData);
	void OnTimeout(const MulActivityArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const MulActivityArg &roArg, MulActivityRes &roRes);
	void OnDelayReplyRpc(const MulActivityArg &roArg, MulActivityRes &roRes, const CUserData &roUserData);

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
	MulActivityArg m_oArg;
	MulActivityRes m_oRes;
};

#endif
