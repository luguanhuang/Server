#ifndef __RPCM2G_GETROLEPOWER_H__
#define __RPCM2G_GETROLEPOWER_H__

// generate by ProtoGen at date: 2017/9/5 14:39:27

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_GETROLEPOWER_TYPE 22338

class RpcM2G_GetRolePower : public CRpc
{
private:

	RpcM2G_GetRolePower():CRpc(RPCM2G_GETROLEPOWER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_GetRolePower *CreateRpc()
	{
		return new RpcM2G_GetRolePower;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetRolePowerArg &roArg, const GetRolePowerRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetRolePowerArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetRolePowerArg &roArg, GetRolePowerRes &roRes);
	void OnDelayReplyRpc(const GetRolePowerArg &roArg, GetRolePowerRes &roRes, const CUserData &roUserData);

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
	GetRolePowerArg m_oArg;
	GetRolePowerRes m_oRes;
};

#endif
