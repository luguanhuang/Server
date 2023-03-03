#ifndef __RPCH2K_REGISTERTS2WORLD_H__
#define __RPCH2K_REGISTERTS2WORLD_H__

// generate by ProtoGen at date: 2017/5/21 20:11:37

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCH2K_REGISTERTS2WORLD_TYPE 60842

class RpcH2K_RegisterTs2World : public CRpc
{
private:

	RpcH2K_RegisterTs2World():CRpc(RPCH2K_REGISTERTS2WORLD_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcH2K_RegisterTs2World *CreateRpc()
	{
		return new RpcH2K_RegisterTs2World;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterTs2WorldArg &roArg, const RegisterTs2WorldRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterTs2WorldArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterTs2WorldArg &roArg, RegisterTs2WorldRes &roRes);
	void OnDelayReplyRpc(const RegisterTs2WorldArg &roArg, RegisterTs2WorldRes &roRes, const CUserData &roUserData);

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
	RegisterTs2WorldArg m_oArg;
	RegisterTs2WorldRes m_oRes;
};

#endif
