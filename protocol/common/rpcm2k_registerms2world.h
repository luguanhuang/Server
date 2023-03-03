#ifndef __RPCM2K_REGISTERMS2WORLD_H__
#define __RPCM2K_REGISTERMS2WORLD_H__

// generate by ProtoGen at date: 2017/3/31 17:14:02

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2K_REGISTERMS2WORLD_TYPE 11622

class RpcM2K_RegisterMs2World : public CRpc
{
private:

	RpcM2K_RegisterMs2World():CRpc(RPCM2K_REGISTERMS2WORLD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2K_RegisterMs2World *CreateRpc()
	{
		return new RpcM2K_RegisterMs2World;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterMs2WorldArg &roArg, const RegisterMs2WorldRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterMs2WorldArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterMs2WorldArg &roArg, RegisterMs2WorldRes &roRes);
	void OnDelayReplyRpc(const RegisterMs2WorldArg &roArg, RegisterMs2WorldRes &roRes, const CUserData &roUserData);

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
	RegisterMs2WorldArg m_oArg;
	RegisterMs2WorldRes m_oRes;
};

#endif
