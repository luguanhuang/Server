#ifndef __RPCC2G_LOGINACTIVITY_H__
#define __RPCC2G_LOGINACTIVITY_H__

// generate by ProtoGen at date: 2017/4/26 19:24:41

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_LOGINACTIVITY_TYPE 64167

class RpcC2G_LoginActivity : public CRpc
{
private:

	RpcC2G_LoginActivity():CRpc(RPCC2G_LOGINACTIVITY_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_LoginActivity *CreateRpc()
	{
		return new RpcC2G_LoginActivity;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LoginActivityArg &roArg, const LoginActivityRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LoginActivityArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LoginActivityArg &roArg, LoginActivityRes &roRes);
	void OnDelayReplyRpc(const LoginActivityArg &roArg, LoginActivityRes &roRes, const CUserData &roUserData);

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
	LoginActivityArg m_oArg;
	LoginActivityRes m_oRes;
};

#endif
