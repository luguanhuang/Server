#ifndef __RPCG2M_REGISTERGS2MS_H__
#define __RPCG2M_REGISTERGS2MS_H__

// generate by ProtoGen at date: 2017/5/18 14:53:45

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_REGISTERGS2MS_TYPE 4755

class RpcG2M_RegisterGs2Ms : public CRpc
{
private:

	RpcG2M_RegisterGs2Ms():CRpc(RPCG2M_REGISTERGS2MS_TYPE)
	{
        m_dwTimeout = 2000000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_RegisterGs2Ms *CreateRpc()
	{
		return new RpcG2M_RegisterGs2Ms;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterGs2MsArg &roArg, const RegisterGs2MsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterGs2MsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterGs2MsArg &roArg, RegisterGs2MsRes &roRes);
	void OnDelayReplyRpc(const RegisterGs2MsArg &roArg, RegisterGs2MsRes &roRes, const CUserData &roUserData);

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
	RegisterGs2MsArg m_oArg;
	RegisterGs2MsRes m_oRes;
};

#endif
