#ifndef __RPCT2N_LOGINREQUESTNEW_H__
#define __RPCT2N_LOGINREQUESTNEW_H__

// generate by ProtoGen at date: 2016/11/3 2:18:52

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCT2N_LOGINREQUESTNEW_TYPE 44486

class RpcT2N_LoginRequestNew : public CRpc
{
private:

	RpcT2N_LoginRequestNew():CRpc(RPCT2N_LOGINREQUESTNEW_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcT2N_LoginRequestNew *CreateRpc()
	{
		return new RpcT2N_LoginRequestNew;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LoginRequestNewArg &roArg, const LoginRequestNewRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LoginRequestNewArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LoginRequestNewArg &roArg, LoginRequestNewRes &roRes);
	void OnDelayReplyRpc(const LoginRequestNewArg &roArg, LoginRequestNewRes &roRes, const CUserData &roUserData);

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
	LoginRequestNewArg m_oArg;
	LoginRequestNewRes m_oRes;
};

#endif
