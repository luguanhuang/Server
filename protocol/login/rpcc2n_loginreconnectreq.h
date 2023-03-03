#ifndef __RPCC2N_LOGINRECONNECTREQ_H__
#define __RPCC2N_LOGINRECONNECTREQ_H__

// generate by ProtoGen at date: 2017/6/21 17:36:53

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2N_LOGINRECONNECTREQ_TYPE 25422

class RpcC2N_LoginReconnectReq : public CRpc
{
private:

	RpcC2N_LoginReconnectReq():CRpc(RPCC2N_LOGINRECONNECTREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2N_LoginReconnectReq *CreateRpc()
	{
		return new RpcC2N_LoginReconnectReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LoginReconnectReqArg &roArg, const LoginReconnectReqRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LoginReconnectReqArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LoginReconnectReqArg &roArg, LoginReconnectReqRes &roRes);
	void OnDelayReplyRpc(const LoginReconnectReqArg &roArg, LoginReconnectReqRes &roRes, const CUserData &roUserData);

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
	LoginReconnectReqArg m_oArg;
	LoginReconnectReqRes m_oRes;
};

#endif
