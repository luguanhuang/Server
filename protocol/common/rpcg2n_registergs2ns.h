#ifndef __RPCG2N_REGISTERGS2NS_H__
#define __RPCG2N_REGISTERGS2NS_H__

// generate by ProtoGen at date: 2016/11/2 1:16:28

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2N_REGISTERGS2NS_TYPE 18862

class RpcG2N_RegisterGs2Ns : public CRpc
{
private:

	RpcG2N_RegisterGs2Ns():CRpc(RPCG2N_REGISTERGS2NS_TYPE)
	{
        m_dwTimeout = 100000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2N_RegisterGs2Ns *CreateRpc()
	{
		return new RpcG2N_RegisterGs2Ns;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterGs2NsArg &roArg, const RegisterGs2NsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterGs2NsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterGs2NsArg &roArg, RegisterGs2NsRes &roRes);
	void OnDelayReplyRpc(const RegisterGs2NsArg &roArg, RegisterGs2NsRes &roRes, const CUserData &roUserData);

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
	RegisterGs2NsArg m_oArg;
	RegisterGs2NsRes m_oRes;
};

#endif
