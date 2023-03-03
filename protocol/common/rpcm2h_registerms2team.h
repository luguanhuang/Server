#ifndef __RPCM2H_REGISTERMS2TEAM_H__
#define __RPCM2H_REGISTERMS2TEAM_H__

// generate by ProtoGen at date: 2017/4/24 15:05:17

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2H_REGISTERMS2TEAM_TYPE 63997

class RpcM2H_RegisterMs2Team : public CRpc
{
private:

	RpcM2H_RegisterMs2Team():CRpc(RPCM2H_REGISTERMS2TEAM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2H_RegisterMs2Team *CreateRpc()
	{
		return new RpcM2H_RegisterMs2Team;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterMs2HArg &roArg, const RegisterMs2HRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterMs2HArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterMs2HArg &roArg, RegisterMs2HRes &roRes);
	void OnDelayReplyRpc(const RegisterMs2HArg &roArg, RegisterMs2HRes &roRes, const CUserData &roUserData);

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
	RegisterMs2HArg m_oArg;
	RegisterMs2HRes m_oRes;
};

#endif
