#ifndef __RPCM2I_REGISTERMS2LOGIN_H__
#define __RPCM2I_REGISTERMS2LOGIN_H__

// generate by ProtoGen at date: 2016/11/7 17:00:53

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2I_REGISTERMS2LOGIN_TYPE 21001

class RpcM2I_RegisterMs2Login : public CRpc
{
private:

	RpcM2I_RegisterMs2Login():CRpc(RPCM2I_REGISTERMS2LOGIN_TYPE)
	{
        m_dwTimeout = 60000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2I_RegisterMs2Login *CreateRpc()
	{
		return new RpcM2I_RegisterMs2Login;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterMs2LoginArg &roArg, const RegisterMs2LoginRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterMs2LoginArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterMs2LoginArg &roArg, RegisterMs2LoginRes &roRes);
	void OnDelayReplyRpc(const RegisterMs2LoginArg &roArg, RegisterMs2LoginRes &roRes, const CUserData &roUserData);

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
	RegisterMs2LoginArg m_oArg;
	RegisterMs2LoginRes m_oRes;
};

#endif
