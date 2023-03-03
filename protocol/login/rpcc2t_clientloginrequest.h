#ifndef __RPCC2T_CLIENTLOGINREQUEST_H__
#define __RPCC2T_CLIENTLOGINREQUEST_H__

// generate by ProtoGen at date: 2015/10/7 15:03:26

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2T_CLIENTLOGINREQUEST_TYPE 10091

class RpcC2T_ClientLoginRequest : public CRpc
{
private:

	RpcC2T_ClientLoginRequest():CRpc(RPCC2T_CLIENTLOGINREQUEST_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2T_ClientLoginRequest *CreateRpc()
	{
		return new RpcC2T_ClientLoginRequest;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LoginArg &roArg, const LoginRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LoginArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LoginArg &roArg, LoginRes &roRes);
	void OnDelayReplyRpc(const LoginArg &roArg, LoginRes &roRes, const CUserData &roUserData);

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
	LoginArg m_oArg;
	LoginRes m_oRes;
};

#endif
