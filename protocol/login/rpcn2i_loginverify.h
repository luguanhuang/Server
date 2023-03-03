#ifndef __RPCN2I_LOGINVERIFY_H__
#define __RPCN2I_LOGINVERIFY_H__

// generate by ProtoGen at date: 2016/11/2 20:14:40

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCN2I_LOGINVERIFY_TYPE 42255

class RpcN2I_LoginVerify : public CRpc
{
private:

	RpcN2I_LoginVerify():CRpc(RPCN2I_LOGINVERIFY_TYPE)
	{
        m_dwTimeout = 30000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcN2I_LoginVerify *CreateRpc()
	{
		return new RpcN2I_LoginVerify;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const LoginVerifyArg &roArg, const LoginVerifyRes &roRes, const CUserData &roUserData);
	void OnTimeout(const LoginVerifyArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const LoginVerifyArg &roArg, LoginVerifyRes &roRes);
	void OnDelayReplyRpc(const LoginVerifyArg &roArg, LoginVerifyRes &roRes, const CUserData &roUserData);

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
	LoginVerifyArg m_oArg;
	LoginVerifyRes m_oRes;
};

#endif
